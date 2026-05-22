# Installing Vindex Wallet

## macOS

### Download
Download the latest `Vindex-Wallet-x.x.x.dmg` from the [Releases](https://github.com/EnticingTitan99/vindex-coin/releases) page.

### Install
1. Open the `.dmg` file
2. Drag **Vindex Wallet** into your **Applications** folder
3. On first launch, right-click → **Open** (required on macOS to bypass Gatekeeper for unsigned apps)
4. Click **Open** in the confirmation dialog

### Architecture
- `x64` — Intel Macs
- `arm64` — Apple Silicon (M1/M2/M3/M4)

---

## Windows

### Download
Download the latest `Vindex-Wallet-Setup-x.x.x.exe` (installer) or `Vindex-Wallet-x.x.x-portable.exe` (no install needed) from the [Releases](https://github.com/EnticingTitan99/vindex-coin/releases) page.

### Installer
1. Run `Vindex-Wallet-Setup-x.x.x.exe`
2. Choose installation directory
3. Launch from the Desktop or Start Menu shortcut

### Portable
1. Run `Vindex-Wallet-x.x.x-portable.exe` directly — no installation needed
2. All data is stored relative to the executable

### Windows SmartScreen
If Windows SmartScreen appears, click **More info** → **Run anyway**.
This is expected for unsigned applications.

---

## Building from Source

```bash
# 1. Clone
git clone https://github.com/EnticingTitan99/vindex-coin.git
cd vindex-coin

# 2. Build the C++ daemon (requires CMake, Boost, OpenSSL)
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)

# 3. Copy binaries
mkdir -p ../binaries/mac
cp bin/monerod ../binaries/mac/vindexd
cp bin/monero-wallet-rpc ../binaries/mac/vindex-wallet-rpc

# 4. Build Electron app
cd ../electron-app
npm install
npm run build:all
# Output in electron-app/dist/
```
