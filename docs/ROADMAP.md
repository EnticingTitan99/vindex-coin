# Vindex Development Roadmap

*Libertatis Vindex — Champion of Liberty*

---

## Phase 1: Identity & Genesis (NOW — Immediate)

| Task | Status | Notes |
|------|--------|-------|
| Fork codebase from Monero v0.18 | ✅ Done | `vindex-coin` repo |
| Name & branding finalised ($VDX) | ✅ Done | See `vindex-coin-research` |
| Unique Network ID | ✅ Done | `VDXNET LIBERTATI` |
| Unique ports (28080/28081/28082) | ✅ Done | Distinct from Monero 18080 |
| Unique address prefix (20 → `V`) | ✅ Done | |
| Block time 60s | ✅ Done | `DIFFICULTY_TARGET_V2 = 60` |
| Ring size default 11 | ✅ Done | |
| Hardforks reset from genesis | ✅ Done | All v1-16 active at block 1 |
| Checkpoints cleared | ✅ Done | Monero checkpoints removed |
| **Generate genesis block (Pass 1)** | 🔴 NEXT | Run `./vindexd --print-genesis-tx` |
| **Embed genesis TX (Pass 2)** | 🔴 BLOCKED | Blocked on Pass 1 |
| Verify daemon starts cleanly | ⬜ Pending | |

## Phase 2: Differentiation (After Genesis)

| Task | Status | Notes |
|------|--------|-------|
| Apply RandomX tightening | ⬜ Pending | `external/randomx/src/configuration.h` |
| Implement adaptive tail emission | ⬜ Pending | `cryptonote_basic_impl.cpp` |
| Update binary/daemon names throughout | ⬜ Pending | Ensure all refs say `vindexd` not `monerod` |
| Update FUNDING.yml donation addresses | ⬜ Pending | Replace Monero addresses |

## Phase 3: Infrastructure (After Testnet)

| Task | Status | Notes |
|------|--------|-------|
| Seed node deployment (Ubuntu 22.04 VPS) | ⬜ Pending | |
| Mining pool (nodejs-pool adaptation) | ⬜ Pending | |
| Block explorer (onion-monero fork) | ⬜ Pending | |
| CLI wallet rebranding | ⬜ Pending | |
| GUI wallet fork (monero-gui) | ⬜ Pending | |

## Phase 4: Launch

| Task | Status | Notes |
|------|--------|-------|
| Whitepaper / one-pager | ⬜ Pending | |
| Website getvindex.org | ⬜ Pending | |
| Announce on BitcoinTalk/Reddit | ⬜ Pending | |
| First block explorer public | ⬜ Pending | |

## Phase 5: Post-Quantum Upgrades

| Hard Fork | Change | ETA |
|-----------|--------|-----|
| v17 | Ring size 16 | ~100k blocks |
| v18 | Hybrid Ed25519 + Dilithium | TBD |
| v19 | Full ML-DSA | TBD |
