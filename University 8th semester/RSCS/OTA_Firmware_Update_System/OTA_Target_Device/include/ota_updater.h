#ifndef OTA_UPDATER_H
#define OTA_UPDATER_H

#include "manifest.h"

/**
 * Perform the full OTA update pipeline:
 *
 *   1. Check version (anti-rollback)
 *   2. Download encrypted firmware (pass 1) → compute hash
 *   3. Verify ECDSA-P256 signature over SHA-256(IV + ciphertext + version)
 *   4. Download encrypted firmware (pass 2) → decrypt + flash to OTA partition
 *   5. Verify GCM authentication tag
 *   6. Activate new partition and reboot
 *
 * @param manifest  Parsed firmware manifest
 * @return true if update was installed (device will reboot)
 */
bool ota_perform_update(const FirmwareManifest &manifest);

/**
 * Mark the currently running firmware as valid.
 * Must be called once after a successful boot to prevent automatic rollback.
 */
void ota_mark_valid();

#endif // OTA_UPDATER_H
