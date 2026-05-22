# Skill: repo_navigator

## Purpose
Give Manus AI a fast, credit-minimal orientation to the vindex-coin codebase before answering any question. Read this skill FIRST, then use the directory map below to fetch ONLY the files relevant to the user's question.

---

## Repository: vindex-coin
- **Language:** C++ (core), JavaScript/Electron (UI), CMake (build)
- **Base:** Monero fork (CryptoNote protocol) with custom modifications
- **Repo URL:** https://github.com/EnticingTitan99/vindex-coin
- **Related repo:** `vindex-coin-research` (private — research notes, tokenomics, whitepaper material)
- **Related repo:** `miners_fork_transfer` (private — Monero fork transfer logic across agents)

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
└── Skills/               ← THIS FOLDER — Manus AI orientation files
```

---

## Credit-Efficient Read Strategy

### Rule 1 — Map Before You Fetch
Never bulk-read directories. Use this map to identify the ONE subdirectory relevant to the question, then fetch only that.

### Rule 2 — Priority Read Order
1. `README.md` — read once per session for project-level context
2. `src/cryptonote_core/` — for consensus, tx validation, mempool questions
3. `src/crypto/` — for hashing, ring signatures, key generation questions
4. `src/wallet/` — for wallet, address, spend/view key questions
5. `src/rpc/` — for API/endpoint questions
6. `src/p2p/` — for networking, sync, peer questions
7. `src/blockchain/` — for block storage, reorg, chain state questions

### Rule 3 — Skip These (Low Signal / High Cost)
- `external/` — third-party code, not custom logic
- `binaries/` — compiled artifacts
- `Doxyfile` — documentation config only (100KB, skip entirely)
- `translations/` — i18n strings, not logic

### Rule 4 — Header Files First
In any `src/` subdirectory, read `.h` header files before `.cpp` files. Headers expose the full interface (class names, method signatures, data structures) at ~10% the token cost of implementation files.

### Rule 5 — Test Files as Ground Truth
`tests/` files are dense with intent. A single test file often reveals what a module is *supposed* to do faster than reading the implementation.

---

## Key Custom Modifications vs Base Monero

This is a **fork**, so assume Monero-standard behaviour UNLESS a file in `src/` has been modified. To check what's custom:
- Look for non-standard class names or namespaces in headers
- Check git blame / commit messages for divergence points
- `README.md` describes high-level custom features

---

## Quick Glossary (CryptoNote/Monero Terms)
| Term | Meaning |
|------|----------|
| Ring signature | Anonymity mechanism mixing real + decoy tx inputs |
| Stealth address | One-time address derived per transaction |
| View key | Key allowing read-only wallet scanning |
| Spend key | Key authorising spending |
| RCT | RingCT — confidential transaction amounts |
| Mixin/ring size | Number of decoys in ring signature |
| Daemon | `vindexd` — the node process |
| Wallet RPC | `vindex-wallet-rpc` — wallet API server |
