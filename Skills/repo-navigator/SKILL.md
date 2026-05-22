---
name: repo-navigator
description: "Credit-minimal orientation map for the vindex-coin codebase. Read this first every session to navigate the repo without bulk-fetching files."
---

# Repo Navigator — vindex-coin

## Repository Overview

- **Language:** C++ (core), JavaScript/Electron (UI), CMake (build)
- **Base:** Monero fork (CryptoNote protocol) with custom modifications
- **Repo:** EnticingTitan99/vindex-coin
- **Related repos (private):**
  - `vindex-coin-research` — tokenomics, whitepaper, research notes
  - `miners_fork_transfer` — cross-agent miner/fork transfer system (C++)

---

## Top-Level Directory Map

```
vindex-coin/
├── src/                  ← CORE SOURCE CODE (start here for logic questions)
│   ├── blockchain/       ← Chain storage, validation, reorganisation
│   ├── cryptonote_core/  ← Transaction pool, core protocol
│   ├── crypto/           ← Hashing, key generation, ring signatures
│   ├── wallet/           ← Wallet logic, key management
│   ├── rpc/              ← Daemon and wallet RPC endpoints
│   ├── p2p/              ← Peer discovery, sync, networking
│   ├── serialization/    ← Binary/JSON serialization
│   └── common/           ← Utilities, locks, thread helpers
├── tests/                ← Unit and integration tests
├── contrib/              ← Community scripts, snap/brew packaging
├── cmake/                ← Build system modules
├── external/             ← Third-party submodules (Boost, OpenSSL, etc.)
├── electron-app/         ← Desktop GUI (Electron/JS)
├── utils/                ← Dev tooling scripts
├── docs/                 ← Architecture and developer documentation
├── translations/         ← i18n strings
├── binaries/             ← Pre-built binaries (do not analyse for logic)
├── CMakeLists.txt        ← Primary build config (39KB — skim only if build question)
├── README.md             ← Project overview (40KB — high value for context)
└── Skills/               ← Manus AI orientation files
```

---

## Credit-Efficient Read Strategy

**Rule 1 — Map Before You Fetch**
Never bulk-read directories. Use this map to identify the one subdirectory relevant to the question, then fetch only that.

**Rule 2 — Priority Read Order**
1. `README.md` — once per session for project-level context
2. `src/cryptonote_core/` — consensus, tx validation, mempool
3. `src/crypto/` — hashing, ring signatures, key generation
4. `src/wallet/` — wallet, address, spend/view key logic
5. `src/rpc/` — API and RPC endpoints
6. `src/p2p/` — networking and peer sync
7. `src/blockchain/` — block storage, reorg, chain state

**Rule 3 — Skip These (Low Signal / High Cost)**
- `external/` — third-party upstream code, not custom logic
- `binaries/` — compiled artifacts only
- `Doxyfile` — documentation config, 100KB, no logic
- `translations/` — i18n strings only

**Rule 4 — Headers First**
In any `src/` subdirectory, read `.h` header files before `.cpp` implementation files. Headers expose the full class interface at ~10% of the token cost.

**Rule 5 — Tests as Ground Truth**
A single test file in `tests/` often reveals module intent faster than reading the implementation.

---

## Quick Glossary

| Term | Meaning |
|------|---------|
| Ring signature | Anonymity mechanism mixing real + decoy tx inputs |
| Stealth address | One-time address derived per transaction |
| View key | Read-only wallet scanning key |
| Spend key | Key authorising spending |
| RCT | RingCT — confidential transaction amounts |
| Mixin/ring size | Number of decoys in ring signature |
| Daemon | `vindexd` — the node process |
| Wallet RPC | `vindex-wallet-rpc` — wallet API server |
