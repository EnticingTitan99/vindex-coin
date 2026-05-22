---
name: p2p-networking-and-node-architecture
description: "P2P networking configuration for Vindex nodes. Covers seed nodes, network isolation, Levin protocol, Dandelion++."
---

# P2P Networking and Node Architecture

## Network Isolation (Done)

- `NETWORK_ID` = `VDXNET LIBERTATI` (16 ASCII bytes) — prevents peering with Monero
- P2P port: 28080 (mainnet), 38080 (testnet), 48080 (stagenet)
- These differ from Monero's 18080/28080/38080

## Seed Nodes (TODO)

Add hardcoded seed nodes in `src/p2p/net_node.inl`:

```cpp
// Vindex mainnet seed nodes
// Add after deploying first persistent server
static const std::vector<std::string> mainnet_seed_nodes = {
  // "<IP>:28080",
};
```

Minimum recommended: 3 seed nodes on separate VPS providers.

## Dandelion++

Kept identical to Monero — battle-tested transaction privacy relay. No changes needed.

## Levin Protocol

CryptoNote inter-node communication protocol. Network ID bytes are checked on handshake — if they don't match, nodes immediately drop the connection. This is the primary isolation mechanism.

## Firewall Rules (for seed node servers)

```bash
sudo ufw allow 28080/tcp  # P2P
sudo ufw allow 28081/tcp  # RPC (restrict to trusted IPs in production)
sudo ufw allow 22/tcp     # SSH
sudo ufw enable
```
