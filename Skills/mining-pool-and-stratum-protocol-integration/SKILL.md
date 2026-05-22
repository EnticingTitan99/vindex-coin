---
name: mining-pool-and-stratum-protocol-integration
description: "Mining pool setup for Vindex using nodejs-pool. Covers config.json adaptation, address validation, block template parsing."
---

# Mining Pool and Stratum Protocol Integration

## Recommended Software: nodejs-pool

https://github.com/dvandal/cryptonote-nodejs-pool

## Key config.json Changes

```json
{
  "coin": "vindex",
  "symbol": "VDX",
  "coinUnits": 1000000000000,
  "coinDecimalPlaces": 4,
  "daemon": {
    "host": "127.0.0.1",
    "port": 28081
  },
  "wallet": {
    "host": "127.0.0.1",
    "port": 28083
  },
  "poolAddress": "V...<your pool address>"
}
```

## Address Validation

Vindex addresses start with `V` (Base58 prefix 20). Update `lib/api.js` address validation regex to match Vindex address format.

## Block Template

The daemon's `get_block_template` RPC response format is identical to Monero — no changes needed to the template parsing logic.

## Stratum Port

Default: 3333 (low diff), 5555 (mid diff), 7777 (high diff). No conflict with Vindex daemon ports.
