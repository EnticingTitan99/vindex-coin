# Vindex Genesis Block Generation Guide

> **STATUS: PASS 1 PENDING** — `GENESIS_TX` is currently set to `""` in `src/cryptonote_config.h`. The build will produce a daemon that outputs the genesis transaction hex. Follow this guide immediately after a successful build.

## Overview

Generating a unique genesis block requires two compilation passes. This is a standard CryptoNote/Monero fork procedure.

---

## Pass 1: Generate the Genesis TX Hex

### 1. Verify the config is ready
```cpp
// In src/cryptonote_config.h — should currently read:
std::string const GENESIS_TX = ""; // empty — triggers genesis generation
uint32_t const GENESIS_NONCE = 10001;
```

### 2. Build the daemon
```bash
# Ubuntu 22.04
sudo apt-get update && sudo apt-get install -y \
  build-essential cmake pkg-config libssl-dev libzmq3-dev \
  libunbound-dev libsodium-dev libunwind8-dev liblzma-dev \
  libreadline6-dev libexpat1-dev \
  libboost-all-dev python3 ccache git

git clone --recursive https://github.com/EnticingTitan99/vindex-coin.git
cd vindex-coin
mkdir -p build/release && cd build/release
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_DEVICE_TREZOR=OFF -DBUILD_TESTS=OFF ../..
make -j$(nproc) daemon
```

### 3. Run genesis generation
```bash
cd build/release
./bin/vindexd --print-genesis-tx
```

You will see output like:
```
Genesis transaction: 013c01ff0001...<long hex string>...
```

Copy the **full hex string** (everything after `Genesis transaction: `).

---

## Pass 2: Embed the Genesis TX

### 1. Edit `src/cryptonote_config.h`
Find this section and replace the empty string:
```cpp
// BEFORE:
std::string const GENESIS_TX = "";

// AFTER (paste your hex from Pass 1):
std::string const GENESIS_TX = "013c01ff0001...<your hex here>...";
```

### 2. Rebuild
```bash
cd build/release
make -j$(nproc) daemon
```

### 3. Verify the daemon starts cleanly
```bash
./bin/vindexd --stagenet --log-level 1
# Should show: "Vindex v... - Libertatis Vindex"
# Should show: Starting p2p server on 0.0.0.0:48080
# Should NOT show any genesis errors
```

---

## Network Parameters (for reference)

| Parameter | Mainnet | Testnet | Stagenet |
|-----------|---------|---------|----------|
| P2P Port | 28080 | 38080 | 48080 |
| RPC Port | 28081 | 38081 | 48081 |
| ZMQ Port | 28082 | 38082 | 48082 |
| Address prefix | 20 (`V`) | 53 | 24 |
| Network ID | `VDXNET LIBERTATI` | `VDXNET LIBERTATJ` | `VDXNET LIBERTATK` |
| Genesis nonce | 10001 | 10002 | 10003 |

---

## Common Pitfalls

- **Submodule errors**: Run `git submodule update --init --recursive` before building
- **LMDB error on startup**: Delete `~/.bitvindex/` if it exists from a previous attempt
- **Wrong block time**: Vindex uses 60s blocks (Monero uses 120s) — ensure `DIFFICULTY_TARGET_V2 = 60` is in place
- **Port conflict**: Ensure no Monero node is running on 18080. Vindex uses 28080.

---

## After Genesis is Complete

1. Commit the updated `cryptonote_config.h` with the real GENESIS_TX
2. Tag the commit: `git tag v0.1.0-genesis && git push origin v0.1.0-genesis`
3. Proceed to seed node deployment — see `docs/SEED_NODES.md`
