const { app, BrowserWindow, ipcMain, shell, Menu, nativeTheme } = require('electron');
const path = require('path');
const { spawn } = require('child_process');

let mainWindow;
let daemonProcess = null;
let walletRpcProcess = null;

// ─── Platform helpers ───────────────────────────────────────────────────────
const isMac = process.platform === 'darwin';
const isWin = process.platform === 'win32';
const binExt = isWin ? '.exe' : '';
const binDir = app.isPackaged
  ? path.join(process.resourcesPath, 'bin')
  : path.join(__dirname, '..', '..', 'binaries', isMac ? 'mac' : 'win');

function binPath(name) {
  return path.join(binDir, name + binExt);
}

// ─── Window ─────────────────────────────────────────────────────────────────
function createWindow() {
  mainWindow = new BrowserWindow({
    width: 1100,
    height: 720,
    minWidth: 800,
    minHeight: 560,
    titleBarStyle: isMac ? 'hiddenInset' : 'default',
    frame: !isWin,
    backgroundColor: nativeTheme.shouldUseDarkColors ? '#0f0e0d' : '#f7f6f2',
    webPreferences: {
      preload: path.join(__dirname, 'preload.js'),
      contextIsolation: true,
      nodeIntegration: false,
      sandbox: false
    },
    icon: path.join(__dirname, '..', 'assets', isWin ? 'icon.ico' : 'icon.icns')
  });

  mainWindow.loadFile(path.join(__dirname, 'renderer', 'index.html'));

  // Remove default menu in production
  if (app.isPackaged) {
    Menu.setApplicationMenu(null);
  }

  mainWindow.webContents.setWindowOpenHandler(({ url }) => {
    shell.openExternal(url);
    return { action: 'deny' };
  });
}

// ─── Daemon management ──────────────────────────────────────────────────────
function startDaemon() {
  if (daemonProcess) return;
  const daemon = binPath('vindexd');
  try {
    daemonProcess = spawn(daemon, [
      '--log-level', '0',
      '--non-interactive'
    ], { detached: false });
    daemonProcess.stdout.on('data', d => sendToRenderer('daemon:log', d.toString()));
    daemonProcess.stderr.on('data', d => sendToRenderer('daemon:log', d.toString()));
    daemonProcess.on('exit', () => { daemonProcess = null; sendToRenderer('daemon:stopped'); });
    sendToRenderer('daemon:started');
  } catch (e) {
    sendToRenderer('daemon:error', e.message);
  }
}

function stopDaemon() {
  if (daemonProcess) {
    daemonProcess.kill();
    daemonProcess = null;
  }
}

// ─── Wallet RPC management ───────────────────────────────────────────────────
function startWalletRpc(walletFile, password) {
  if (walletRpcProcess) return;
  const rpc = binPath('vindex-wallet-rpc');
  try {
    walletRpcProcess = spawn(rpc, [
      '--wallet-file', walletFile,
      '--password', password,
      '--rpc-bind-port', '18083',
      '--disable-rpc-login'
    ], { detached: false });
    walletRpcProcess.on('exit', () => { walletRpcProcess = null; });
  } catch (e) {
    sendToRenderer('wallet:error', e.message);
  }
}

function stopWalletRpc() {
  if (walletRpcProcess) {
    walletRpcProcess.kill();
    walletRpcProcess = null;
  }
}

// ─── IPC handlers ────────────────────────────────────────────────────────────
ipcMain.handle('daemon:start', () => { startDaemon(); return true; });
ipcMain.handle('daemon:stop',  () => { stopDaemon();  return true; });
ipcMain.handle('wallet:open',  (_, { file, password }) => { startWalletRpc(file, password); return true; });
ipcMain.handle('wallet:close', () => { stopWalletRpc(); return true; });
ipcMain.handle('theme:get',    () => nativeTheme.shouldUseDarkColors ? 'dark' : 'light');
ipcMain.handle('app:version',  () => app.getVersion());
ipcMain.on('window:minimize', () => mainWindow?.minimize());
ipcMain.on('window:maximize', () => mainWindow?.isMaximized() ? mainWindow.unmaximize() : mainWindow.maximize());
ipcMain.on('window:close',    () => mainWindow?.close());

function sendToRenderer(channel, data) {
  if (mainWindow && !mainWindow.isDestroyed()) {
    mainWindow.webContents.send(channel, data);
  }
}

// ─── App lifecycle ───────────────────────────────────────────────────────────
app.whenReady().then(() => {
  createWindow();
  app.on('activate', () => { if (BrowserWindow.getAllWindows().length === 0) createWindow(); });
});

app.on('window-all-closed', () => {
  stopDaemon();
  stopWalletRpc();
  if (!isMac) app.quit();
});

app.on('before-quit', () => {
  stopDaemon();
  stopWalletRpc();
});
