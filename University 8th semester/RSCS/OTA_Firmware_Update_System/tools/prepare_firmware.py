#!/usr/bin/env python3
"""
prepare_firmware.py — One-command firmware release pipeline.

Automates the full release workflow:
  1. Reads firmware version from version.h
  2. Builds firmware via PlatformIO
  3. Encrypts with AES-256-GCM (random IV)
  4. Signs  SHA-256(IV || ciphertext || version_bytes)  with ECDSA P-256
  5. Generates manifest.json at the project root (ready to commit)
  6. Generates firmware.bin.enc in tools/ (ready for GitHub Release upload)

Usage:
  cd tools/
  python prepare_firmware.py

  # Skip the build step (if you already built):
  python prepare_firmware.py --skip-build

  # Custom GitHub repo (defaults are auto-detected from version.h):
  python prepare_firmware.py --repo user/repo
"""

import argparse
import base64
import json
import os
import re
import struct
import subprocess
import sys

from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives.ciphers.aead import AESGCM
from cryptography.hazmat.primitives import hashes, serialization


# ---------------------------------------------------------------------------
# Path constants (relative to this script's directory)
# ---------------------------------------------------------------------------
SCRIPT_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.normpath(os.path.join(SCRIPT_DIR, ".."))
DEVICE_DIR = os.path.join(PROJECT_ROOT, "OTA_Target_Device")

VERSION_H = os.path.join(DEVICE_DIR, "include", "version.h")
FIRMWARE_BIN = os.path.join(
    DEVICE_DIR, ".pio", "build", "seeed_xiao_esp32c3", "firmware.bin"
)

# Default key paths (in tools/generated/)
DEFAULT_SIGNING_KEY = os.path.join(SCRIPT_DIR, "generated", "signing_key.pem")
DEFAULT_AES_KEY = os.path.join(SCRIPT_DIR, "generated", "aes_key.bin")

# Output locations
ENCRYPTED_FW_OUT = os.path.join(SCRIPT_DIR, "generated", "firmware.bin.enc")
MANIFEST_OUT = os.path.join(PROJECT_ROOT, "manifest.json")

# Default GitHub repo for firmware URL construction
DEFAULT_GITHUB_REPO = "vladislav-atanassov/Programming-in-university"


# ---------------------------------------------------------------------------
# Helpers
# ---------------------------------------------------------------------------

def read_version_from_header() -> tuple[int, int, int]:
    """Parse FIRMWARE_VERSION_MAJOR/MINOR/PATCH from version.h."""
    if not os.path.isfile(VERSION_H):
        print(f"[!] version.h not found: {VERSION_H}", file=sys.stderr)
        sys.exit(1)

    with open(VERSION_H, "r") as f:
        content = f.read()

    major = re.search(r"FIRMWARE_VERSION_MAJOR\s+(\d+)", content)
    minor = re.search(r"FIRMWARE_VERSION_MINOR\s+(\d+)", content)
    patch = re.search(r"FIRMWARE_VERSION_PATCH\s+(\d+)", content)

    if not all([major, minor, patch]):
        print("[!] Could not parse version from version.h", file=sys.stderr)
        sys.exit(1)

    return int(major.group(1)), int(minor.group(1)), int(patch.group(1))


def version_to_int(major: int, minor: int, patch: int) -> int:
    """Encode version as integer: MAJOR*10000 + MINOR*100 + PATCH."""
    return major * 10000 + minor * 100 + patch


def version_to_tag(major: int, minor: int, patch: int) -> str:
    """Format version as git tag string: v1.2.3."""
    return f"v{major}.{minor}.{patch}"


def build_firmware():
    """Run PlatformIO build."""
    print("[*] Building firmware...")

    # Try to find platformio
    pio_paths = [
        "platformio",  # system PATH
        "pio",  # common alias
        os.path.expanduser(
            "~\\.platformio\\penv\\Scripts\\platformio.exe"
        ),  # Windows default
    ]

    for pio in pio_paths:
        try:
            result = subprocess.run(
                [pio, "run"],
                cwd=DEVICE_DIR,
                capture_output=True,
                text=True,
            )
            if result.returncode == 0:
                # Extract size info from output
                for line in result.stdout.splitlines():
                    if "RAM:" in line or "Flash:" in line or "SUCCESS" in line:
                        print(f"    {line.strip()}")
                return
            else:
                print(f"[!] Build failed:\n{result.stderr}", file=sys.stderr)
                sys.exit(1)
        except FileNotFoundError:
            continue

    print("[!] PlatformIO not found. Install it or add to PATH.", file=sys.stderr)
    sys.exit(1)


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


# ---------------------------------------------------------------------------
# Main
# ---------------------------------------------------------------------------

def main():
    parser = argparse.ArgumentParser(
        description="One-command firmware release: build → encrypt → sign → manifest."
    )
    parser.add_argument("--skip-build", action="store_true",
                        help="Skip PlatformIO build (use existing firmware.bin)")
    parser.add_argument("--key", "-k", default=DEFAULT_SIGNING_KEY,
                        help="ECDSA P-256 private key PEM")
    parser.add_argument("--aes-key", "-a", default=DEFAULT_AES_KEY,
                        help="AES-256 key file (32 bytes)")
    parser.add_argument("--repo", "-r", default=DEFAULT_GITHUB_REPO,
                        help=f"GitHub repo (default: {DEFAULT_GITHUB_REPO})")
    args = parser.parse_args()

    # -----------------------------------------------------------------
    # 1. Read version from version.h
    # -----------------------------------------------------------------
    major, minor, patch = read_version_from_header()
    version_int = version_to_int(major, minor, patch)
    version_tag = version_to_tag(major, minor, patch)

    print(f"[+] Version: {version_tag} (code {version_int})")

    # Abort if version hasn't been bumped since last manifest
    if os.path.isfile(MANIFEST_OUT):
        try:
            with open(MANIFEST_OUT, "r") as f:
                old_manifest = json.load(f)
            old_version = old_manifest.get("version", 0)
            if version_int <= old_version:
                print(f"\n[!] Version not bumped! "
                      f"(version.h={version_int}, manifest={old_version})",
                      file=sys.stderr)
                print("    Devices will reject this update (anti-rollback).",
                      file=sys.stderr)
                print("    Bump the version in version.h first.",
                      file=sys.stderr)
                sys.exit(1)
        except (json.JSONDecodeError, KeyError):
            pass  # Corrupted manifest — proceed anyway

    # -----------------------------------------------------------------
    # 2. Build firmware (unless skipped)
    # -----------------------------------------------------------------
    if not args.skip_build:
        build_firmware()
    else:
        print("[*] Skipping build (--skip-build)")

    # -----------------------------------------------------------------
    # 3. Load firmware binary
    # -----------------------------------------------------------------
    if not os.path.isfile(FIRMWARE_BIN):
        print(f"[!] Firmware not found: {FIRMWARE_BIN}", file=sys.stderr)
        print("    Build first, or check the path.", file=sys.stderr)
        sys.exit(1)

    with open(FIRMWARE_BIN, "rb") as f:
        plaintext = f.read()
    print(f"[+] Firmware loaded: {len(plaintext)} bytes")

    # -----------------------------------------------------------------
    # 4. Load keys
    # -----------------------------------------------------------------
    private_key = load_private_key(args.key)
    aes_key = load_aes_key(args.aes_key)
    print("[+] Keys loaded")

    # -----------------------------------------------------------------
    # 5. Encrypt with AES-256-GCM
    # -----------------------------------------------------------------
    iv = os.urandom(12)  # Random 96-bit IV
    aesgcm = AESGCM(aes_key)
    ciphertext_with_tag = aesgcm.encrypt(iv, plaintext, None)

    ciphertext = ciphertext_with_tag[:-16]
    tag = ciphertext_with_tag[-16:]

    print(f"[+] Encrypted: {len(ciphertext)} bytes + 16 bytes tag")

    # -----------------------------------------------------------------
    # 6. Sign: SHA-256( IV || ciphertext || version_bytes_LE )
    # -----------------------------------------------------------------
    version_bytes = struct.pack("<I", version_int)
    message = iv + ciphertext + version_bytes
    signature = private_key.sign(message, ec.ECDSA(hashes.SHA256()))

    print(f"[+] Signed: {len(signature)} bytes DER signature")

    # -----------------------------------------------------------------
    # 7. Write outputs
    # -----------------------------------------------------------------
    # Encrypted firmware -> tools/firmware.bin.enc
    with open(ENCRYPTED_FW_OUT, "wb") as f:
        f.write(ciphertext)
    print(f"[+] Encrypted firmware -> {ENCRYPTED_FW_OUT}")

    # Construct firmware download URL from repo + version tag
    firmware_url = (
        f"https://github.com/{args.repo}/releases/download/"
        f"{version_tag}/firmware.bin.enc"
    )

    # Manifest -> project root/manifest.json (ready to commit & push)
    manifest = {
        "version": version_int,
        "firmware_url": firmware_url,
        "iv": base64.b64encode(iv).decode(),
        "tag": base64.b64encode(tag).decode(),
        "signature": base64.b64encode(signature).decode(),
        "size": len(ciphertext),
    }

    with open(MANIFEST_OUT, "w") as f:
        json.dump(manifest, f, indent=2)
    print(f"[+] Manifest -> {MANIFEST_OUT}")

    # -----------------------------------------------------------------
    # Summary
    # -----------------------------------------------------------------
    print()
    print("=" * 58)
    print(f"  Release {version_tag} (code {version_int}) ready!")
    print("=" * 58)
    print()
    print("  Next steps:")
    print(f"    1. git add manifest.json && git commit && git push")
    print(f"    2. Create GitHub Release '{version_tag}'")
    print(f"       and upload: tools/generated/firmware.bin.enc")
    print()
    print(f"  Firmware URL: {firmware_url}")
    print()


if __name__ == "__main__":
    main()
