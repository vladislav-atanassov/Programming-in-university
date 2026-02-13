#ifndef CRYPTO_H
#define CRYPTO_H

#include <stddef.h>
#include <stdint.h>


// ---------------------------------------------------------------------------
// Streaming SHA-256 + ECDSA-P256 signature verification
// ---------------------------------------------------------------------------
// Usage:
//   crypto_hash_ctx_t *ctx = crypto_hash_begin();
//   crypto_hash_update(ctx, iv,  iv_len);
//   crypto_hash_update(ctx, ciphertext_chunk, chunk_len);  // repeat per chunk
//   crypto_hash_update(ctx, version_bytes, 4);
//   bool ok = crypto_hash_verify_signature(ctx, signature, sig_len);
//   // ctx is freed automatically after verify
// ---------------------------------------------------------------------------

typedef struct crypto_hash_ctx crypto_hash_ctx_t;

crypto_hash_ctx_t *crypto_hash_begin();
void crypto_hash_update(crypto_hash_ctx_t *ctx, const uint8_t *data,
                        size_t len);
bool crypto_hash_verify_signature(crypto_hash_ctx_t *ctx,
                                  const uint8_t *signature, size_t sig_len);

// ---------------------------------------------------------------------------
// Streaming AES-256-GCM decryption
// ---------------------------------------------------------------------------
// Usage:
//   crypto_gcm_ctx_t *ctx = crypto_gcm_begin(iv, 12);
//   crypto_gcm_decrypt(ctx, chunk_in, chunk_len, chunk_out);  // repeat
//   bool ok = crypto_gcm_finish(ctx, tag, 16);
//   // ctx is freed automatically after finish
// ---------------------------------------------------------------------------

typedef struct crypto_gcm_ctx crypto_gcm_ctx_t;

crypto_gcm_ctx_t *crypto_gcm_begin(const uint8_t *iv, size_t iv_len);
bool crypto_gcm_decrypt(crypto_gcm_ctx_t *ctx, const uint8_t *input, size_t len,
                        uint8_t *output);
bool crypto_gcm_finish(crypto_gcm_ctx_t *ctx, const uint8_t *expected_tag,
                       size_t tag_len);

#endif // CRYPTO_H
