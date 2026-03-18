# Maintainer: Iskander MegaLander <email@example.com>
# Contributor: GrijanderousLender

pkgname=xfce-liview
pkgver=1.0.0
pkgrel=1
pkgdesc="Ultra-lightweight image viewer for XFCE - Fast, minimal, works on low-resource systems"
arch=("x86_64")
url="https://github.com/yourusername/xfce-liview"
license=("GPL3")
depends=("sdl2" "sdl2_image" "sdl2_ttf" "zenity")
makedepends=("cmake" "gcc" "make")
source=("$pkgname-$pkgver.tar.gz::https://github.com/yourusername/xfce-liview/archive/refs/tags/v$pkgver.tar.gz"
        "xfce-liview.desktop"
        "uca.xml"
        "LICENSE")
sha256sums=("SKIP"
            "SKIP"
            "SKIP"
            "SKIP")

build() {
    mkdir -p build && cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr
    make
}

package() {
    cd build
    make DESTDIR="$pkgdir" install
    
    install -Dm644 "$srcdir/xfce-liview.desktop" "$pkgdir/usr/share/applications/xfce-liview.desktop"
    install -Dm644 "$srcdir/uca.xml" "$pkgdir/usr/share/Thunar/uca.xml"
    
    install -d "$pkgdir/usr/share/pixmaps"
    install -Dm644 "icon/256x256/apps/liview.png" "$pkgdir/usr/share/pixmaps/xfce-liview.png"
    
    install -d "$pkgdir/usr/share/icons/hicolor/256x256/apps"
    install -d "$pkgdir/usr/share/icons/hicolor/128x128/apps"
    install -d "$pkgdir/usr/share/icons/hicolor/64x64/apps"
    install -d "$pkgdir/usr/share/icons/hicolor/48x48/apps"
    install -d "$pkgdir/usr/share/icons/hicolor/32x32/apps"
    install -d "$pkgdir/usr/share/icons/hicolor/24x24/apps"
    install -d "$pkgdir/usr/share/icons/hicolor/16x16/apps"
    
    install -Dm644 "icon/256x256/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/256x256/apps/xfce-liview.png"
    install -Dm644 "icon/128x128/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/128x128/apps/xfce-liview.png"
    install -Dm644 "icon/64x64/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/64x64/apps/xfce-liview.png"
    install -Dm644 "icon/48x48/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/48x48/apps/xfce-liview.png"
    install -Dm644 "icon/32x32/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/32x32/apps/xfce-liview.png"
    install -Dm644 "icon/24x24/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/24x24/apps/xfce-liview.png"
    install -Dm644 "icon/16x16/apps/liview.png" "$pkgdir/usr/share/icons/hicolor/16x16/apps/xfce-liview.png"
    
    install -Dm644 "$srcdir/LICENSE" "$pkgdir/usr/share/licenses/$pkgname/LICENSE"
}
