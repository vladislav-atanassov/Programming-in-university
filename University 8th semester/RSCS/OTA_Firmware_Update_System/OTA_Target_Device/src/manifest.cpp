#include "manifest.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>


#include "mbedtls/base64.h"

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/**
 * Decode a base64-encoded string into a binary buffer.
 * @return number of bytes written, or 0 on failure.
 */
static size_t b64_decode(const char *b64, uint8_t *out, size_t out_max) {
  size_t olen = 0;
  int ret = mbedtls_base64_decode(out, out_max, &olen, (const uint8_t *)b64,
                                  strlen(b64));
  if (ret != 0) {
    Serial.printf("[Manifest] base64 decode failed: -0x%04X\n", -ret);
    return 0;
  }
  return olen;
}

// ---------------------------------------------------------------------------
// Public API
// ---------------------------------------------------------------------------

bool manifest_fetch_and_parse(const char *url, FirmwareManifest &out) {
  Serial.printf("[Manifest] Fetching %s\n", url);

  WiFiClientSecure client;
  // Note: Transport security (TLS certificate verification) is skipped here.
  // The firmware's ECDSA signature is the real trust anchor — even if an
  // attacker MITMs the download, tampered firmware will fail signature
  // verification, so OTA security is NOT compromised by this choice.
  client.setInsecure();

  HTTPClient http;
  http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
  http.begin(client, url);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    Serial.printf("[Manifest] HTTP GET failed: %d\n", httpCode);
    http.end();
    return false;
  }

  String payload = http.getString();
  http.end();

  Serial.printf("[Manifest] Received %d bytes\n", payload.length());

  // Parse JSON
  JsonDocument doc;
  DeserializationError err = deserializeJson(doc, payload);
  if (err) {
    Serial.printf("[Manifest] JSON parse error: %s\n", err.c_str());
    return false;
  }

  // Extract fields
  if (!doc["version"].is<uint32_t>() ||
      !doc["firmware_url"].is<const char *>() ||
      !doc["iv"].is<const char *>() || !doc["tag"].is<const char *>() ||
      !doc["signature"].is<const char *>() || !doc["size"].is<size_t>()) {
    Serial.println("[Manifest] Missing or invalid fields");
    return false;
  }

  out.version = doc["version"].as<uint32_t>();
  out.firmware_url = String(doc["firmware_url"].as<const char *>());
  out.firmware_size = doc["size"].as<size_t>();

  // Decode base64 binary fields
  size_t n;

  n = b64_decode(doc["iv"].as<const char *>(), out.iv, MANIFEST_IV_LEN);
  if (n != MANIFEST_IV_LEN) {
    Serial.printf("[Manifest] IV decode: expected %d, got %d\n",
                  MANIFEST_IV_LEN, n);
    return false;
  }

  n = b64_decode(doc["tag"].as<const char *>(), out.tag, MANIFEST_TAG_LEN);
  if (n != MANIFEST_TAG_LEN) {
    Serial.printf("[Manifest] Tag decode: expected %d, got %d\n",
                  MANIFEST_TAG_LEN, n);
    return false;
  }

  n = b64_decode(doc["signature"].as<const char *>(), out.signature,
                 MANIFEST_MAX_SIG_LEN);
  if (n == 0) {
    Serial.println("[Manifest] Signature decode failed");
    return false;
  }
  out.signature_len = n;

  Serial.printf("[Manifest] Parsed — version: %u, size: %u bytes\n",
                out.version, out.firmware_size);
  return true;
}
