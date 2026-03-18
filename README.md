# xfce-liview

<div align="center">

![Logo](https://img.shields.io/badge/XFCE-Light%20Image%20Viewer-blue?style=for-the-badge)
![License](https://img.shields.io/badge/License-GPL%20v3-green?style=for-the-badge)
![C++](https://img.shields.io/badge/C++-17-blue?style=for-the-badge)
![SDL2](https://img.shields.io/badge/SDL2-2.0-orange?style=for-the-badge)

**Ultra-lightweight image viewer for XFCE and Linux systems**

*Fast. Minimal. Low-resource.*

</div>

---

## Features

- **Lightning fast** - Loads in under 100ms
- **Minimal footprint** - Uses less than 30MB RAM
- **Multi-language** - 20 languages supported
- **Simple interface** - No clutter, just your images
- **Keyboard navigation** - Full keyboard support
- **XFCE integration** - Thunar context menu support
- **Cross-distribution** - DEB and AUR packages available

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
│ Archivo  Ver  Ayuda                        │
├─────────────────────────────────────────────┤
│                                             │
│                                             │
│              [Image Here]                   │
│                                             │
│                                             │
├─────────────────────────────────────────────┤
│ image.jpg (1/25)                      ─ □ ✕ │
└─────────────────────────────────────────────┘
```

### About Dialog
```
┌─────────────────────────────────────────────┐
│                                             │
│   ┌──────────┐   XFCE Light Image Viewer    │
│   │          │   Version 1.0.0              │
│   │   🖼️     │                              │
│   │          │   Purpose:                    │
│   └──────────┘   Ultra-lightweight viewer    │
│                  for low-resource systems    │
│                                             │
│                  Author:                     │
│                  Iskander MegaLander        │
│                  GrijanderousLender         │
│                                             │
│                  License: GNU GPL v3+        │
│                                             │
│             [ Click to close ]              │
└─────────────────────────────────────────────┘
```

## Installation

### Debian/Ubuntu

```bash
# Download the latest .deb package
sudo dpkg -i xfce-liview_1.0.0_amd64.deb

# Install dependencies if needed
sudo apt-get install -f
```

### Arch Linux (AUR)

```bash
# Using yay
yay -S xfce-liview

# Or manually
git clone https://aur.archlinux.org/xfce-liview.git
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
- Zenity

**Build:**
```bash
# Clone the repository
git clone https://github.com/yourusername/xfce-liview.git
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

## Usage

### Command Line
```bash
# Open with folder path
xfce-liview /path/to/images

# Or just run (will prompt for folder)
xfce-liview
```

### Thunar Integration
After installation, right-click any folder in Thunar and select:
```
Open with > XFCE Light Image Viewer
```

## Keyboard Shortcuts

| Key | Action |
|-----|--------|
| `←` / `→` | Previous / Next image |
| `Home` | First image |
| `End` | Last image |
| `Mouse Wheel` | Navigate images |
| `F11` / `Escape` | Toggle fullscreen |
| `Ctrl + Q` | Quit |

## Menu Options

### Archivo (File)
- **Abrir carpeta...** - Select a new folder
- **Cerrar** - Close the application

### Ver (View)
- **Pantalla completa** - Enter fullscreen mode
- **Ventana** - Return to windowed mode

### Ayuda (Help)
- **Acerca de...** - Show about dialog
- **Idioma** - Change language

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
| Zenity | GTK dialogs for folder selection |

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

- **Iskander MegaLander**
- **GrijanderousLender**

## Acknowledgments

- Built with [SDL2](https://www.libsdl.org/)
- Inspired by XFCE's minimalist philosophy
- Created using [OpenCode](https://opencode.ai) / Big Pickle

---

<div align="center">

**Made with ❤️ for the Linux community**

</div>
