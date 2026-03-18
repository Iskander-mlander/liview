# XFCE Light Image Viewer - Especificaciones

## 1. Resumen del Proyecto

**Nombre:** xfce-liview  
**Tipo:** Visor de imágenes minimalista  
**Propósito:** Visor de imágenes ultra-liviano para sistemas con pocos recursos  
**Tecnología:** C++ con SDL2  

## 2. Características Principales

### 2.1 Rendimiento
- Tiempo de inicio: < 100ms
- Carga de imágenes: instantánea
- Memoria: ~20-30MB
- Sin dependencias pesadas (no GTK, no Qt)

### 2.2 Formatos Soportados
- JPEG (.jpg, .jpeg)
- PNG (.png)
- GIF (.gif)
- BMP (.bmp)
- WebP (.webp)

### 2.3 Búsqueda de Imágenes
- Búsqueda recursiva en subdirectorios (por defecto)
- Ordenamiento alfabético

## 3. Interfaz de Usuario

### 3.1 Diseño
- Pantalla principal: visor de imágenes centrado
- Panel izquierdo: lista de thumbnails (opcional, toggle con T)
- Barra de estado inferior: nombre de imagen y posición

### 3.2 Tema Visual
- Fondo oscuro: #1E1E1E
- Thumbnail seleccionado: #4A90D9 (azul)
- Thumbnail normal: #373737 (gris oscuro)

## 4. Controles

| Acción | Atajo |
|--------|-------|
| Siguiente imagen | Rueda abajo / Flecha derecha / Espacio |
| Imagen anterior | Rueda arriba / Flecha izquierda |
| Primera imagen | Home |
| Última imagen | End |
| Pantalla completa | F11 |
| Mostrar/Ocultar thumbnails | T |
| Click en zona izquierda | Imagen anterior |
| Click en zona derecha | Siguiente imagen |
| Salir | Escape |

## 5. Arquitectura

### 5.1 Estructura de Archivos
```
xfce-liview/
├── CMakeLists.txt      # Configuración de compilación
├── src/
│   ├── main.cpp        # Punto de entrada
│   ├── viewer.h        # Cabeceras
│   └── viewer.cpp      # Implementación
└── SPEC.md             # Este archivo
```

### 5.2 Dependencias
- SDL2
- SDL2_image
- C++17

### 5.3 Compilación
```bash
mkdir build && cd build
cmake ..
make
```

## 6. Instalación

```bash
# Ubuntu/Debian
sudo apt install libsdl2-dev libsdl2-image-dev
cmake -B build && cmake --build build
sudo cp build/xfce-liview /usr/local/bin/
```

## 7. Uso

```bash
# Abrir directorio
./xfce-liview ~/Imágenes

# Abrir imagen específica (directorio actual)
/xfce-liview foto.jpg
```

## 8. Limitaciones Conocidas

- Sin soporte para SVG (requeriría librerías adicionales)
- Sin zoom ni pan (futura versión)
- Sin navegación por carpetas en UI (usa sistema de archivos)

## 9. Futuras Mejoras

- [ ] Teclado numérico para navegación rápida
- [ ] Zoom con +/- y scroll del ratón
- [ ] Soporte para más formatos (TIFF, ICO)
- [ ] Atajos configurables
- [ ] Historial de directorios recientes
