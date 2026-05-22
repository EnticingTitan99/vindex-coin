---
name: blockchain-consensus-engineering
description: "Consensus rules for Vindex: block reward, emission curve, dynamic block size, hard fork management, adaptive tail emission."
---

# Blockchain Consensus Engineering

## Emission Curve

Vindex uses Monero's base emission (`EMISSION_SPEED_FACTOR_PER_MINUTE = 20`) plus an **adaptive tail emission**:

```
base_subsidy = 300000000000  (0.3 VDX)
max_subsidy  = 600000000000  (0.6 VDX)

if rolling_100_block_median_tx_count > 50:
    subsidy = max_subsidy
elif > 10:
    subsidy = base_subsidy + (tx_count * 6000000000)
else:
    subsidy = base_subsidy
```

**File to modify**: `src/cryptonote_basic/cryptonote_basic_impl.cpp` → `get_block_reward()`

## Hard Fork Management

- All v1-v16 forks activate at block 1 (genesis) — Vindex launches at full Monero v0.18 protocol parity
- Future forks defined in `src/hardforks/hardforks.cpp`
- `src/hardforks/hardforks.h` — declares the arrays

## Dynamic Block Size

- `CRYPTONOTE_BLOCK_GRANTED_FULL_REWARD_ZONE_V5 = 300000` (300 KB penalty-free)
- Long-term median window: 100,000 blocks
- Surge factor: 50x (max burst size)

## Checkpoints

- `src/checkpoints/checkpoints.cpp` — currently empty (new chain)
- Add first checkpoint ~1000 blocks after mainnet genesis
