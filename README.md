# xfce-liview

<div align="center">

![Logo](https://img.shields.io/badge/XFCE-Light%20Image%20Viewer-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-GPL%20v3-green?style=for-the-badge)
![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge)
![SDL2](https://img.shields.io/badge/SDL2-2.0-orange?style=for-the-badge)
![Imlib2](https://img.shields.io/badge/Imlib2-1.12-green?style=for-the-badge)

**Ultra-lightweight image viewer for XFCE and Linux systems**

*Fast. Minimal. Low-resource.*

</div>

---

## Features
- **CLI support** - Open images directly from file manager (Thunar)
- **Lazy loading** - Fast directory scanning, loads images on demand
- **Imlib2 backend** - Fast image decoding using Imlib2
- **Texture cache** - Pre-loads adjacent images for smooth navigation
- **Smart cache reuse** - Reuses pre-loaded images instead of re-decoding
- **Scaled texture cache** - Caches scaled versions to avoid re-scaling on resize
- **Multi-language** - 20 languages supported
- **Simple interface** - No clutter, just your images
- **Keyboard navigation** - Full keyboard support
- **XFCE integration** - Thunar context menu support
- **Distribution** - AUR 

## Supported Formats

| Format | Extension |
|--------|-----------|
| JPEG   | .jpg, .jpeg |
| PNG    | .png |
| GIF    | .gif |
| BMP    | .bmp |
| WebP   | .webp |

## Screenshots

### Main Interface
```
┌─────────────────────────────────────────────┐
│ File  View  Help                            │
├─────────────────────────────────────────────┤
│                                             │
│                                             │
│              [Image Here]                   │
│                                             │
│                                             │
├─────────────────────────────────────────────┤
│ image.jpg (1/25)                            │
└─────────────────────────────────────────────┘
```
### Installation

**Quick Install (Debian/Ubuntu):**
```bash
# Run the build script (requires root for installation)
sudo ./build-deb.sh
```

**Manual Install (Debian/Ubuntu):**
```bash
# Install dependencies
sudo apt-get update
sudo apt-get install -y cmake g++ libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev libimlib2-dev libx11-dev yad

# Build
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make

# Install
sudo make install
sudo cp ../debian/xfce-liview/usr/share/applications/xfce-liview.desktop /usr/share/applications/
sudo cp ../uca.xml /usr/share/Thunar/uca.xml
```

### Arch Linux (AUR)

```bash
# manually
git clone https://github.com/Iskander-mlander/liview.git
cd xfce-liview
makepkg -si
```

### Build from Source
**Requirements:**
- C++17 compiler (g++ or clang++)
- CMake 3.10+
- SDL2
- SDL2_image
- SDL2_ttf
- YAD (Yet Another Dialog)

**Quick Install (Arch-based):**
```bash
sudo ./install-Arch.sh
```

**Manual Build:**
```bash
# Clone the repository
git clone https://github.com/wsnlndrv/xfce-liview.git
cd xfce-liview

# Create build directory
mkdir build && cd build

# Configure and build
cmake .. -DCMAKE_BUILD_TYPE=Release
make

# Run
./xfce-liview
```

**Install:**
```bash
sudo make install
```

**Uninstall:**
```bash
# Arch Linux
sudo ./uninstall-Arch.sh

# Debian/Ubuntu (if installed via package)
sudo dpkg -r xfce-liview

# Manual uninstall
sudo make uninstall
```

## Usage

### Command Line
```bash
# Open with image file (recommended for Thunar integration)
xfce-liview /path/to/image.png

# Open with folder path (recursive search)
xfce-liview /path/to/images
```

### Thunar Integration
After installation, you can open images directly from Thunar:
```
Right-click on image > Open with > XFCE Light Image Viewer
```

The viewer will open the clicked image and allow navigation through all images in the same directory.

## Keyboard Shortcuts
| Key | Action |
|-----|--------|
| `←` / `↑` / `PageUp` | Previous image |
| `→` / `↓` / `PageDown` / `Space` | Next image |
| `Home` | First image |
| `End` | Last image |
| `Mouse Wheel` | Navigate images |
| `F11` | Toggle fullscreen |
| `Escape` | Exit fullscreen / Close menu / Close |

## Menu Options
The menu automatically adapts to your system language. Available menus:

### File
- **Open folder...** - Select a new folder (YAD dialog)
- **Close** - Close the application

### View
- **Fullscreen** - Enter fullscreen mode
- **Window** - Return to windowed mode

### Help
- **About...** - Show about dialog
- **Language** - Change language (YAD dialog)

## Supported Languages
| Language | Code |
|-----------|------|
| Español | es |
| English | en |
| Deutsch | de |
| Français | fr |
| Italiano | it |
| Português | pt |
| Русский | ru |
| Türkçe | tr |
| Ελληνικά | el |
| العربية | ar |
| 中文 | zh |
| 日本語 | ja |
| 한국어 | ko |
| हिन्दी | hi |
| Bahasa Indonesia | id |
| فارسی | fa |
| Polski | pl |
| اردو | ur |
| Tiếng Việt | vi |

## Dependencies
| Package | Description |
|---------|-------------|
| SDL2 | Simple DirectMedia Layer |
| SDL2_image | Image file loading library |
| SDL2_ttf | TrueType font rendering |
| Imlib2 | Fast image decoding |
| YAD | GTK dialogs for folder selection |

## Performance Optimizations

The viewer implements several optimizations for fast image navigation:

### 1. Imlib2 Backend
Uses Imlib2 for image decoding - the same high-performance library.

### 2. Pre-load Cache
Pre-loads adjacent images (previous and next) in the background. When navigating, checks if the target image is already cached before loading from disk.

### 3. Smart Cache Reuse
Tracks which images are cached using `last_cached_prev` and `last_cached_next`. Avoids re-decoding if the cached texture is still valid.

### 4. Scaled Texture Cache
When an image needs to be scaled (downscaled for display), caches the scaled texture in `scaled_tex`. Reuses it on subsequent frames unless the window size changes.

### 4. Load Order Optimization
Loads from preload cache first before falling back to disk loading:
1. Check `cache_prev` - if contains current image, use directly
2. Check `cache_next` - if contains current image, use directly
3. Fall back to `IMG_Load()` from disk

## Project Structure
```
xfce-liview/
├── src/
│   ├── main.cpp      # Entry point
│   ├── viewer.h      # Header file
│   └── viewer.cpp    # Main implementation
├── icon/             # Application icons
├── debian/           # Debian packaging
├── CMakeLists.txt    # Build configuration
├── PKGBUILD          # Arch Linux AUR package
├── LICENSE           # GPL v3 license
└── README.md         # This file
```

## Contributing
1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## License
This project is licensed under the GNU General Public License v3.0 - see the [LICENSE](LICENSE) file for details.

## Authors
- **Iskander MegaLander** AKA wsnlndrv

## Acknowledgments
- Built with [SDL2](https://www.libsdl.org/)
- Inspired by XFCE's minimalist philosophy and Feh -> (the best!)
- Created using [OpenCode](https://opencode.ai) / Big Pickle

---

<div align="center">

**Made with ❤️ for the Linux community**

</div>
