---
name: codebase-context
description: "Pre-digested architecture and module map for vindex-coin. Use alongside repo-navigator to answer questions accurately with minimum file fetching."
---

# Codebase Context — vindex-coin

## Project Identity

- **Name:** Vindex Coin
- **Base Protocol:** CryptoNote v2 (Monero lineage)
- **Consensus:** Proof of Work — RandomX (CPU-optimised, ASIC-resistant)
- **Privacy:** Ring signatures + stealth addresses + RingCT
- **Build:** CMake ≥ 3.5, C++14
- **GUI:** Electron desktop app (`electron-app/`)

---

## Architecture Layers

```
[ Electron GUI / CLI ]  ↔  [ Wallet RPC ]  ↔  [ Daemon RPC ]
                                                      ↓
                                          [ cryptonote_core ]
                                              /          \
                                    [ blockchain ]    [ tx_pool ]
                                          ↓
                                    [ crypto layer ]
                                (ring sigs, hashing)
                                          ↓
                                    [ p2p layer ]
                                (peer sync, propagation)
```

---

## Module Responsibilities

### `cryptonote_core/`
Central orchestrator: `Blockchain`, `tx_pool`, `core`. Transaction verification pipeline and block validation.
**Key files:** `blockchain.h`, `cryptonote_core.h`, `tx_pool.h`

### `crypto/`
Low-level primitives: RandomX PoW hash, Keccak, ed25519, ring signature generation/verification.
**Key files:** `crypto.h`, `hash.h`, `random.h`

### `wallet/`
Wallet2 class — key derivation, subaddress management, tx construction and signing.
**Key files:** `wallet2.h`, `api/wallet.h`

### `rpc/`
HTTP/JSON-RPC server for daemon and wallet, all external API command handlers.
**Key files:** `core_rpc_server_commands_defs.h` (all RPC structs defined here)

### `p2p/`
Levin protocol, peer list management, block/tx propagation.
**Key files:** `net_node.h`, `p2p_protocol_defs.h`

### `blockchain/`
LMDB database wrapper for chain state, block and transaction storage.
**Key files:** `blockchain_db.h`, `lmdb/db_lmdb.h`

---

## Question → File Mapping

| Question Type | Go To |
|---------------|-------|
| Transaction validation | `src/cryptonote_core/blockchain.cpp` |
| Mining / PoW | `src/cryptonote_core/cryptonote_core.cpp` + `src/crypto/` |
| Ring signatures | `src/crypto/crypto.cpp`, `src/ringct/` |
| Wallet chain scanning | `src/wallet/wallet2.cpp` |
| RPC endpoints | `src/rpc/core_rpc_server_commands_defs.h` |
| P2P sync | `src/p2p/net_node.inl` |
| Emission / supply params | `src/cryptonote_config.h` |
| Electron GUI ↔ daemon | `electron-app/src/` — look for IPC/RPC calls |
| Build flags | `CMakeLists.txt` — grep for `option()` calls |
| Custom vs Monero upstream | `README.md` + `git log` on modified files |

---

## Critical Config File

`src/cryptonote_config.h` — read this for any question about coin supply, genesis block hash, network ports, ring size requirements, minimum fee policy, or block time targets. This single file answers ~40% of configuration questions at minimal token cost.

---

## Manus Task Protocol

1. Read `Skills/repo-navigator/SKILL.md` — session map
2. Read `Skills/codebase-context/SKILL.md` — this file
3. Identify the relevant module from the Question → File Mapping table above
4. Fetch the `.h` header file first
5. Fetch `.cpp` only if the header is insufficient
6. Answer — do not read more files than needed
7. When modifying code: read only the specific function/class being changed

---

## Token Budget Guidelines

| Action | Cost | Guideline |
|--------|------|-----------|
| Read a `.h` header | Low | Always preferred first |
| Read `cryptonote_config.h` | Very Low | High ROI — do this early |
| Read a test file | Low-Medium | High intent signal per token |
| Read `README.md` | Medium | Once per session |
| Read a `.cpp` impl file | High | Only if header insufficient |
| Read `CMakeLists.txt` | Very High | Only for build questions |
| Scan `external/` | Very High | Avoid — upstream code only |
