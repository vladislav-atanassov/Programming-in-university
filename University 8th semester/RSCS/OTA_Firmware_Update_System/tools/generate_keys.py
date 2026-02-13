#!/usr/bin/env python3
"""
generate_keys.py — Generate cryptographic keys for the OTA update system.

Creates:
  - signing_key.pem   : ECDSA P-256 private key (keep secret!)
  - public_key.pem    : ECDSA P-256 public key
  - aes_key.bin       : Random 256-bit AES key

Also prints C byte arrays ready to paste into config.h.
"""

import os
import sys
from cryptography.hazmat.primitives.asymmetric import ec
from cryptography.hazmat.primitives import serialization


def main():
    out_dir = os.path.dirname(os.path.abspath(__file__))

    # -----------------------------------------------------------------
    # 1. Generate ECDSA P-256 key pair
    # -----------------------------------------------------------------
    private_key = ec.generate_private_key(ec.SECP256R1())

    # Save private key (PEM)
    priv_path = os.path.join(out_dir, "signing_key.pem")
    with open(priv_path, "wb") as f:
        f.write(private_key.private_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PrivateFormat.PKCS8,
            encryption_algorithm=serialization.NoEncryption()
        ))
    print(f"[+] Private key saved:  {priv_path}")

    # Save public key (PEM)
    pub_key = private_key.public_key()
    pub_path = os.path.join(out_dir, "public_key.pem")
    with open(pub_path, "wb") as f:
        f.write(pub_key.public_bytes(
            encoding=serialization.Encoding.PEM,
            format=serialization.PublicFormat.SubjectPublicKeyInfo
        ))
    print(f"[+] Public key saved:   {pub_path}")

    # Public key in DER format for embedding
    pub_der = pub_key.public_bytes(
        encoding=serialization.Encoding.DER,
        format=serialization.PublicFormat.SubjectPublicKeyInfo
    )

    # -----------------------------------------------------------------
    # 2. Generate AES-256 key
    # -----------------------------------------------------------------
    aes_key = os.urandom(32)
    aes_path = os.path.join(out_dir, "aes_key.bin")
    with open(aes_path, "wb") as f:
        f.write(aes_key)
    print(f"[+] AES-256 key saved:  {aes_path}")

    # -----------------------------------------------------------------
    # 3. Print C byte arrays for config.h
    # -----------------------------------------------------------------
    print()
    print("=" * 60)
    print("  Paste the following into include/config.h")
    print("=" * 60)
    print()

    # AES key
    print("// AES-256 symmetric key (32 bytes)")
    print("static const uint8_t AES_KEY[32] = {")
    for i in range(0, 32, 8):
        chunk = aes_key[i:i+8]
        line = "    " + ", ".join(f"0x{b:02X}" for b in chunk)
        if i + 8 < 32:
            line += ","
        print(line)
    print("};")
    print()

    # ECDSA public key (DER)
    print(f"// ECDSA P-256 public key in DER format ({len(pub_der)} bytes)")
    print("static const uint8_t ECDSA_PUBLIC_KEY[] = {")
    for i in range(0, len(pub_der), 8):
        chunk = pub_der[i:i+8]
        line = "    " + ", ".join(f"0x{b:02X}" for b in chunk)
        if i + 8 < len(pub_der):
            line += ","
        print(line)
    print("};")
    print(f"static const size_t ECDSA_PUBLIC_KEY_LEN = sizeof(ECDSA_PUBLIC_KEY);")

    print()
    print("[+] Done. Remember to update config.h with the keys above!")


if __name__ == "__main__":
    main()
