# Secure Encrypted OTA Firmware Update System (ESP32)

A secure Over-The-Air firmware update system for ESP32-C3. The device downloads encrypted firmware from GitHub Releases, verifies its ECDSA-P256 digital signature, decrypts it with AES-256-GCM, and installs it using dual A/B OTA partitions with automatic rollback.

## Architecture

```
┌───────────────────────────────────────────────┐
│                 SERVER (Python)               │
│                                               │
│                  firmware.bin                 │
│                      │                        │
│                      ▼                        │
│         AES-256-GCM encrypt (random IV)       │
│                      │                        │
│                      ▼                        │
│  ECDSA-P256 sign (IV + ciphertext + version)  │
│                      │                        │
│                      ▼                        │
│           Upload to GitHub Releases           │
│      (firmware.bin.enc + manifest.json)       │
└──────────────────────┬────────────────────────┘
                       │ HTTPS
┌──────────────────────▼────────────────────────┐
│                   ESP32-C3                    │
│                                               │
│            Download manifest.json             │
│                      │                        │
│                      ▼                        │
│         Check version (anti-rollback)         │
│                      │                        │
│                      ▼                        │
│            Download firmware.bin.enc          │
│                      │                        │
│                      ▼                        │
│          Verify ECDSA-P256 signature          │
│                      │                        │
│                      ▼                        │
│             Decrypt AES-256-GCM               │
│                      │                        │
│                      ▼                        │
│   Install via OTA API (inactive partition)    │
│                      │                        │
│                      ▼                        │
│                   Reboot                      │
└───────────────────────────────────────────────┘
```

## Security Features

| Feature | Implementation |
|---|---|
| Firmware encryption | AES-256-GCM (authenticated encryption) |
| Digital signatures | ECDSA P-256 over SHA-256 |
| Per-update uniqueness | Random IV per firmware build |
| Anti-rollback | Integer version comparison |
| Root of trust | Public key + AES key embedded in firmware |
| Integrity verification | GCM authentication tag |

## Reliability Features

| Feature | Implementation |
|---|---|
| Dual OTA partitions | A/B scheme via custom `partitions.csv` |
| Automatic rollback | ESP-IDF bootloader reverts on crash |
| Signature-first verification | Two-pass: verify signature before flashing |
| Safe abort | Never activates partition if any check fails |

## Project Structure

```
OTA_Firmware_Update_System/
├── OTA_Target_Device/              # ESP32 firmware (PlatformIO)
│   ├── include/
│   │   ├── config.example.h        # Template (copy to config.h)
│   │   ├── config.h                # WiFi + crypto keys (gitignored)
│   │   ├── version.h               # Firmware version
│   │   ├── wifi_manager.h
│   │   ├── crypto.h                # Streaming ECDSA + AES-GCM
│   │   ├── manifest.h
│   │   └── ota_updater.h
│   ├── src/
│   │   ├── main.cpp
│   │   ├── wifi_manager.cpp
│   │   ├── crypto.cpp
│   │   ├── manifest.cpp
│   │   └── ota_updater.cpp
│   ├── partitions.csv              # Dual OTA partition layout
│   └── platformio.ini
├── tools/                          # Server-side Python scripts
│   ├── generated/                  # Keys + build artifacts (gitignored)
│   │   ├── .gitkeep                # Keeps directory in repo
│   │   ├── signing_key.pem         # ECDSA private key (secret)
│   │   ├── aes_key.bin             # AES-256 key (secret)
│   │   ├── public_key.pem          # ECDSA public key
│   │   └── firmware.bin.enc        # Encrypted firmware (for upload)
│   ├── generate_keys.py            # Generate ECDSA + AES keys
│   ├── prepare_firmware.py         # Build + encrypt + sign + manifest
│   └── requirements.txt
├── manifest.json                   # OTA manifest (committed to repo)
└── README.md
```

## Quick Start

### 1. Install Python Dependencies

```bash
cd tools
pip install -r requirements.txt
```

### 2. Generate Cryptographic Keys (one-time)

```bash
python generate_keys.py
```

This creates keys in `tools/generated/` and prints C byte arrays to paste into `config.h`.

> [!CAUTION]
> Never commit `signing_key.pem` or `aes_key.bin` to version control. The `tools/generated/` directory is gitignored for this reason.

### 3. Configure the ESP32

```bash
cd OTA_Target_Device
copy include\config.example.h include\config.h
```

Edit `include/config.h`:
- Set `WIFI_SSID` and `WIFI_PASSWORD`
- Set `MANIFEST_URL` to your GitHub-hosted manifest URL
- Paste the AES key and ECDSA public key arrays from step 2

### 4. Build and Flash (initial)

```bash
pio run --target upload
pio device monitor
```

### 5. Publish an OTA Update

> [!NOTE]
> You must bump the version in `include/version.h` before preparing a release. The script will abort if the version hasn't changed.

After making firmware changes, bump the version in `include/version.h`, then run a single command:

```bash
cd tools
python prepare_firmware.py
```

This automatically:
1. Reads the version from `version.h`
2. **Aborts if the version hasn't been bumped** (devices would reject a same-or-older version)
3. Builds the firmware via PlatformIO
4. Encrypts with AES-256-GCM
5. Signs with ECDSA-P256
6. Outputs `firmware.bin.enc` to `tools/generated/`
7. Outputs `manifest.json` to the project root

Then publish to GitHub:
1. Commit and push `manifest.json`
2. Create a GitHub Release (e.g. `v1.2.0`) and upload `tools/generated/firmware.bin.enc`

> [!IMPORTANT]
> `manifest.json` and `firmware.bin.enc` are a matched pair — they must come from the same run of `prepare_firmware.py`.

Options:
- `--skip-build` — skip the PlatformIO build step (use existing binary)
- `--repo user/repo` — override the GitHub repository for the download URL

## Manifest Format

```json
{
  "version": 10100,
  "firmware_url": "https://github.com/.../firmware.bin.enc",
  "iv": "<base64 12 bytes>",
  "tag": "<base64 16 bytes>",
  "signature": "<base64 ECDSA-P256 DER>",
  "size": 123456
}
```

## OTA Update Flow

1. **Manifest fetch** — ESP32 fetches `manifest.json` over HTTPS every 60 seconds
2. **Version check** — Compares `manifest.version` against `FIRMWARE_VERSION`. Skips if not newer (anti-rollback).
3. **Pass 1 (signature verification)** — Streams the encrypted firmware while computing `SHA-256(IV || ciphertext || version_bytes)`. Verifies the ECDSA-P256 signature. **Aborts if invalid** — nothing is written to flash.
4. **Pass 2 (decrypt + flash)** — Re-downloads the firmware, decrypts each chunk with AES-256-GCM, and writes to the inactive OTA partition.
5. **GCM tag check** — Verifies the authentication tag. Aborts without activating if mismatched.
6. **Activate + reboot** — Sets the new partition as the boot target and reboots.
7. **Self-validation** — On first boot, new firmware calls `esp_ota_mark_app_valid_cancel_rollback()`. If it crashes before this call, the bootloader automatically rolls back.

> [!NOTE]
> Power loss during an update is safe — the boot partition only switches after all verification passes. The device will reboot into the previous firmware.

## Version Encoding

Versions are encoded as integers for comparison: `MAJOR * 10000 + MINOR * 100 + PATCH`

| Version | Code |
|---------|------|
| v1.0.0 | 10000 |
| v1.1.0 | 10100 |
| v1.2.3 | 10203 |
| v2.0.0 | 20000 |
