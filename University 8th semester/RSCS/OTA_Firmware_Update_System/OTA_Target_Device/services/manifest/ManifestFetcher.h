#ifndef MANIFEST_FETCHER_H
#define MANIFEST_FETCHER_H

#include <Arduino.h>
#include <array>
#include <cstddef>
#include <cstdint>

struct FirmwareManifest
{
    static constexpr size_t IV_LEN      = 12;
    static constexpr size_t TAG_LEN     = 16;
    static constexpr size_t MAX_SIG_LEN = 72;

    uint32_t version = 0;
    String firmware_url;
    std::array<uint8_t, IV_LEN> iv{};
    std::array<uint8_t, TAG_LEN> tag{};
    std::array<uint8_t, MAX_SIG_LEN> signature{};
    size_t signature_len = 0;
    size_t firmware_size = 0;
};

class ManifestFetcher
{
public:
    bool fetch(const char* url, FirmwareManifest& out);

private:
    static size_t b64Decode(const char* b64, uint8_t* out, size_t out_max);
};

#endif // MANIFEST_FETCHER_H
