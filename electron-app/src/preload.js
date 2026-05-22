const { contextBridge, ipcRenderer } = require('electron');

contextBridge.exposeInMainWorld('vindex', {
  // Daemon
  daemonStart:  () => ipcRenderer.invoke('daemon:start'),
  daemonStop:   () => ipcRenderer.invoke('daemon:stop'),
  onDaemonLog:  (cb) => ipcRenderer.on('daemon:log',     (_, d) => cb(d)),
  onDaemonStart:(cb) => ipcRenderer.on('daemon:started', ()    => cb()),
  onDaemonStop: (cb) => ipcRenderer.on('daemon:stopped', ()    => cb()),
  onDaemonError:(cb) => ipcRenderer.on('daemon:error',   (_, e) => cb(e)),

  // Wallet
  walletOpen:   (file, password) => ipcRenderer.invoke('wallet:open', { file, password }),
  walletClose:  () => ipcRenderer.invoke('wallet:close'),
  onWalletError:(cb) => ipcRenderer.on('wallet:error',   (_, e) => cb(e)),

  // App
  getTheme:     () => ipcRenderer.invoke('theme:get'),
  getVersion:   () => ipcRenderer.invoke('app:version'),

  // Window controls (Windows custom frame)
  minimize:     () => ipcRenderer.send('window:minimize'),
  maximize:     () => ipcRenderer.send('window:maximize'),
  close:        () => ipcRenderer.send('window:close')
});
