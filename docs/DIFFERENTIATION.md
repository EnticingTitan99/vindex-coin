# Vindex — Fork Differentiation Strategy

*Libertatis Vindex: Champion of Liberty*

This document defines what sets Vindex ($VDX) apart from Monero and other privacy coins.

---

## Design Philosophy

Monero is the gold standard for privacy. Rather than competing on privacy alone, Vindex targets three underserved areas:

1. **CPU-first mining fairness** — tighter RandomX configuration improves CPU advantage
2. **Adaptive tail emission** — responds to network activity rather than fixed inflation
3. **Faster confirmations** — 60s blocks vs Monero's 120s

---

## Differentiator 1: Faster Block Time (60s)

- `DIFFICULTY_TARGET_V2 = 60` in `src/cryptonote_config.h`
- 2x faster user confirmations
- More frequent difficulty adjustments = smoother difficulty curve
- Trade-off: slightly more orphan blocks — acceptable at launch scale

---

## Differentiator 2: Tightened RandomX (CPU-First Mining)

Modify `external/randomx/src/configuration.h`:

| Parameter | Monero Default | Vindex |
|-----------|---------------|--------|
| `RANDOMX_PROGRAM_SIZE` | 256 instructions | 320 |
| `RANDOMX_PROGRAM_ITERATIONS` | 2048 | 2560 |
| `RANDOMX_PROGRAM_COUNT` | 8 | 10 |

**Effect**: More CPU compute per hash. GPUs don't benefit from extra instructions the way CPUs do (branch prediction, out-of-order execution). Net result: ~15-20% CPU hashrate advantage over Monero's baseline.

> **TODO**: Apply these changes to `external/randomx/src/configuration.h`

---

## Differentiator 3: Adaptive Tail Emission

Monero's tail emission is a fixed 0.6 XMR/block forever. Vindex's scales with activity.

```
base_subsidy = 300000000000  (0.3 VDX minimum)
max_subsidy  = 600000000000  (0.6 VDX maximum)

if (tx_count_median > 50):
    subsidy = max_subsidy
elif (tx_count_median > 10):
    subsidy = base_subsidy + (tx_count_median * 6000000000)
else:
    subsidy = base_subsidy
```

Constants `FINAL_SUBSIDY_PER_MINUTE` and `FINAL_SUBSIDY_PER_MINUTE_MAX` are set in `cryptonote_config.h`.

> **TODO**: Implement adaptive logic in `src/cryptonote_basic/cryptonote_basic_impl.cpp` → `get_block_reward()`

---

## Differentiator 4: Ring Size 11 (vs Monero 16)

- `CRYPTONOTE_DEFAULT_RING_SIZE = 11` in `cryptonote_config.h`
- On a new chain with few outputs, ring size 16 draws from a tiny anonymity set anyway
- Plan: hard fork v19 to ring size 16 once chain has >100k outputs

---

## Differentiator 5: Unique Identity

| Parameter | Value |
|-----------|-------|
| Coin name | **Vindex** |
| Ticker | **$VDX** |
| Tagline | *Libertatis Vindex* (Champion of Liberty) |
| Address start | `V` (Base58 prefix 20) |
| P2P port | 28080 |
| RPC port | 28081 |
| Network ID | `VDXNET LIBERTATI` (ASCII-encoded 16 bytes) |
| Genesis nonce | 10001 |

---

## Differentiator 6: Post-Quantum Roadmap

No major privacy coin has implemented post-quantum signatures. NIST finalised CRYSTALS-Dilithium (ML-DSA) in 2024.

| Version | Change | Status |
|---------|--------|--------|
| v1-16 | Full Monero v0.18 parity from genesis | ✅ Done |
| v17 | Ring size 16 (when >100k outputs) | Planned |
| v18 | Hybrid Ed25519 + CRYSTALS-Dilithium wallet keys | Planned |
| v19 | Full ML-DSA + Bulletproofs+ | Future |

---

## What We Are NOT Changing

- **RingCT** — battle-tested, keeping as-is
- **Bulletproofs+** — best-in-class range proofs
- **Dandelion++** — transaction privacy relay
- **LMDB** — database backend
- **CLSAG** — ring signature scheme
