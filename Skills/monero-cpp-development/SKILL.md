---
name: monero-cpp-development
description: "C++ build, debug, and development guide for the Vindex (Monero fork) codebase. Ubuntu 22.04 reference."
---

# Monero C++ Development (Vindex)

## Build (Ubuntu 22.04)

```bash
sudo apt-get update && sudo apt-get install -y \
  build-essential cmake pkg-config libssl-dev libzmq3-dev \
  libunbound-dev libsodium-dev libunwind8-dev liblzma-dev \
  libreadline6-dev libexpat1-dev libboost-all-dev \
  python3 ccache git

git clone --recursive https://github.com/EnticingTitan99/vindex-coin.git
cd vindex-coin
mkdir -p build/release && cd build/release
cmake -DCMAKE_BUILD_TYPE=Release -DUSE_DEVICE_TREZOR=OFF -DBUILD_TESTS=OFF ../..
make -j$(nproc) daemon
```

## Key Source Files

| File | Purpose |
|------|---------|
| `src/cryptonote_config.h` | ALL network parameters — primary config file |
| `src/hardforks/hardforks.cpp` | Hard fork schedule |
| `src/checkpoints/checkpoints.cpp` | Chain checkpoints |
| `src/cryptonote_basic/cryptonote_basic_impl.cpp` | Block reward / emission |
| `src/p2p/net_node.inl` | Seed nodes |
| `external/randomx/src/configuration.h` | RandomX PoW parameters |

## Common Build Issues

- **Submodules missing**: `git submodule update --init --recursive`
- **Boost not found**: `sudo apt-get install libboost-all-dev`
- **ccache speed**: `export CCACHE_MAXSIZE=10G` before building
- **Low RAM**: Use `make -j2` instead of `make -j$(nproc)` on machines with <8GB RAM

## Debug Build

```bash
cmake -DCMAKE_BUILD_TYPE=Debug -DSANITIZE=ON ../..
make -j$(nproc) daemon
```
