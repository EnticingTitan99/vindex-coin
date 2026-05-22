# Skill: codebase_context

## Purpose
Provides Manus AI with pre-digested architectural context for the vindex-coin project to answer questions accurately without needing to read every file. Use this alongside `repo_navigator.md`.

---

## Project Identity

- **Name:** Vindex Coin
- **Base Protocol:** CryptoNote v2 (Monero lineage)
- **Consensus:** Proof of Work — RandomX algorithm (CPU-optimised, ASIC-resistant)
- **Privacy Model:** Ring signatures + stealth addresses + RingCT (confidential amounts)
- **Build System:** CMake ≥ 3.5; C++14 standard
- **GUI:** Electron desktop app (`electron-app/`)
- **Related Repos:**
  - `vindex-coin-research` — tokenomics, whitepaper, research notes (private)
  - `miners_fork_transfer` — cross-agent miner/fork transfer system (private, C++)

---

## Architecture Layers

```
[ Electron GUI / CLI ]  ←→  [ Wallet RPC ]  ←→  [ Daemon RPC ]
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

## Module Responsibilities (src/)

### `cryptonote_core/`
- Central orchestrator: `Blockchain`, `tx_pool`, `core`
- Transaction verification pipeline
- Block validation and acceptance
- **Key files to check:** `blockchain.h`, `cryptonote_core.h`, `tx_pool.h`

### `crypto/`
- Low-level cryptographic primitives
- RandomX PoW hash, Keccak, ed25519
- Ring signature generation and verification
- **Key files:** `crypto.h`, `hash.h`, `random.h`

### `wallet/`
- Wallet2 class (primary wallet implementation)
- Key derivation, subaddress management
- Transaction construction and signing
- **Key files:** `wallet2.h`, `api/wallet.h`

### `rpc/`
- HTTP/JSON-RPC server for daemon and wallet
- Command handlers for all external API calls
- **Key files:** `core_rpc_server_commands_defs.h` (all RPC structs)

### `p2p/`
- Levin protocol (Monero P2P)
- Peer list management, block/tx propagation
- **Key files:** `net_node.h`, `p2p_protocol_defs.h`

### `blockchain/`
- LMDB database wrapper for chain state
- Block and transaction storage
- **Key files:** `blockchain_db.h`, `lmdb/db_lmdb.h`

---

## Common Question → File Mapping

| Question Type | Go To |
|---------------|-------|
| How are transactions validated? | `src/cryptonote_core/blockchain.cpp` |
| How does mining/PoW work? | `src/cryptonote_core/cryptonote_core.cpp` + `src/crypto/` |
| How are ring signatures constructed? | `src/crypto/crypto.cpp`, `src/ringct/` |
| How does the wallet scan the chain? | `src/wallet/wallet2.cpp` |
| What RPC endpoints exist? | `src/rpc/core_rpc_server_commands_defs.h` |
| How does P2P sync work? | `src/p2p/net_node.inl` |
| What are the emission/supply params? | `src/cryptonote_config.h` (top-level src/) |
| How does the Electron UI connect? | `electron-app/src/` — look for IPC/RPC calls |
| Build flags and options? | `CMakeLists.txt` (grep for option() calls) |
| What's custom vs Monero upstream? | `README.md` + git log on modified files |

---

## Critical Config File

`src/cryptonote_config.h` — **Read this file for any question about:**
- Coin supply / emission curve
- Genesis block hash
- Network ports (mainnet/testnet/stagenet)
- Ring size requirements
- Minimum fee policy
- Block time targets

This single file answers ~40% of "how is this coin configured" questions at minimal token cost.

---

## Manus Task Execution Protocol

When given a task about this codebase:

1. **Read `Skills/repo_navigator.md`** (this session's map)
2. **Read `Skills/codebase_context.md`** (this file — architectural context)
3. **Identify** which module from the `Common Question → File Mapping` table applies
4. **Fetch header file first** (`.h`) for the relevant module
5. **Fetch `.cpp` only if** the header is insufficient to answer
6. **Answer** — do not read more files than needed
7. **If modifying code:** read only the specific function/class being changed, not the whole file

---

## Token Budget Guidelines

| Action | Estimated Cost | Guideline |
|--------|---------------|----------|
| Read a `.h` header | Low | Always preferred first |
| Read a `.cpp` impl file | High | Only if header insufficient |
| Read `cryptonote_config.h` | Very Low | Do this early — high ROI |
| Read `README.md` | Medium | Once per session |
| Read `CMakeLists.txt` | Very High | Only for build questions |
| Scan `external/` | Very High | Avoid — upstream code only |
| Read a test file | Low-Medium | High intent signal per token |
