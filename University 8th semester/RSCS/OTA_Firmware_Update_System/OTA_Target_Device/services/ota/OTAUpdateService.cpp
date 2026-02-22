#include "OTAUpdateService.h"

#include "config.h"
#include "version.h"

#include <Arduino.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "esp_ota_ops.h"
#include "esp_partition.h"

#include <array>
#include <cstring>

OTAUpdateService::OTAUpdateService(WifiManager& wifi, ManifestFetcher& manifestFetcher, CryptoService& crypto)
    : wifi_(wifi), manifestFetcher_(manifestFetcher), crypto_(crypto)
{
    startPeriodicCheck();
}

void OTAUpdateService::startPeriodicCheck()
{
    otaCheckTicker_.attach(
        OTA_CHECK_INTERVAL_S, +[](OTAUpdateService* self) { self->checkPendingUpdate_ = true; }, this);
}

void OTAUpdateService::markCurrentFirmwareValid()
{
    esp_ota_mark_app_valid_cancel_rollback();
    Serial.println("[OTA] Current firmware marked as VALID");
}

void OTAUpdateService::checkForUpdate()
{
    if (!checkPendingUpdate_)
    {
        return;
    }

    checkPendingUpdate_ = false;

    Serial.println();
    Serial.println("[OTA] ---- OTA check ----");

    FirmwareManifest manifest;
    if (manifestFetcher_.fetch(MANIFEST_URL, manifest))
    {
        performUpdate(manifest);
    }
    else
    {
        Serial.println("[OTA] Manifest fetch failed");
    }
}

template<typename ChunkCallback>
bool OTAUpdateService::httpsStreamDownload(const String& url, size_t expected_size, ChunkCallback on_chunk)
{
    WiFiClientSecure client;
    client.setInsecure(); // ECDSA signature is the trust anchor, not TLS

    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("[OTA] HTTP GET failed: %d\n", httpCode);
        http.end();
        return false;
    }

    int contentLength = http.getSize();
    if (contentLength > 0 && static_cast<size_t>(contentLength) != expected_size)
    {
        Serial.printf("[OTA] Size mismatch: manifest=%u, server=%d\n", expected_size, contentLength);
        http.end();
        return false;
    }

    WiFiClient* stream = http.getStreamPtr();
    static std::array<uint8_t, CHUNK_SIZE> buf;
    size_t totalRead = 0;

    while (http.connected() && totalRead < expected_size)
    {
        size_t remaining = expected_size - totalRead;
        size_t toRead    = (remaining < CHUNK_SIZE) ? remaining : CHUNK_SIZE;

        int bytesRead = stream->readBytes(buf.data(), toRead);
        if (bytesRead <= 0)
        {
            Serial.println("[OTA] Stream read timeout");
            http.end();
            return false;
        }

        if (!on_chunk(buf.data(), static_cast<size_t>(bytesRead)))
        {
            http.end();
            return false;
        }

        totalRead += bytesRead;
    }

    http.end();

    if (totalRead != expected_size)
    {
        Serial.printf("[OTA] Incomplete download: %u / %u\n", totalRead, expected_size);
        return false;
    }

    return true;
}

bool OTAUpdateService::performUpdate(const FirmwareManifest& manifest)
{
    if (manifest.version <= FIRMWARE_VERSION)
    {
        Serial.printf("[OTA] No update needed (current=%u, remote=%u)\n", static_cast<uint32_t>(FIRMWARE_VERSION),
                      manifest.version);
        return false;
    }

    Serial.printf("[OTA] Update available: v%u → v%u (%u bytes)\n", static_cast<uint32_t>(FIRMWARE_VERSION),
                  manifest.version, manifest.firmware_size);

    std::array<uint8_t, VERSION_BYTES_LEN> version_bytes{};
    uint32_t ver = manifest.version;
    std::memcpy(version_bytes.data(), &ver, VERSION_BYTES_LEN);

    // Pass 1: download + hash for signature verification
    // Hash = SHA-256( IV || ciphertext || version_bytes )
    Serial.println("[OTA] Pass 1: signature verification...");

    if (!crypto_.beginHash())
    {
        return false;
    }

    crypto_.updateHash(manifest.iv);

    bool ok = httpsStreamDownload(manifest.firmware_url, manifest.firmware_size,
                                  [this](const uint8_t* buf, size_t len) -> bool {
                                      crypto_.updateHash(buf, len);
                                      return true;
                                  });

    if (!ok)
    {
        Serial.println("[OTA] Pass 1 download failed");
        return false;
    }

    crypto_.updateHash(version_bytes);

    if (!crypto_.verifySignature(manifest.signature.data(), manifest.signature_len))
    {
        Serial.println("[OTA] ABORTING — signature invalid!");
        return false;
    }

    // Pass 2: download + decrypt + flash
    Serial.println("[OTA] Pass 2: decrypt and flash...");

    const esp_partition_t* update_partition = esp_ota_get_next_update_partition(nullptr);
    if (!update_partition)
    {
        Serial.println("[OTA] No OTA partition available");
        return false;
    }

    Serial.printf("[OTA] Writing to partition: %s (offset 0x%08X)\n", update_partition->label,
                  update_partition->address);

    esp_ota_handle_t ota_handle;
    esp_err_t err = esp_ota_begin(update_partition, manifest.firmware_size, &ota_handle);
    if (err != ESP_OK)
    {
        Serial.printf("[OTA] esp_ota_begin failed: 0x%04X\n", err);
        return false;
    }

    if (!crypto_.beginDecryption(manifest.iv))
    {
        esp_ota_abort(ota_handle);
        return false;
    }

    static std::array<uint8_t, CHUNK_SIZE> decrypt_buf;
    bool flash_ok = httpsStreamDownload(manifest.firmware_url, manifest.firmware_size,
                                        [this, &ota_handle](const uint8_t* buf, size_t len) -> bool {
                                            if (!crypto_.decrypt(buf, len, decrypt_buf.data()))
                                            {
                                                return false;
                                            }
                                            esp_err_t werr = esp_ota_write(ota_handle, decrypt_buf.data(), len);
                                            if (werr != ESP_OK)
                                            {
                                                Serial.printf("[OTA] esp_ota_write failed: 0x%04X\n", werr);
                                                return false;
                                            }
                                            return true;
                                        });

    if (!flash_ok)
    {
        Serial.println("[OTA] Pass 2 failed");
        esp_ota_abort(ota_handle);
        return false;
    }

    if (!crypto_.finishDecryption(manifest.tag))
    {
        Serial.println("[OTA] ABORTING — GCM auth tag mismatch!");
        esp_ota_abort(ota_handle);
        return false;
    }

    err = esp_ota_end(ota_handle);
    if (err != ESP_OK)
    {
        Serial.printf("[OTA] esp_ota_end failed: 0x%04X\n", err);
        return false;
    }

    // Activate new partition and reboot
    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK)
    {
        Serial.printf("[OTA] esp_ota_set_boot_partition failed: 0x%04X\n", err);
        return false;
    }

    Serial.println("[OTA] ========================================");
    Serial.println("[OTA]  Update installed successfully!");
    Serial.println("[OTA]  Rebooting into new firmware...");
    Serial.println("[OTA] ========================================");

    delay(REBOOT_DELAY_MS);
    esp_restart();

    return true;
}
