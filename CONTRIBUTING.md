# Contributing to Vindex Coin

Thank you for your interest in contributing! This guide covers everything you need to get a working build on Ubuntu/Linux, understand the project structure, and submit quality contributions.

---

## Table of Contents

- [Prerequisites](#prerequisites)
- [Getting the Source](#getting-the-source)
- [Building on Ubuntu / Debian](#building-on-ubuntu--debian)
- [Common Build Errors & Fixes](#common-build-errors--fixes)
- [Project Structure](#project-structure)
- [Submitting a Pull Request](#submitting-a-pull-request)
- [Code Style](#code-style)

---

## Prerequisites

Ensure the following are installed before attempting a build:

```bash
sudo apt-get update
sudo apt-get install -y \
  build-essential cmake pkg-config git ccache \
  libssl-dev libzmq3-dev libunbound-dev \
  libsodium-dev libunwind8-dev liblzma-dev \
  libreadline6-dev libexpat1-dev \
  libboost-chrono-dev libboost-date-time-dev \
  libboost-filesystem-dev libboost-locale-dev \
  libboost-program-options-dev libboost-regex-dev \
  libboost-serialization-dev libboost-system-dev \
  libboost-thread-dev python3
```

**Minimum versions:**
| Tool | Minimum |
|---|---|
| GCC / Clang | GCC 9 / Clang 10 |
| CMake | 3.14 |
| Boost | 1.71 |
| OpenSSL | 1.1.1 |

---

## Getting the Source

> ⚠️ **Critical:** This repository uses Git submodules. A plain `git clone` will leave `external/rapidjson`, `external/randomx`, and `external/gtest` empty, causing an immediate CMake failure. Always clone with `--recurse-submodules`.

```bash
# Recommended: clone and populate all submodules in one step
git clone --recurse-submodules https://github.com/EnticingTitan99/vindex-coin.git
cd vindex-coin
```

If you already cloned without `--recurse-submodules`:

```bash
cd vindex-coin
git submodule update --init --force
```

### Required Submodules

| Path | Upstream | Purpose |
|---|---|---|
| `external/rapidjson` | Tencent/rapidjson | JSON parsing |
| `external/randomx` | tevador/RandomX | Proof-of-work algorithm |
| `external/supercop` | vindex-project/supercop | Cryptographic primitives |
| `external/gtest` | google/googletest | Unit testing |

---

## Building on Ubuntu / Debian

### Standard Release Build

```bash
make release
```

The built binary will be at:
```
build/Linux/main/release/bin/vindexd
```

### Manual CMake Build

If `make release` is unavailable, use CMake directly:

```bash
mkdir -p build/Linux/main/release
cd build/Linux/main/release

cmake \
  -DCMAKE_BUILD_TYPE=Release \
  -DBUILD_TESTS=OFF \
  -DUSE_DEVICE_TREZOR=OFF \
  ../../../..

make -j$(nproc) daemon
```

### Skipping Submodule Check (advanced)

If you have manually placed submodule source at the correct paths and wish to skip CMake's submodule validation:

```bash
cmake -DMANUAL_SUBMODULES=1 ...
```

### Build with ccache (faster rebuilds)

```bash
cmake \
  -DCMAKE_C_COMPILER_LAUNCHER=ccache \
  -DCMAKE_CXX_COMPILER_LAUNCHER=ccache \
  ...
```

### Docker Build

```bash
docker build --build-arg NPROC=$(nproc) -t vindex:local .
```

---

## Common Build Errors & Fixes

### ❌ `Submodule 'external/rapidjson' is not up-to-date`

**Cause:** Submodules were not initialised after cloning.

**Fix:**
```bash
git submodule update --init --force
```

Then re-run your build command.

---

### ❌ `-bash: ./build/release/bin/vindexd: No such file or directory`

**Cause:** The build failed before producing a binary (usually the CMake error above). The correct release binary path is:
```
build/Linux/main/release/bin/vindexd
```
not `build/release/bin/vindexd`.

**Fix:** Resolve the CMake error first, then rebuild. Check the path:
```bash
find build/ -name 'vindexd' 2>/dev/null
```

---

### ❌ CMake cannot find Boost / OpenSSL

**Fix:** Install the dev packages listed in [Prerequisites](#prerequisites). If you have a non-standard install location:
```bash
cmake -DBOOST_ROOT=/path/to/boost ...
```

---

### ❌ `make: cmake-release` error

**Cause:** CMake configuration failed. Check the full log at:
```
build/Linux/main/release/CMakeFiles/CMakeOutput.log
build/Linux/main/release/CMakeFiles/CMakeError.log
```

---

## Project Structure

```
vindex-coin/
├── src/                  # Core daemon & wallet source
├── external/             # Third-party libraries (git submodules)
│   ├── rapidjson/        # JSON (submodule)
│   ├── randomx/          # PoW algorithm (submodule)
│   ├── supercop/         # Crypto primitives (submodule)
│   └── gtest/            # Unit tests (submodule)
├── contrib/              # Community scripts & tools
├── tests/                # Test suite
├── electron-app/         # Desktop wallet (Electron)
├── cmake/                # CMake modules
├── .github/workflows/    # CI/CD pipelines
├── CMakeLists.txt        # Main build configuration
└── Makefile              # Convenience build targets
```

---

## Submitting a Pull Request

1. **Fork** the repository and create a feature branch from `main`:
   ```bash
   git checkout -b feat/your-feature-name
   ```
2. **Write your changes.** Keep commits atomic and well-described.
3. **Ensure the build passes** locally before pushing:
   ```bash
   git submodule update --init --force
   make release
   ```
4. **Push and open a PR** against `main`.
5. CI will automatically run the Linux build and submodule validation. Ensure all checks pass.

---

## Code Style

- C++14 standard (`-std=c++14`)
- Follow existing code style in the file you are editing
- Run `cppcheck` for static analysis before submitting:
  ```bash
  cppcheck --enable=warning,style,performance --error-exitcode=1 src/
  ```
- Avoid committing build artifacts, binaries, or IDE config files

---

*For questions, open an issue or visit the community channels linked in [README.md](README.md).*
