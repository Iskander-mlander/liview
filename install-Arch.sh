#!/bin/bash

set -e

echo "=== XFCE Light Image Viewer - Build Script ==="

if [ "$EUID" -ne 0 ]; then
    echo "This script requires root privileges for installation."
    echo "Please run with: sudo $0"
    exit 1
fi

DE="${XDG_CURRENT_DESKTOP:-${DESKTOP_SESSION:-unknown}}"
echo "Detected desktop: $DE"

echo "[1/6] Checking dependencies..."
deps=("sdl2" "sdl2_image" "sdl2_ttf" "yad" "cmake" "gcc")
for dep in "${deps[@]}"; do
    if ! pacman -Qs "^$dep$" > /dev/null 2>&1; then
        echo "Installing $dep..."
        pacman -S --noconfirm "$dep"
    fi
done

echo "[2/6] Creating build directory..."
cd "$(dirname "$0")"
mkdir -p build
cd build

echo "[3/6] Configuring with CMake..."
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr

echo "[4/6] Building..."
make -j$(nproc)

echo "[5/6] Installing..."
make install

echo "[6/6] Configuring desktop integration..."

install -Dm644 ../debian/xfce-liview/usr/share/applications/xfce-liview.desktop /usr/share/applications/xfce-liview.desktop

case "$DE" in
    *[Xx][Ff][Cc][Ee]*)
        echo " XFCE detected - installing Thunar integration..."
        install -Dm644 ../uca.xml /usr/share/Thunar/uca.xml
        ;;
    *[Oo][Pp][Ee][Nn][Bb][Oo][Xx]*|[Bb][Ss][Pp][Ww][Mm]*)
        echo " Openbox/BSPWM detected - configuring menu..."
        
        OB_MENU="$HOME/.config/openbox/menu.xml"
        if [ -f "$OB_MENU" ]; then
            if ! grep -q "xfce-liview" "$OB_MENU" 2>/dev/null; then
                echo " Adding entry to Openbox menu..."
                sed -i 's|<menu id="apps-graphics-menu" label="Gráficos">|<menu id="apps-graphics-menu" label="Gráficos">\n  <item label="XFCE Light Image Viewer"><action name="Execute"><command>xfce-liview %f</command><startupnotify><enabled>yes</enabled></startupnotify></action></item>|' "$OB_MENU" 2>/dev/null || true
                openbox --reconfigure 2>/dev/null || true
            fi
        fi
        echo " You can also right-click and select 'Openbox > Reconfigure' to refresh menus."
        ;;
    *[Kk][Dd][Ee]*)
        echo " KDE detected - desktop entry installed"
        ;;
    *[Gg][Nn][Oo][Mm][Ee]*)
        echo " GNOME detected - desktop entry installed"
        ;;
    *[Cc][Ii][Nn][Nn][Mm][Oo][Nn]*)
        echo " Cinnamon detected - desktop entry installed"
        ;;
    *[Mm][Aa][Tt][Ee]*)
        echo " MATE detected - desktop entry installed"
        ;;
    *[Ll][Xx][Qq][Tt]*)
        echo " LXQt detected - desktop entry installed"
        ;;
    *[Ll][Xx][Dd][Ee]*)
        echo " LXDE detected - desktop entry installed"
        ;;
    *)
        echo " Unknown desktop: $DE"
        ;;
esac

echo ""
echo "=== Installation complete! ==="
echo "Run with: xfce-liview <image-file or directory>"
echo ""
echo "Supported: XFCE (Thunar), Openbox, BSPWM, KDE, GNOME, Cinnamon, MATE, LXQt, LXDE"