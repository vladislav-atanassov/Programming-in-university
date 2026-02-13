#!/usr/bin/env python3
"""
prepare_firmware.py — Encrypt, sign, and package firmware for OTA distribution.

Workflow:
  1. Extracts firmware.bin from PlatformIO build output (or uses --input)
  2. Encrypts with AES-256-GCM (random IV)
  3. Signs  SHA-256(IV || ciphertext || version_bytes)  with ECDSA P-256
  4. Generates manifest.json with all fields base64-encoded

Outputs (in --output-dir):
  - firmware.bin.enc   : Encrypted firmware
  - manifest.json      : Manifest for the ESP32 to fetch

Usage:
  python prepare_firmware.py --version 10100 \\
      --key signing_key.pem --aes-key aes_key.bin \\
      --firmware-url https://github.com/.../firmware.bin.enc
"""

import argparse
import base64
import hashlib
import json
import os
import struct
import sys

from cryptography.hazmat.primitives.asymmetric import ec, utils
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.primitives import hashes, serialization


# Default PlatformIO build output path (relative to project root)
DEFAULT_FIRMWARE_PATH = os.path.join(
    "..", "OTA_Target_Device", ".pio", "build",
    "seeed_xiao_esp32c3", "firmware.bin"
)


def load_private_key(path: str):
    with open(path, "rb") as f:
        return serialization.load_pem_private_key(f.read(), password=None)


def load_aes_key(path: str) -> bytes:
    with open(path, "rb") as f:
        key = f.read()
    if len(key) != 32:
        print(f"[!] AES key must be 32 bytes, got {len(key)}", file=sys.stderr)
        sys.exit(1)
    return key


def main():
    parser = argparse.ArgumentParser(
        description="Encrypt, sign, and package firmware for OTA."
    )
    parser.add_argument("--input", "-i",
                        help="Path to raw firmware.bin "
                             "(default: auto-detect from PlatformIO build)")
    parser.add_argument("--version", "-v", type=int, required=True,
                        help="Firmware version as integer "
                             "(e.g. 10200 for v1.2.0)")
    parser.add_argument("--key", "-k", default="signing_key.pem",
                        help="Path to ECDSA P-256 private key PEM")
    parser.add_argument("--aes-key", "-a", default="aes_key.bin",
                        help="Path to AES-256 key file (32 bytes)")
    parser.add_argument("--firmware-url", "-u", required=True,
                        help="Public HTTPS URL where firmware.bin.enc "
                             "will be hosted")
    parser.add_argument("--output-dir", "-o", default=".",
                        help="Directory for output files")
    args = parser.parse_args()

    # -----------------------------------------------------------------
    # 1. Locate firmware binary
    # -----------------------------------------------------------------
    if args.input:
        fw_path = args.input
    else:
        # Auto-detect from PlatformIO build output
        script_dir = os.path.dirname(os.path.abspath(__file__))
        fw_path = os.path.join(script_dir, DEFAULT_FIRMWARE_PATH)
        fw_path = os.path.normpath(fw_path)

    if not os.path.isfile(fw_path):
        print(f"[!] Firmware binary not found: {fw_path}", file=sys.stderr)
        print("    Build with PlatformIO first, or specify --input",
              file=sys.stderr)
        sys.exit(1)

    with open(fw_path, "rb") as f:
        plaintext = f.read()
    print(f"[+] Loaded firmware: {fw_path} ({len(plaintext)} bytes)")

    # -----------------------------------------------------------------
    # 2. Load keys
    # -----------------------------------------------------------------
    private_key = load_private_key(args.key)
    aes_key = load_aes_key(args.aes_key)
    print("[+] Keys loaded")

    # -----------------------------------------------------------------
    # 3. Encrypt with AES-256-GCM
    # -----------------------------------------------------------------
    iv = os.urandom(12)     # Random 96-bit IV
    aesgcm = AESGCM(aes_key)
    ciphertext_with_tag = aesgcm.encrypt(iv, plaintext, None)

    # AESGCM.encrypt() appends the 16-byte tag to the ciphertext
    ciphertext = ciphertext_with_tag[:-16]
    tag = ciphertext_with_tag[-16:]

    print(f"[+] Encrypted: {len(ciphertext)} bytes ciphertext + 16 bytes tag")

    # -----------------------------------------------------------------
    # 4. Sign:  SHA-256( IV || ciphertext || version_bytes_LE )
    # -----------------------------------------------------------------
    version_bytes = struct.pack("<I", args.version)   # little-endian uint32

    message = iv + ciphertext + version_bytes
    signature = private_key.sign(message, ec.ECDSA(hashes.SHA256()))

    print(f"[+] Signed ({len(signature)} bytes DER signature)")

    # -----------------------------------------------------------------
    # 5. Write outputs
    # -----------------------------------------------------------------
    os.makedirs(args.output_dir, exist_ok=True)

    enc_path = os.path.join(args.output_dir, "firmware.bin.enc")
    with open(enc_path, "wb") as f:
        f.write(ciphertext)
    print(f"[+] Encrypted firmware: {enc_path}")

    manifest = {
        "version":      args.version,
        "firmware_url":  args.firmware_url,
        "iv":           base64.b64encode(iv).decode(),
        "tag":          base64.b64encode(tag).decode(),
        "signature":    base64.b64encode(signature).decode(),
        "size":         len(ciphertext),
    }

    manifest_path = os.path.join(args.output_dir, "manifest.json")
    with open(manifest_path, "w") as f:
        json.dump(manifest, f, indent=2)
    print(f"[+] Manifest: {manifest_path}")

    # Print summary
    print()
    print("=" * 50)
    print("  Upload these to GitHub Releases:")
    print(f"    1. {enc_path}")
    print(f"    2. {manifest_path}")
    print("=" * 50)


if __name__ == "__main__":
    main()
