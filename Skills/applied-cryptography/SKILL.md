---
name: applied-cryptography
description: "Applied cryptography for Monero/CryptoNote forks. Covers Ed25519, Pedersen Commitments, Bulletproofs+, CLSAG ring signatures, Diffie-Hellman stealth addressing, and Keccak hashing."
---

# Applied Cryptography

## Key Primitives in Vindex

- **Ed25519** — Elliptic curve signatures and key generation (wallet keys)
- **Pedersen Commitments** — Hides transaction amounts in RingCT
- **Bulletproofs+** — Compact zero-knowledge range proofs (non-negative amounts)
- **CLSAG Ring Signatures** — Transaction untraceability (ring of 11 decoys)
- **Diffie-Hellman** — Key exchange for stealth addressing (one-time addresses)
- **Keccak (SHA-3 variant)** — Proof-of-Work hashing and general hashing

## Relevant Source Files

- `src/crypto/` — Core Ed25519, Keccak, random generation
- `src/ringct/` — RingCT, CLSAG, Bulletproofs+
- `src/cryptonote_basic/` — Transaction structure, stealth addressing

## Post-Quantum Roadmap

NIST finalised CRYSTALS-Dilithium (ML-DSA) in 2024. Vindex hard fork v18 plans hybrid Ed25519 + Dilithium wallet keys. No existing privacy coin has implemented this — first-mover advantage.
