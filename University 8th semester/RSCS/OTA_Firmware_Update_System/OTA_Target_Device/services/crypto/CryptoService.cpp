#include "CryptoService.h"

#include "config.h"

#include <Arduino.h>

#include "mbedtls/pk.h"

#include <cstring>

bool CryptoService::beginHash()
{
    if (hashActive_)
    {
        mbedtls_sha256_free(&sha_);
    }

    mbedtls_sha256_init(&sha_);
    mbedtls_sha256_starts(&sha_, 0);

    hashActive_ = true;
    return true;
}

void CryptoService::updateHash(const uint8_t* data, size_t len)
{
    if (hashActive_)
    {
        mbedtls_sha256_update(&sha_, data, len);
    }
}

bool CryptoService::verifySignature(const uint8_t* signature, size_t sig_len)
{
    if (!hashActive_)
    {
        return false;
    }

    std::array<uint8_t, SHA256_HASH_LEN> hash{};
    mbedtls_sha256_finish(&sha_, hash.data());
    mbedtls_sha256_free(&sha_);
    hashActive_ = false;

    mbedtls_pk_context pk;
    mbedtls_pk_init(&pk);

    int ret = mbedtls_pk_parse_public_key(&pk, ECDSA_PUBLIC_KEY, ECDSA_PUBLIC_KEY_LEN);
    if (ret != 0)
    {
        Serial.printf("[Crypto] pk_parse_public_key failed: -0x%04X\n", -ret);
        mbedtls_pk_free(&pk);
        return false;
    }

    ret = mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, hash.data(), hash.size(), signature, sig_len);

    mbedtls_pk_free(&pk);

    if (ret != 0)
    {
        Serial.printf("[Crypto] Signature verification FAILED: -0x%04X\n", -ret);
        return false;
    }

    Serial.println("[Crypto] Signature verification OK");
    return true;
}

bool CryptoService::beginDecryption(const std::array<uint8_t, IV_LEN>& iv)
{
    if (gcmActive_)
    {
        mbedtls_gcm_free(&gcm_);
    }

    mbedtls_gcm_init(&gcm_);

    int ret = mbedtls_gcm_setkey(&gcm_, MBEDTLS_CIPHER_ID_AES, AES_KEY, AES_KEY_BITS);
    if (ret != 0)
    {
        Serial.printf("[Crypto] gcm_setkey failed: -0x%04X\n", -ret);
        mbedtls_gcm_free(&gcm_);
        gcmActive_ = false;
        return false;
    }

    ret = mbedtls_gcm_starts(&gcm_, MBEDTLS_GCM_DECRYPT, iv.data(), iv.size(), nullptr, 0);
    if (ret != 0)
    {
        Serial.printf("[Crypto] gcm_starts failed: -0x%04X\n", -ret);
        mbedtls_gcm_free(&gcm_);
        gcmActive_ = false;
        return false;
    }

    gcmActive_ = true;
    return true;
}

bool CryptoService::decrypt(const uint8_t* input, size_t len, uint8_t* output)
{
    if (!gcmActive_)
    {
        return false;
    }

    int ret = mbedtls_gcm_update(&gcm_, len, input, output);
    if (ret != 0)
    {
        Serial.printf("[Crypto] gcm_update failed: -0x%04X\n", -ret);
        return false;
    }
    return true;
}

bool CryptoService::finishDecryption(const std::array<uint8_t, GCM_TAG_LEN>& expected_tag)
{
    if (!gcmActive_)
    {
        return false;
    }

    std::array<uint8_t, GCM_TAG_LEN> computed_tag{};
    int ret = mbedtls_gcm_finish(&gcm_, computed_tag.data(), computed_tag.size());

    mbedtls_gcm_free(&gcm_);
    gcmActive_ = false;

    if (ret != 0)
    {
        Serial.printf("[Crypto] gcm_finish failed: -0x%04X\n", -ret);
        return false;
    }

    if (std::memcmp(computed_tag.data(), expected_tag.data(), GCM_TAG_LEN) != 0)
    {
        Serial.println("[Crypto] GCM auth tag MISMATCH");
        return false;
    }

    Serial.println("[Crypto] GCM auth tag OK");
    return true;
}
