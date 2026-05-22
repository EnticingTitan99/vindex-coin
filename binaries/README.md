# Vindex Binaries

This directory holds the compiled Vindex daemon and wallet RPC binaries,
bundled into the Electron app at build time via `electron-builder`'s `extraResources`.

## Structure

```
binaries/
├── mac/
│   ├── vindexd          # Vindex daemon (macOS universal or x64/arm64)
│   └── vindex-wallet-rpc
└── win/
    ├── vindexd.exe      # Vindex daemon (Windows x64)
    └── vindex-wallet-rpc.exe
```

## How to populate

Build the C++ node from `src/` using CMake, then copy the output binaries here
before running `npm run build:mac` or `npm run build:win` from `electron-app/`.

```bash
# From repo root after CMake build:
cp build/bin/monerod  binaries/mac/vindexd
cp build/bin/monero-wallet-rpc binaries/mac/vindex-wallet-rpc
```

The `electron-builder` config in `electron-app/package.json` will
automatically bundle these into `Resources/bin/` inside the app package.
