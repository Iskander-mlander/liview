#!/bin/bash

set -e

echo "=== XFCE Light Image Viewer - Uninstall Script ==="

if [ "$EUID" -ne 0 ]; then
    echo "This script requires root privileges for uninstallation."
    echo "Please run with: sudo $0"
    exit 1
fi

echo "[1/2] Removing binary..."
rm -f /usr/bin/xfce-liview

echo "[2/2] Removing Thunar integration..."
rm -f /usr/share/Thunar/uca.xml

echo ""
echo "=== Uninstall complete! ==="