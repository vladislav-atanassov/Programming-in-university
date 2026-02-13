#include "crypto.h"
#include "config.h"

#include <Arduino.h>

#include "mbedtls/gcm.h"
#include "mbedtls/pk.h"
#include "mbedtls/sha256.h"

#include <stdlib.h>
#include <string.h>

// ===================================================================
// Streaming SHA-256 + ECDSA-P256 signature verification
// ===================================================================

struct crypto_hash_ctx {
  mbedtls_sha256_context sha;
};

crypto_hash_ctx_t *crypto_hash_begin() {
  crypto_hash_ctx_t *ctx =
      (crypto_hash_ctx_t *)malloc(sizeof(crypto_hash_ctx_t));
  if (!ctx) {
    Serial.println("[Crypto] hash ctx alloc failed");
    return nullptr;
  }

  mbedtls_sha256_init(&ctx->sha);
  mbedtls_sha256_starts(&ctx->sha, 0); // 0 = SHA-256 (not SHA-224)
  return ctx;
}

void crypto_hash_update(crypto_hash_ctx_t *ctx, const uint8_t *data,
                        size_t len) {
  if (ctx) {
    mbedtls_sha256_update(&ctx->sha, data, len);
  }
}

bool crypto_hash_verify_signature(crypto_hash_ctx_t *ctx,
                                  const uint8_t *signature, size_t sig_len) {
  if (!ctx)
    return false;

  // Finalize the SHA-256 hash
  uint8_t hash[32];
  mbedtls_sha256_finish(&ctx->sha, hash);
  mbedtls_sha256_free(&ctx->sha);

  // Parse the ECDSA P-256 public key
  mbedtls_pk_context pk;
  mbedtls_pk_init(&pk);

  int ret =
      mbedtls_pk_parse_public_key(&pk, ECDSA_PUBLIC_KEY, ECDSA_PUBLIC_KEY_LEN);
  if (ret != 0) {
    Serial.printf("[Crypto] pk_parse_public_key failed: -0x%04X\n", -ret);
    mbedtls_pk_free(&pk);
    free(ctx);
    return false;
  }

  // Verify ECDSA signature over the SHA-256 hash
  ret = mbedtls_pk_verify(&pk, MBEDTLS_MD_SHA256, hash, sizeof(hash), signature,
                          sig_len);

  mbedtls_pk_free(&pk);
  free(ctx);

  if (ret != 0) {
    Serial.printf("[Crypto] Signature verification FAILED: -0x%04X\n", -ret);
    return false;
  }

  Serial.println("[Crypto] Signature verification OK");
  return true;
}

// ===================================================================
// Streaming AES-256-GCM decryption
// ===================================================================

struct crypto_gcm_ctx {
  mbedtls_gcm_context gcm;
};

crypto_gcm_ctx_t *crypto_gcm_begin(const uint8_t *iv, size_t iv_len) {
  crypto_gcm_ctx_t *ctx = (crypto_gcm_ctx_t *)malloc(sizeof(crypto_gcm_ctx_t));
  if (!ctx) {
    Serial.println("[Crypto] gcm ctx alloc failed");
    return nullptr;
  }

  mbedtls_gcm_init(&ctx->gcm);

  int ret = mbedtls_gcm_setkey(&ctx->gcm, MBEDTLS_CIPHER_ID_AES, AES_KEY, 256);
  if (ret != 0) {
    Serial.printf("[Crypto] gcm_setkey failed: -0x%04X\n", -ret);
    mbedtls_gcm_free(&ctx->gcm);
    free(ctx);
    return nullptr;
  }

  ret = mbedtls_gcm_starts(&ctx->gcm, MBEDTLS_GCM_DECRYPT, iv, iv_len, NULL,
                           0); // no AAD
  if (ret != 0) {
    Serial.printf("[Crypto] gcm_starts failed: -0x%04X\n", -ret);
    mbedtls_gcm_free(&ctx->gcm);
    free(ctx);
    return nullptr;
  }

  return ctx;
}

bool crypto_gcm_decrypt(crypto_gcm_ctx_t *ctx, const uint8_t *input, size_t len,
                        uint8_t *output) {
  if (!ctx)
    return false;

  int ret = mbedtls_gcm_update(&ctx->gcm, len, input, output);
  if (ret != 0) {
    Serial.printf("[Crypto] gcm_update failed: -0x%04X\n", -ret);
    return false;
  }
  return true;
}

bool crypto_gcm_finish(crypto_gcm_ctx_t *ctx, const uint8_t *expected_tag,
                       size_t tag_len) {
  if (!ctx)
    return false;

  // Compute the authentication tag
  uint8_t computed_tag[16];
  int ret = mbedtls_gcm_finish(&ctx->gcm, computed_tag, sizeof(computed_tag));

  mbedtls_gcm_free(&ctx->gcm);
  free(ctx);

  if (ret != 0) {
    Serial.printf("[Crypto] gcm_finish failed: -0x%04X\n", -ret);
    return false;
  }

  // Constant-time comparison of tags
  uint8_t diff = 0;
  for (size_t i = 0; i < tag_len && i < 16; i++) {
    diff |= computed_tag[i] ^ expected_tag[i];
  }

  if (diff != 0) {
    Serial.println(
        "[Crypto] GCM auth tag MISMATCH — data is corrupted or tampered");
    return false;
  }

  Serial.println("[Crypto] GCM auth tag OK");
  return true;
}
