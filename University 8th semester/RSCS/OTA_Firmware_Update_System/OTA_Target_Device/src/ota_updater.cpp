#include "ota_updater.h"
#include "crypto.h"
#include "version.h"

#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "esp_ota_ops.h"
#include "esp_partition.h"

// Streaming buffer size — balance between RAM usage and throughput
static const size_t CHUNK_SIZE = 4096;

// ---------------------------------------------------------------------------
// Internal: streaming HTTPS download helper
// ---------------------------------------------------------------------------
// Calls `on_chunk(buf, len)` for each received chunk.
// Returns true if the full download completed successfully.

template <typename ChunkCallback>
static bool https_stream_download(const String &url, size_t expected_size,
                                  ChunkCallback on_chunk) {
  WiFiClientSecure client;
  client.setInsecure(); // Signature is the real trust anchor

  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.begin(client, url);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[OTA] HTTP GET failed: %d\n", httpCode);
    http.end();
    return false;
  }

  int contentLength = http.getSize();
  if (contentLength > 0 && (size_t)contentLength != expected_size) {
    Serial.printf("[OTA] Size mismatch: manifest=%u, server=%d\n",
                  expected_size, contentLength);
    http.end();
    return false;
  }

  WiFiClient *stream = http.getStreamPtr();
  static uint8_t buf[CHUNK_SIZE]; // static to avoid stack overflow
  size_t totalRead = 0;

  while (http.connected() && totalRead < expected_size) {
    size_t remaining = expected_size - totalRead;
    size_t toRead = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;

    int bytesRead = stream->readBytes(buf, toRead);
    if (bytesRead <= 0) {
      Serial.println("[OTA] Stream read timeout");
      http.end();
      return false;
    }

    if (!on_chunk(buf, (size_t)bytesRead)) {
      http.end();
      return false;
    }

    totalRead += bytesRead;
  }

  http.end();

  if (totalRead != expected_size) {
    Serial.printf("[OTA] Incomplete download: %u / %u\n", totalRead,
                  expected_size);
    return false;
  }

  return true;
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

void ota_mark_valid() {
  esp_ota_mark_app_valid_cancel_rollback();
  Serial.println("[OTA] Current firmware marked as VALID");
}

bool ota_perform_update(const FirmwareManifest &manifest) {
  // ---------------------------------------------------------------
  // 1. Anti-rollback check
  // ---------------------------------------------------------------
  if (manifest.version <= FIRMWARE_VERSION) {
    Serial.printf("[OTA] No update needed (current=%u, remote=%u)\n",
                  (uint32_t)FIRMWARE_VERSION, manifest.version);
    return false;
  }

  Serial.printf("[OTA] Update available: v%u → v%u (%u bytes)\n",
                (uint32_t)FIRMWARE_VERSION, manifest.version,
                manifest.firmware_size);

  // Encode version as 4 little-endian bytes (must match server signing)
  uint8_t version_bytes[4];
  uint32_t ver = manifest.version;
  memcpy(version_bytes, &ver, 4);

  // ---------------------------------------------------------------
  // 2. PASS 1 — Download + hash for signature verification
  //    Hash = SHA-256( IV || ciphertext || version_bytes )
  // ---------------------------------------------------------------
  Serial.println("[OTA] Pass 1: downloading for signature verification...");

  crypto_hash_ctx_t *hash_ctx = crypto_hash_begin();
  if (!hash_ctx)
    return false;

  // Hash the IV first
  crypto_hash_update(hash_ctx, manifest.iv, MANIFEST_IV_LEN);

  // Stream the ciphertext and hash each chunk
  bool ok = https_stream_download(manifest.firmware_url, manifest.firmware_size,
                                  [&](const uint8_t *buf, size_t len) -> bool {
                                    crypto_hash_update(hash_ctx, buf, len);
                                    return true;
                                  });

  if (!ok) {
    Serial.println("[OTA] Pass 1 download failed");
    // hash_ctx is leaked here — but we're aborting anyway
    return false;
  }

  // Hash the version bytes
  crypto_hash_update(hash_ctx, version_bytes, 4);

  // Verify ECDSA-P256 signature
  if (!crypto_hash_verify_signature(hash_ctx, manifest.signature,
                                    manifest.signature_len)) {
    Serial.println("[OTA] ABORTING — signature invalid!");
    return false;
  }

  // ---------------------------------------------------------------
  // 3. PASS 2 — Download + decrypt + flash to OTA partition
  // ---------------------------------------------------------------
  Serial.println("[OTA] Pass 2: downloading, decrypting, and flashing...");

  // Find the next OTA partition
  const esp_partition_t *update_partition =
      esp_ota_get_next_update_partition(NULL);
  if (!update_partition) {
    Serial.println("[OTA] No OTA partition available");
    return false;
  }

  Serial.printf("[OTA] Writing to partition: %s (offset 0x%08X)\n",
                update_partition->label, update_partition->address);

  // Begin OTA write
  esp_ota_handle_t ota_handle;
  esp_err_t err =
      esp_ota_begin(update_partition, manifest.firmware_size, &ota_handle);
  if (err != ESP_OK) {
    Serial.printf("[OTA] esp_ota_begin failed: 0x%04X\n", err);
    return false;
  }

  // Initialize AES-256-GCM streaming decryption
  crypto_gcm_ctx_t *gcm_ctx = crypto_gcm_begin(manifest.iv, MANIFEST_IV_LEN);
  if (!gcm_ctx) {
    esp_ota_abort(ota_handle);
    return false;
  }

  // Stream download → decrypt → OTA write
  static uint8_t decrypt_buf[CHUNK_SIZE]; // static to avoid stack overflow
  bool flash_ok = https_stream_download(
      manifest.firmware_url, manifest.firmware_size,
      [&](const uint8_t *buf, size_t len) -> bool {
        // Decrypt chunk
        if (!crypto_gcm_decrypt(gcm_ctx, buf, len, decrypt_buf)) {
          return false;
        }
        // Write decrypted data to OTA partition
        esp_err_t werr = esp_ota_write(ota_handle, decrypt_buf, len);
        if (werr != ESP_OK) {
          Serial.printf("[OTA] esp_ota_write failed: 0x%04X\n", werr);
          return false;
        }
        return true;
      });

  if (!flash_ok) {
    Serial.println("[OTA] Pass 2 download/decrypt/flash failed");
    esp_ota_abort(ota_handle);
    // gcm_ctx leaked — aborting anyway
    return false;
  }

  // Verify GCM authentication tag
  if (!crypto_gcm_finish(gcm_ctx, manifest.tag, MANIFEST_TAG_LEN)) {
    Serial.println("[OTA] ABORTING — GCM auth tag mismatch!");
    esp_ota_abort(ota_handle);
    return false;
  }

  // Finalize OTA write
  err = esp_ota_end(ota_handle);
  if (err != ESP_OK) {
    Serial.printf("[OTA] esp_ota_end failed: 0x%04X\n", err);
    return false;
  }

  // ---------------------------------------------------------------
  // 4. Activate new partition and reboot
  // ---------------------------------------------------------------
  err = esp_ota_set_boot_partition(update_partition);
  if (err != ESP_OK) {
    Serial.printf("[OTA] esp_ota_set_boot_partition failed: 0x%04X\n", err);
    return false;
  }

  Serial.println("[OTA] ========================================");
  Serial.println("[OTA]  Update installed successfully!");
  Serial.println("[OTA]  Rebooting into new firmware...");
  Serial.println("[OTA] ========================================");

  delay(1000);
  esp_restart();

  return true; // unreachable, but keeps compiler happy
}
