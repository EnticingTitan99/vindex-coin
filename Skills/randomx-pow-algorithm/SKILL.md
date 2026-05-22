---
name: randomx-pow-algorithm
description: "RandomX Proof-of-Work configuration for Vindex. Covers tightening parameters for CPU-first mining advantage."
---

# RandomX Proof-of-Work Algorithm

## Vindex Changes (TODO: apply to external/randomx/src/configuration.h)

| Parameter | Monero Default | Vindex Target | File |
|-----------|---------------|---------------|------|
| `RANDOMX_PROGRAM_SIZE` | 256 | 320 | `external/randomx/src/configuration.h` |
| `RANDOMX_PROGRAM_ITERATIONS` | 2048 | 2560 | `external/randomx/src/configuration.h` |
| `RANDOMX_PROGRAM_COUNT` | 8 | 10 | `external/randomx/src/configuration.h` |

## Effect

More CPU compute per hash. GPUs don't benefit from extra integer instructions the way CPUs do (branch prediction, out-of-order execution). Estimated ~15-20% CPU hashrate advantage over vanilla Monero miners.

## Merge-Mining Prevention

Altering these constants means Vindex's PoW hash is incompatible with Monero's — trivial merge mining is prevented.

## Epoch Configuration

- Vindex block time: 60s (Monero: 120s)
- RandomX seed height interval may need adjustment: `SEEDHASH_EPOCH_BLOCKS = 2048` (in blocks)
- At 60s/block, epoch = 2048 * 60 = ~34 hours (Monero: ~68 hours) — acceptable
