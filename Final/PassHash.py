import hashlib
import secrets

# ── Simulated user registration ──────────────────────
username = "lia"
password = "mySecurePassword123"

# Generate random nonce/salt (256-bit)
nonce = secrets.token_hex(32)

# Hash: username + password + nonce
stored_hash = hashlib.sha256(
    (username + password + nonce).encode()
).hexdigest()

print("=== STORED ACCOUNT DATA ===")
print(f"Username: {username}")
print(f"Nonce:    {nonce}")
print(f"Hash:     {stored_hash}")

# ── Login attempt ─────────────────────────────────────
print("\n=== LOGIN ATTEMPT ===")
attempt = input("Enter password: ")

attempt_hash = hashlib.sha256(
    (username + attempt + nonce).encode()
).hexdigest()

if attempt_hash == stored_hash:
    print("→ Access GRANTED  (hashes matched)")
else:
    print("→ Access DENIED   (hashes did not match)")

# ── Bonus: show avalanche effect ─────────────────────
print("\n=== AVALANCHE EFFECT DEMO ===")
h1 = hashlib.sha256(b"password123").hexdigest()
h2 = hashlib.sha256(b"password124").hexdigest()
print(f"password123 → {h1}")
print(f"password124 → {h2}")
print("(One character change, completely different hash)")