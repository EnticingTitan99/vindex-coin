---
name: linux-systems-administration-and-server-infrastructure
description: "Linux server setup for Vindex seed nodes and infrastructure. Ubuntu 22.04 reference."
---

# Linux Systems Administration — Vindex Infrastructure

## Minimum Server Specs (Seed Node)

| Resource | Minimum | Recommended |
|----------|---------|-------------|
| OS | Ubuntu 22.04 LTS | Ubuntu 22.04 LTS |
| RAM | 4 GB | 8 GB |
| Disk | 100 GB SSD | 250 GB SSD |
| Network | 100 Mbps | 1 Gbps |

## Server Hardening

```bash
# Create vindex user
useradd -m -s /bin/bash vindex

# Firewall
sudo ufw allow 28080/tcp  # P2P mainnet
sudo ufw allow 22/tcp     # SSH
sudo ufw deny 28081/tcp   # RPC — restrict to localhost or trusted IPs
sudo ufw enable
```

## Systemd Service

```ini
# /etc/systemd/system/vindexd.service
[Unit]
Description=Vindex Daemon
After=network.target

[Service]
User=vindex
ExecStart=/usr/local/bin/vindexd --detach --data-dir /var/lib/vindex --log-file /var/log/vindex/vindexd.log
Restart=always
RestartSec=30

[Install]
WantedBy=multi-user.target
```

```bash
sudo systemctl enable vindexd
sudo systemctl start vindexd
```

## Health Monitoring

```bash
# Check sync status
curl -s http://127.0.0.1:28081/get_info | python3 -m json.tool | grep -E 'height|synced|tx_count'

# Check peer count
curl -s http://127.0.0.1:28081/get_connections | python3 -m json.tool | grep -c 'peer_id'
```
