#ifndef CRYPTO_SERVICE_H
#define CRYPTO_SERVICE_H

#include <array>
#include <cstddef>
#include <cstdint>

#include "mbedtls/gcm.h"    // IWYU pragma: keep
#include "mbedtls/sha256.h" // IWYU pragma: keep

class CryptoService
{
public:
    static constexpr size_t SHA256_HASH_LEN = 32;
    static constexpr size_t GCM_TAG_LEN     = 16;
    static constexpr size_t AES_KEY_BITS    = 256;
    static constexpr size_t IV_LEN          = 12;

    bool beginHash();
    void updateHash(const uint8_t* data, size_t len);
    bool verifySignature(const uint8_t* signature, size_t sig_len);

    bool beginDecryption(const std::array<uint8_t, IV_LEN>& iv);
    bool decrypt(const uint8_t* input, size_t len, uint8_t* output);
    bool finishDecryption(const std::array<uint8_t, GCM_TAG_LEN>& expected_tag);

    template<size_t N> void updateHash(const std::array<uint8_t, N>& data)
    {
        updateHash(data.data(), N);
    }

private:
    mbedtls_sha256_context sha_{};
    mbedtls_gcm_context gcm_{};
    bool hashActive_ = false;
    bool gcmActive_  = false;
};

#endif // CRYPTO_SERVICE_H
