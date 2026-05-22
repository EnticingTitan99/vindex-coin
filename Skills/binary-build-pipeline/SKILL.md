# Skill: binary-build-pipeline

**Version:** 1.0  
**Project:** Vindex Coin (VDX)  
**Purpose:** Equip any AI agent with precise, repo-matched instructions for building `vindexd` and `vindex-wallet-rpc` binaries from source and staging them into the Electron desktop app.

---

## Overview

Vindex Coin is a Monero v0.18.x fork. Its C++ daemon (`vindexd`) and wallet RPC server (`vindex-wallet-rpc`) must be compiled and placed into `binaries/mac/` and `binaries/win/` before the Electron GUI (`electron-app/`) can launch a live node. The repo contains three GitHub Actions workflows that automate this:

| Workflow file | Trigger | Purpose |
|---|---|---|
| `.github/workflows/build-linux.yml` | push to `main`/`develop`, or manual | Build `vindexd` on Ubuntu 22.04 |
| `.github/workflows/build.yml` | push `v*` tag, or manual | Build Electron app (macOS + Windows installers) |
| `.github/workflows/release.yml` | push `v*` tag | Create draft GitHub Release with all artifacts |

The five steps below map 1-to-1 to these workflows plus local verification.

---

## Step 1 — Validate Submodules & Install Dependencies

### Submodule check (run locally or CI enforces via `submodule-check` job)

```bash
git submodule update --init --force

# Verify these three are populated (CI fails if any are empty):
ls external/randomx/CMakeLists.txt
ls external/rapidjson/include/
ls external/gtest/CMakeLists.txt
```

If any directory is empty, the `submodule-check` job in `build-linux.yml` will gate the build and print the fix command.

### Install build dependencies (Ubuntu 22.04 — exact match to CI)

```bash
sudo apt-get update -qq
sudo apt-get install -y \
  build-essential cmake pkg-config ccache git \
  libssl-dev libzmq3-dev libunbound-dev \
  libsodium-dev libunwind8-dev liblzma-dev \
  libreadline6-dev libexpat1-dev \
  libboost-chrono-dev libboost-date-time-dev \
  libboost-filesystem-dev libboost-locale-dev \
  libboost-program-options-dev libboost-regex-dev \
  libboost-serialization-dev libboost-system-dev \
  libboost-thread-dev python3
```

### Dependency reference

| Library | Purpose | Min version |
|---|---|---|
| CMake | Build system | 3.5 |
| Boost | Core utilities, networking, serialisation | 1.58 |
| OpenSSL | TLS / HTTPS RPC | 1.1 |
| libzmq | ZMQ RPC layer (port 28082) | 4.x |
| libsodium | Ed25519 key operations | any stable |
| libunbound | DNS seednode resolution | any stable |
| libunwind | Stack trace support | any stable |
| ccache | Object-file caching (optional but strongly recommended) | any stable |

---

## Step 2 — Build `vindexd` on Linux

This is the primary build. Linux is the reference platform and seed node OS.

### Option A — via GitHub Actions CI (recommended)

Trigger manually: **GitHub → Actions → Build Linux (vindexd) → Run workflow → Branch: main**

The artifact `vindexd-linux-amd64-{sha}` is retained for 14 days.

### Option B — local build on Ubuntu 22.04

```bash
mkdir -p build/Linux/main/release
cd build/Linux/main/release

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_C_COMPILER_LAUNCHER=ccache \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  -DBUILD_TESTS=OFF \
  -DUSE_DEVICE_TREZOR=OFF \
  ../../../..

# Build daemon only (faster):
make -j$(nproc) daemon

# Build daemon + wallet RPC (required for full app):
make -j$(nproc) daemon wallet_rpc
```

### Key CMake flags

| Flag | Value | Reason |
|---|---|---|
| `CMAKE_BUILD_TYPE` | `Release` | Full optimisation, strips debug symbols |
| `BUILD_TESTS` | `OFF` | Skips gtest suite — saves ~15 min compile |
| `USE_DEVICE_TREZOR` | `OFF` | Removes Trezor hardware wallet dependency |
| `*_COMPILER_LAUNCHER` | `ccache` | Caches objects — subsequent builds ~5–8 min instead of ~40 min |

### Verification

```bash
ls -lh build/Linux/main/release/bin/vindexd
build/Linux/main/release/bin/vindexd --version
# Expected: binary ~20-40 MB; version string printed without error
```

### Build time estimates

| Condition | Time |
|---|---|
| Cold build (no cache), 4-core runner | ~35–45 min |
| Warm build (ccache hit) | ~5–8 min |

---

## Step 3 — Cross-Build for macOS & Windows

### macOS (native or `macos-latest` runner)

```bash
# Install Homebrew dependencies:
brew install cmake boost openssl zmq libsodium unbound ccache

mkdir -p build/mac/release && cd build/mac/release

# Intel (x64):
cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DOPENSSL_ROOT_DIR=$(brew --prefix openssl) \
  -DBUILD_TESTS=OFF \
  -DUSE_DEVICE_TREZOR=OFF \
  ../../..

# Apple Silicon (arm64) — add this flag:
# -DCMAKE_OSX_ARCHITECTURES=arm64

# Universal binary (both arches — requires Xcode 12+):
# -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64"

make -j$(sysctl -n hw.logicalcpu) daemon wallet_rpc
```

### Windows (cross-compile from Linux using mingw-w64)

```bash
sudo apt-get install -y mingw-w64

mkdir -p build/win/release && cd build/win/release

cmake \
  -DCMAKE_TOOLCHAIN_FILE=../../../../cmake/toolchain/mingw64.cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTS=OFF \
  ../../..

make -j$(nproc) daemon wallet_rpc
```

Alternatively, use the `windows-latest` GitHub Actions runner to build natively with MSVC — the `ci.yml` workflow handles Boost Windows binary prefetch.

### Expected output after Step 3

| Binary | Platform | Build output path |
|---|---|---|
| `vindexd` | Linux x64 | `build/Linux/main/release/bin/` |
| `vindex-wallet-rpc` | Linux x64 | `build/Linux/main/release/bin/` |
| `vindexd` | macOS x64/arm64 | `build/mac/release/bin/` |
| `vindex-wallet-rpc` | macOS x64/arm64 | `build/mac/release/bin/` |
| `vindexd.exe` | Windows x64 | `build/win/release/bin/` |
| `vindex-wallet-rpc.exe` | Windows x64 | `build/win/release/bin/` |

---

## Step 4 — Stage Binaries into `binaries/` & Verify Electron Integration

### How `main.js` resolves binary paths

```javascript
// electron-app/src/main.js

// Development (not packaged):
const binDir = path.join(__dirname, '..', '..', 'binaries', isMac ? 'mac' : 'win');

// Production (packaged app):
const binDir = path.join(process.resourcesPath, 'bin');
```

The binary names expected are `vindexd` (Linux/macOS) and `vindexd.exe` (Windows), and likewise for `vindex-wallet-rpc`.

### Copy binaries to staging directories

```bash
# macOS
mkdir -p binaries/mac
cp build/mac/release/bin/vindexd            binaries/mac/vindexd
cp build/mac/release/bin/vindex-wallet-rpc  binaries/mac/vindex-wallet-rpc
chmod +x binaries/mac/vindexd binaries/mac/vindex-wallet-rpc

# Windows
mkdir -p binaries/win
cp build/win/release/bin/vindexd.exe            binaries/win/vindexd.exe
cp build/win/release/bin/vindex-wallet-rpc.exe  binaries/win/vindex-wallet-rpc.exe
```

> **Important:** Add `binaries/` to `.gitignore`. Compiled binaries are never committed — they are distributed via GitHub Release assets and pulled in at Electron build time via `extraResources`.

### Run Electron in dev mode and verify daemon IPC

```bash
cd electron-app
npm install
npm run dev
```

Verification checklist:
- [ ] **Daemon tab → Start Node**: status dot turns green ("Connected")
- [ ] **Daemon Log panel**: shows `vindexd` startup lines (P2P init, DB open, etc.)
- [ ] **Stop Node**: status dot returns to grey ("Disconnected")
- [ ] `vindex-wallet-rpc` IPC (`wallet:open` handler): spawns process on port `18083`; `get_version` RPC call returns without error

### wallet-rpc ports (VDX-specific)

| Network | P2P port | RPC port | ZMQ port |
|---|---|---|---|
| Mainnet | 28080 | 28081 | 28082 |
| Testnet | 38080 | 38081 | 38082 |
| Stagenet | 48080 | 48081 | 48082 |
| Wallet RPC | — | 18083 | — |

---

## Step 5 — Tag a Release & Publish via GitHub Actions

### Pre-release checklist

```bash
git status         # must be clean
git log --oneline -5  # verify HEAD is correct
```

### Push a version tag

```bash
git tag v0.1.0-testnet
git push origin v0.1.0-testnet
```

This triggers `build.yml`, which:
1. Runs `npm install` + `npm run build:mac` on `macos-latest` (produces `.dmg` + `.zip` for x64 + arm64)
2. Runs `npm install` + `npm run build:win` on `windows-latest` (produces NSIS `.exe` installer + portable `.exe`)
3. Creates a **draft GitHub Release** named `Vindex Wallet v0.1.0-testnet` with all platform artifacts attached

### Inject daemon binaries into the Electron build

The `build.yml` workflow builds the GUI shell only. Before `npm run build:mac/win`, a workflow step must download the `vindexd-linux-amd64-{sha}` CI artifact and copy it to `electron-app/bin/`. Add between `npm install` and `npm run build:mac`:

```yaml
- name: Download vindexd artifact
  uses: actions/download-artifact@v4
  with:
    name: vindexd-linux-amd64-${{ github.sha }}
    path: electron-app/bin/
```

### Publish the release

1. Go to **GitHub → Releases → (draft release)**
2. Verify all four platform artifacts are attached
3. Add release notes (testnet parameters, ports, network ID `VDXNET LIBERTATJ`)
4. For testnet: leave "Set as pre-release" checked
5. For mainnet promotion: uncheck pre-release
6. Click **Publish release**

### Post-publish verification

- [ ] `vindexd --version` output matches the tag
- [ ] `.dmg` installs and launches on macOS (Gatekeeper may require right-click → Open for unsigned builds)
- [ ] `.exe` installer runs on Windows (SmartScreen will warn for unsigned builds — requires code signing certificate for production)
- [ ] Daemon connects to at least one seed node on testnet
- [ ] Wallet RPC endpoint `127.0.0.1:18083/json_rpc` responds to `get_version`

---

## Common Failure Modes & Fixes

| Symptom | Likely cause | Fix |
|---|---|---|
| `submodule-check` fails in CI | Submodules empty / not initialised | `git submodule update --init --force` |
| CMake error: `Boost not found` | Boost not installed or wrong version | `sudo apt-get install libboost-all-dev` |
| CMake error: `OpenSSL not found` on macOS | Homebrew OpenSSL not on path | Add `-DOPENSSL_ROOT_DIR=$(brew --prefix openssl)` |
| `make` fails with OOM | Not enough RAM on runner | Reduce parallelism: `make -j2` instead of `-j$(nproc)` |
| Electron: daemon status stays grey | `binaries/mac/` or `binaries/win/` empty | Stage binaries per Step 4 |
| Electron: IPC `wallet:open` fails | `vindex-wallet-rpc` not in `binaries/` | Copy `vindex-wallet-rpc` binary alongside `vindexd` |
| macOS Gatekeeper blocks binary | Binary not signed | Right-click → Open, or `xattr -d com.apple.quarantine binaries/mac/vindexd` |
| Windows SmartScreen blocks installer | Unsigned executable | Accepted for testnet; requires EV code-signing cert for mainnet |
| `vindexd` crashes at genesis | Hardfork table misconfiguration | v1 must be at height 0, all subsequent forks at height 1 — do NOT change hardforks.cpp |

---

## Repo Path Reference

| Path | Content |
|---|---|
| `src/cryptonote_config.h` | All coin constants (ports, prefixes, emission, ring size) |
| `src/hardforks.cpp` | Hardfork schedule — v1@h0, v2–v15@h1 |
| `src/checkpoints.cpp` | DNS seednode stub + checkpoint table |
| `electron-app/src/main.js` | Electron main process, daemon/wallet IPC handlers |
| `electron-app/src/renderer/` | All GUI pages (Overview, Send, Receive, Transactions, Miner, Daemon, Settings) |
| `electron-app/package.json` | electron-builder config, npm scripts (`build:mac`, `build:win`, `build:all`) |
| `.github/workflows/build-linux.yml` | Daemon build CI (Ubuntu 22.04) |
| `.github/workflows/build.yml` | Electron app build CI (macOS + Windows) |
| `.github/workflows/release.yml` | GitHub Release creation |
| `binaries/mac/` | Staging directory for macOS daemon binaries (not committed) |
| `binaries/win/` | Staging directory for Windows daemon binaries (not committed) |

---

## Notes for AI Agents

- **Never modify** `src/hardforks.cpp` hardfork heights or the genesis TX in `src/cryptonote_core/cryptonote_format_utils.cpp`. These are locked.
- **Never commit** compiled binaries to the repo. Use GitHub Release assets only.
- The wallet RPC renderer pages (`Send`, `Receive`, `Transactions`) currently contain TODO stubs. Wallet RPC wiring is the next development phase after binary staging is complete.
- `cryptonote_config.h` documents a planned v19 ring size increase (11 → 16). Do not implement this until the chain has >100k outputs.
- Seed nodes are not yet deployed. The chain cannot sync peers without at least one always-on `vindexd` instance with its IP hardcoded in `src/p2p/net_node.inl`.
