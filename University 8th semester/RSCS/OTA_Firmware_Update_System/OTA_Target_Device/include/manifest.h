#ifndef MANIFEST_H
#define MANIFEST_H

#include <Arduino.h>
#include <stddef.h>
#include <stdint.h>


// Maximum sizes for decoded binary fields
#define MANIFEST_IV_LEN 12
#define MANIFEST_TAG_LEN 16
#define MANIFEST_MAX_SIG_LEN 72 // DER-encoded ECDSA-P256 is ≤72 bytes

struct FirmwareManifest {
  uint32_t version;                        // Encoded version integer
  String firmware_url;                     // HTTPS download URL
  uint8_t iv[MANIFEST_IV_LEN];             // AES-GCM initialization vector
  uint8_t tag[MANIFEST_TAG_LEN];           // AES-GCM authentication tag
  uint8_t signature[MANIFEST_MAX_SIG_LEN]; // ECDSA-P256 DER signature
  size_t signature_len;                    // Actual signature length
  size_t firmware_size;                    // Encrypted firmware size in bytes
};

/**
 * Fetch the manifest JSON over HTTPS and parse it.
 * Base64-encoded fields (iv, tag, signature) are decoded into binary.
 *
 * @param url       HTTPS URL of the manifest JSON
 * @param out       Parsed manifest output
 * @return true on success
 */
bool manifest_fetch_and_parse(const char *url, FirmwareManifest &out);

#endif // MANIFEST_H
