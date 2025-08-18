#!/usr/bin/env bash
set -eu
chmod 644 flag.txt
echo "[*] launching challenge (local)"
./pwn_canary_easy
