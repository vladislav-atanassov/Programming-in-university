# Secure Encrypted OTA Firmware Update System (ESP32)

A secure Over-The-Air firmware update system for ESP32-C3. The device downloads encrypted firmware from GitHub Releases, verifies its ECDSA-P256 digital signature, decrypts it with AES-256-GCM, and installs it using dual A/B OTA partitions with automatic rollback.

## Architecture

```
┌─────────────────────────────────────────────────┐
│                  SERVER (Python)                │
│                                                 │
│  firmware.bin                                   │
│       │                                         │
│       ▼                                         │
│  AES-256-GCM encrypt (random IV)                │
│       │                                         │
│       ▼                                         │
│  ECDSA-P256 sign (IV + ciphertext + version)    │
│       │                                         │
│       ▼                                         │
│  Upload to GitHub Releases                      │
│  (firmware.bin.enc + manifest.json)             │
└───────────────────────┬─────────────────────────┘
                        │ HTTPS
┌───────────────────────▼─────────────────────────┐
│                  ESP32-C3                       │
│                                                 │
│  Download manifest.json                         │
│       │                                         │
│       ▼                                         │
│  Check version (anti-rollback)                  │
│       │                                         │
│       ▼                                         │
│  Download firmware.bin.enc                      │
│       │                                         │
│       ▼                                         │
│  Verify ECDSA-P256 signature                    │
│       │                                         │
│       ▼                                         │
│  Decrypt AES-256-GCM                            │
│       │                                         │
│       ▼                                         │
│  Install via OTA API (inactive partition)       │
│       │                                         │
│       ▼                                         │
│  Reboot                                         │
└─────────────────────────────────────────────────┘
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
├── OTA_Target_Device/            # ESP32 firmware (PlatformIO)
│   ├── include/
│   │   ├── config.example.h      # Template (copy to config.h)
│   │   ├── config.h              # WiFi + crypto keys (gitignored)
│   │   ├── version.h             # Firmware version
│   │   ├── wifi_manager.h
│   │   ├── crypto.h              # Streaming ECDSA + AES-GCM
│   │   ├── manifest.h
│   │   └── ota_updater.h
│   ├── src/
│   │   ├── main.cpp
│   │   ├── wifi_manager.cpp
│   │   ├── crypto.cpp
│   │   ├── manifest.cpp
│   │   └── ota_updater.cpp
│   ├── partitions.csv            # Dual OTA partition layout
│   └── platformio.ini
├── tools/                        # Server-side Python scripts
│   ├── generate_keys.py          # Generate ECDSA + AES keys
│   ├── prepare_firmware.py       # Encrypt + sign + manifest
│   └── requirements.txt
└── README.md
```

## Quick Start

### 1. Generate Cryptographic Keys

```bash
cd tools
pip install -r requirements.txt
python generate_keys.py
```

This creates `signing_key.pem`, `public_key.pem`, and `aes_key.bin`, and prints C byte arrays to paste into `config.h`.

### 2. Configure the ESP32

```bash
cd OTA_Target_Device
copy include\config.example.h include\config.h
```

Edit `include/config.h`:
- Set `WIFI_SSID` and `WIFI_PASSWORD`
- Set `MANIFEST_URL` to your GitHub-hosted manifest URL
- Paste the AES key and ECDSA public key arrays from step 1

### 3. Build and Flash

```bash
C:\Users\VladislavAtanassov\.platformio\penv\Scripts\platformio.exe run
C:\Users\VladislavAtanassov\.platformio\penv\Scripts\platformio.exe run --target upload
C:\Users\VladislavAtanassov\.platformio\penv\Scripts\platformio.exe device monitor
```

### 4. Publish an OTA Update

After making firmware changes, bump the version in `version.h`, rebuild, then:

```bash
cd tools
python prepare_firmware.py --version 10100 \
    --firmware-url https://github.com/USER/REPO/releases/download/v1.1.0/firmware.bin.enc
```

Upload `firmware.bin.enc` and `manifest.json` to a GitHub Release (or host `manifest.json` on GitHub Pages).

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

## OTA Update Flow (Detailed)

1. **Manifest fetch** — ESP32 fetches `manifest.json` over HTTPS
2. **Version check** — Compares `manifest.version` against `FIRMWARE_VERSION`. Skips if not newer (anti-rollback).
3. **Pass 1 (signature verification)** — Streams the encrypted firmware while computing `SHA-256(IV || ciphertext || version_bytes)`. Verifies the ECDSA-P256 signature. **Aborts if invalid** — nothing is written to flash.
4. **Pass 2 (decrypt + flash)** — Re-downloads the firmware, decrypts each chunk with AES-256-GCM, and writes to the inactive OTA partition.
5. **GCM tag check** — Verifies the authentication tag. Aborts without activating if mismatched.
6. **Activate + reboot** — Sets the new partition as the boot target and reboots.
7. **Self-validation** — On first boot, new firmware calls `esp_ota_mark_app_valid_cancel_rollback()`. If it crashes before this call, the bootloader automatically rolls back.
