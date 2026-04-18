#!/bin/bash

set -e

echo "=== XFCE Light Image Viewer - Debian Package Builder ==="

# Check if we're in the right directory
if [ ! -f "CMakeLists.txt" ]; then
    echo "Error: CMakeLists.txt not found. Please run from the project root."
    exit 1
fi

PKG_NAME="xfce-liview"
PKG_VERSION="1.0.0"
PKG_DIR="debian/xfce-liview"

echo "[1/6] Creating source package structure..."

# Create debian source directory structure
mkdir -p /tmp/${PKG_NAME}-${PKG_VERSION}
cd /tmp/${PKG_NAME}-${PKG_VERSION}

# Copy source files
cp -r $(pwd)/* .

# Create .orig.tar.gz
echo "Creating original source tarball..."
tar -czf ${PKG_NAME}_${PKG_VERSION}.orig.tar.gz --exclude='build' --exclude='.git' .

# Create debian directory
mkdir -p debian

echo "[2/6] Creating debian packaging files..."

# Create debian/control
cat > debian/control << 'EOF'
Source: xfce-liview
Section: graphics
Priority: optional
Maintainer: XFCE Community
Build-Depends: debhelper-compat (= 13), cmake, pkg-config, lib SDL2-dev, lib SDL2-image-dev, lib SDL2-ttf-dev, lib imlib2-dev, lib x11-dev
Standards-Version: 4.6.0

Package: xfce-liview
Architecture: any
Depends: ${misc:Depends}, libsdl2-2.0-0, libsdl2-image-2.0-0, libsdl2-ttf-2.0-0, libimlib2-1, yad
Description: Ultra-lightweight image viewer for XFCE
 Ultra-fast image viewer optimized for low-resource systems.
 Supports JPEG, PNG, GIF, BMP, and WebP formats.
 Features recursive directory search and minimal memory usage.
EOF

# Create debian/rules
cat > debian/rules << 'EOF'
#!/usr/bin/make -f

%:
	dh $@ --buildsystem=cmake

override_dh_auto_configure:
	dh_auto_configure -- -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr

override_dh_install:
	dh_install --exclude-build
	install -Dm644 debian/xfce-liview/usr/share/applications/xfce-liview.desktop debian/xfce-liview/usr/share/applications/
	install -Dm644 debian/uca.xml debian/xfce-liview/usr/share/Thunar/uca.xml
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/pixmaps/xfce-liview.png
	install -d debian/xfce-liview/usr/share/icons/hicolor/256x256/apps
	install -d debian/xfce-liview/usr/share/icons/hicolor/128x128/apps
	install -d debian/xfce-liview/usr/share/icons/hicolor/64x64/apps
	install -d debian/xfce-liview/usr/share/icons/hicolor/48x48/apps
	install -d debian/xfce-liview/usr/share/icons/hicolor/32x32/apps
	install -d debian/xfce-liview/usr/share/icons/hicolor/24x24/apps
	install -d debian/xfce-liview/usr/share/icons/hicolor/16x16/apps
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/256x256/apps/xfce-liview.png
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/128x128/apps/xfce-liview.png
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/64x64/apps/xfce-liview.png
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/48x48/apps/xfce-liview.png
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/32x32/apps/xfce-liview.png
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/24x24/apps/xfce-liview.png
	install -Dm644 debian/liview.png debian/xfce-liview/usr/share/icons/hicolor/16x16/apps/xfce-liview.png
EOF
chmod +x debian/rules

# Create debian/changelog
cat > debian/changelog << EOF
${PKG_NAME} (${PKG_VERSION}) stable; urgency=low

  * Initial release

 -- XFCE Community  $(date -R)
EOF

# Create debian/copyright
cat > debian/copyright << 'EOF'
Format: https://www.debian.org/doc/packaging-manuals/copyright-format/1.0/
Source: https://github.com/yourusername/xfce-liview

Files: *
Copyright: 2024 Iskander MegaLander, GrijanderousLender
License: GPL-3+
EOF

# Create debian/compat
echo "13" > debian/compat

# Create debian/source/format
mkdir -p debian/source
echo "3.0 (quilt)" > debian/source/format

# Copy desktop and icons
cp debian/xfce-liview/usr/share/applications/xfce-liview.desktop debian/
cp debian/xfce-liview/usr/share/Thunar/uca.xml debian/
cp icon/256x256/apps/liview.png debian/

echo "[3/6] Building package..."
dpkg-buildpackage -us -uc -b

echo "[4/6] Moving package to project directory..."
mv /tmp/${PKG_NAME}_${PKG_VERSION}_*.deb . 2>/dev/null || true

echo "[5/6] Cleaning up..."
cd /tmp
rm -rf ${PKG_NAME}-${PKG_VERSION}

echo "[6/6] Done!"
echo ""
echo "Package created successfully!"
ls -la *.deb 2>/dev/null || echo "Note: Package files are in /tmp/"
echo ""
echo "To install on a Debian/Ubuntu system:"
echo "  sudo dpkg -i xfce-liview_1.0.0_amd64.deb"
echo "  sudo apt-get install -f  # to install missing dependencies"