---
name: genesis-block-and-network-parameter-configuration
description: "Two-pass genesis block generation and network parameter configuration for Vindex. Primary reference for cryptonote_config.h changes."
---

# Genesis Block and Network Parameter Configuration

## Current Status: PASS 1 PENDING

`GENESIS_TX = ""` is set in `src/cryptonote_config.h`. Follow the two-pass procedure below.

## Two-Pass Genesis Generation

### Pass 1
1. Confirm `GENESIS_TX = ""` in `src/cryptonote_config.h`
2. Build: `make -j$(nproc) daemon`
3. Run: `./build/release/bin/vindexd --print-genesis-tx`
4. Copy the full hex output

### Pass 2
1. Paste hex into `src/cryptonote_config.h`: `std::string const GENESIS_TX = "<hex>";
2. Rebuild: `make -j$(nproc) daemon`
3. Verify: `./build/release/bin/vindexd --stagenet --log-level 1`

## Vindex Network Parameters

| Parameter | Mainnet | Testnet | Stagenet |
|-----------|---------|---------|----------|
| P2P Port | 28080 | 38080 | 48080 |
| RPC Port | 28081 | 38081 | 48081 |
| ZMQ Port | 28082 | 38082 | 48082 |
| Address prefix | 20 (`V`) | 53 | 24 |
| Network ID | `VDXNET LIBERTATI` | `VDXNET LIBERTATJ` | `VDXNET LIBERTATK` |
| Genesis nonce | 10001 | 10002 | 10003 |

## Common Pitfalls

- Never reuse Monero's `NETWORK_ID` bytes
- Delete `~/.bitvindex/` between test runs
- Vindex block time is 60s — `DIFFICULTY_TARGET_V2 = 60`
