---
name: block-explorer-and-wallet-adaptation
description: "Block explorer and wallet adaptation for Vindex. Covers onion-monero-blockchain-explorer and monero-gui rebranding."
---

# Block Explorer and Wallet Adaptation

## Block Explorer: onion-monero-blockchain-explorer

https://github.com/moneroexamples/onion-monero-blockchain-explorer

### Key changes in `src/config.h`:
```cpp
#define BLOCKCHAIN_DB "./blockchain"
#define DAEMON_URL "http://127.0.0.1:28081"  // Vindex RPC port
#define NETWORK_TYPE 0  // mainnet
#define COIN_NAME "Vindex"
#define COIN_SYMBOL "VDX"
#define ADDRESS_PREFIX 20  // 'V'
```

## CLI Wallet: vindex-wallet-cli

The wallet binary is already named `vindex-wallet-cli` if CMake is set up correctly. Ensure:
- Default daemon port updated to 28081
- Address prefix checks updated (prefix 20 not 18)

## GUI Wallet: monero-gui fork

https://github.com/monero-project/monero-gui

1. Fork the repo
2. Update `version.js`, `main.qml` with Vindex branding
3. Update default daemon port to 28081
4. Update address validation (prefix 20)
5. Replace Monero logo/icons with Vindex branding
