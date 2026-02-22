#include "ManifestFetcher.h"

#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>

#include "mbedtls/base64.h"

size_t ManifestFetcher::b64Decode(const char* b64, uint8_t* out, size_t out_max)
{
    size_t olen = 0;
    int ret     = mbedtls_base64_decode(out, out_max, &olen, reinterpret_cast<const uint8_t*>(b64), strlen(b64));
    if (ret != 0)
    {
        Serial.printf("[Manifest] base64 decode failed: -0x%04X\n", -ret);
        return 0;
    }
    return olen;
}

bool ManifestFetcher::fetch(const char* url, FirmwareManifest& out)
{
    Serial.printf("[Manifest] Fetching %s\n", url);

    WiFiClientSecure client;
    client.setInsecure(); // ECDSA signature is the trust anchor, not TLS

    HTTPClient http;
    http.setFollowRedirects(HTTPC_STRICT_FOLLOW_REDIRECTS);
    http.begin(client, url);

    int httpCode = http.GET();
    if (httpCode != HTTP_CODE_OK)
    {
        Serial.printf("[Manifest] HTTP GET failed: %d\n", httpCode);
        http.end();
        return false;
    }

    String payload = http.getString();
    http.end();

    Serial.printf("[Manifest] Received %d bytes\n", payload.length());

    JsonDocument doc;
    DeserializationError err = deserializeJson(doc, payload);
    if (err)
    {
        Serial.printf("[Manifest] JSON parse error: %s\n", err.c_str());
        return false;
    }

    if (!doc["version"].is<uint32_t>() || !doc["firmware_url"].is<const char*>() || !doc["iv"].is<const char*>() ||
        !doc["tag"].is<const char*>() || !doc["signature"].is<const char*>() || !doc["size"].is<size_t>())
    {
        Serial.println("[Manifest] Missing or invalid fields");
        return false;
    }

    out.version       = doc["version"].as<uint32_t>();
    out.firmware_url  = String(doc["firmware_url"].as<const char*>());
    out.firmware_size = doc["size"].as<size_t>();

    size_t n;

    n = b64Decode(doc["iv"].as<const char*>(), out.iv.data(), FirmwareManifest::IV_LEN);
    if (n != FirmwareManifest::IV_LEN)
    {
        Serial.printf("[Manifest] IV decode: expected %d, got %d\n", FirmwareManifest::IV_LEN, n);
        return false;
    }

    n = b64Decode(doc["tag"].as<const char*>(), out.tag.data(), FirmwareManifest::TAG_LEN);
    if (n != FirmwareManifest::TAG_LEN)
    {
        Serial.printf("[Manifest] Tag decode: expected %d, got %d\n", FirmwareManifest::TAG_LEN, n);
        return false;
    }

    n = b64Decode(doc["signature"].as<const char*>(), out.signature.data(), FirmwareManifest::MAX_SIG_LEN);
    if (n == 0)
    {
        Serial.println("[Manifest] Signature decode failed");
        return false;
    }
    out.signature_len = n;

    Serial.printf("[Manifest] Parsed — version: %u, size: %u bytes\n", out.version, out.firmware_size);
    return true;
}
