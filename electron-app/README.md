# Vindex Wallet — Desktop App

> Libertatis Vindex · Classy, minimal Electron wallet & miner for VDX

## Features

- **Wallet UI** — Overview, Send, Receive, Transaction history
- **Miner UI** — CPU mining via daemon RPC (RandomX)
- **Daemon control** — Start/stop the `vindexd` node from the GUI
- **Light & dark mode** — Follows system preference, toggle in titlebar
- **macOS + Windows** — Native installers for both platforms

---

## Development

```bash
cd electron-app
npm install
npm start
```

> Requires Node.js ≥ 18 and npm ≥ 9.

---

## Building Installers

### Prerequisites

Place compiled Vindex binaries in:
```
binaries/
├── mac/
│   ├── vindexd          # macOS daemon (arm64 + x64 universal)
│   └── vindex-wallet-rpc
└── win/
    ├── vindexd.exe      # Windows daemon (x64)
    └── vindex-wallet-rpc.exe
```

### macOS (`.dmg` + `.zip`)
```bash
npm run build:mac
# Output: dist/Vindex Wallet-1.0.0.dmg  (Intel)
#         dist/Vindex Wallet-1.0.0-arm64.dmg  (Apple Silicon)
```

### Windows (NSIS installer + portable `.exe`)
```bash
npm run build:win
# Output: dist/Vindex Wallet Setup 1.0.0.exe
#         dist/Vindex Wallet 1.0.0.exe  (portable)
```

### Both platforms at once
```bash
npm run build:all
```

> **macOS codesigning:** Set `CSC_LINK` and `CSC_KEY_PASSWORD` env vars with your `.p12` certificate to sign the `.dmg`. Unsigned builds will trigger Gatekeeper — users can right-click → Open to bypass.

> **Windows codesigning:** Set `WIN_CSC_LINK` and `WIN_CSC_KEY_PASSWORD` for EV signing. Without a cert, SmartScreen will warn on first run.

---

## Architecture

```
electron-app/
├── src/
│   ├── main.js          # Electron main process — window, IPC, child processes
│   ├── preload.js       # Context bridge — exposes vindex API to renderer
│   └── renderer/
│       └── index.html   # Full UI (vanilla HTML/CSS/JS — no framework needed)
├── assets/
│   ├── icon.icns        # macOS app icon (place 1024×1024 here)
│   ├── icon.ico         # Windows app icon (256×256)
│   ├── dmg-background.png   # macOS DMG installer background
│   └── entitlements.mac.plist
└── package.json         # Build config + electron-builder
```

## Wallet RPC Integration

The renderer communicates with `vindex-wallet-rpc` (local HTTP on port `18083`).
All wallet calls go through `window.vindex.*` (preload bridge) → IPC → main process.

To add a balance fetch, call the wallet RPC:
```js
fetch('http://127.0.0.1:18083/json_rpc', {
  method: 'POST',
  headers: { 'Content-Type': 'application/json' },
  body: JSON.stringify({ jsonrpc: '2.0', id: '0', method: 'get_balance', params: { account_index: 0 } })
}).then(r => r.json()).then(data => console.log(data.result.balance));
```

## Design

- **Fonts:** Instrument Serif (display) + Satoshi (body)
- **Colors:** Vindex dark — near-black surfaces, teal accent (`#4fa8b0` dark / `#01696f` light)
- **Both modes:** Light and dark, system-aware with manual toggle
- **Style:** Classy, minimal — no gradients, no glow, no noise
