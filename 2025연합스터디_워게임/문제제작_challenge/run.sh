#!/usr/bin/env bash
set -euo pipefail

# 로컬 실행 스크립트
chmod 600 flag.txt 2>/dev/null || true
echo "[*] Launching ./challenge"
./challenge
