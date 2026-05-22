# Vindex Project — Skills & Technologies

This document lists the core technologies, skills, and knowledge domains used in the Vindex project.

## Core Languages
- **C++17** — Primary implementation language (daemon, wallet, crypto core)
- **CMake** — Build system
- **Python 3** — Build scripts, tooling, test harness
- **Bash/Shell** — CI scripts, deployment

## Cryptography
- **RandomX** — Proof-of-Work algorithm (ASIC-resistant, CPU-friendly)
- **Ring Signatures (CLSAG)** — Transaction untraceability
- **Bulletproofs+** — Range proofs (compact, zero-knowledge)
- **RingCT (Ring Confidential Transactions)** — Amount hiding
- **Stealth Addresses** — One-time recipient addresses
- **View Tags** — Wallet scanning optimisation (fork v15+)
- **Ed25519 / Curve25519** — Elliptic curve cryptography
- **libsodium** — Cryptographic primitives

## Networking
- **Dandelion++** — Transaction privacy propagation
- **ZeroMQ (libzmq)** — Inter-process messaging & RPC pub/sub
- **Tor / I2P** — Anonymity network integration
- **P2P Protocol** — Custom peer-to-peer levin protocol
- **IPv4/IPv6 dual-stack** — Network support

## Storage
- **LMDB** — High-performance blockchain database
- **LevelDB** (legacy) — Earlier blockchain storage

## Build & DevOps
- **GitHub Actions** — CI/CD pipelines
- **Docker** — Containerised builds and deployment
- **Guix** — Reproducible, deterministic builds
- **ccache** — Compilation caching
- **MinGW / MSYS2** — Windows cross-compilation
- **Clang / GCC** — Compiler toolchains

## Testing
- **GTest** — Unit testing framework
- **AddressSanitizer (ASAN)** — Memory error detection
- **valgrind** — Memory debugging
- **Coverity** — Static analysis
- **OSS-Fuzz** — Continuous fuzzing

## Platforms Supported
- Linux (x86_64, ARM64, ARM32, RISC-V)
- macOS (Intel & Apple Silicon)
- Windows (via MinGW/MSYS2)
- Raspberry Pi
- FreeBSD, OpenBSD, NetBSD
- Android (ARM32/ARM64)

## Hardware Wallet Support
- **Ledger** (via libhidapi + libusb)
- **Trezor** (via libprotobuf)

## Vindex-Specific Differentiators
- Ticker: **$VDX**
- Tagline: *Libertatis Vindex* (Champion of Liberty)
- Named after the Roman concept of a *vindex*: a legal protector of liberty
- Derived from Monero codebase (CryptoNote / Monero v0.18 lineage)
- Network data dir: `.bitvindex`
- Daemon binary: `vindexd`
- Wallet binary: `vindex-wallet-cli`
