/*
 * xfce-liview - Lightweight image viewer for XFCE
 * Copyright (C) 2024  Iskander MegaLander, GrijanderousLender
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#include "viewer.h"
#include <iostream>
#include <sys/stat.h>
#include <stdlib.h>

std::string select_folder() {
    FILE* f = popen("zenity --file-selection --directory --title='Seleccionar carpeta de imágenes'", "r");
    if (!f) return "";
    
    char path[1024];
    if (fgets(path, sizeof(path), f)) {
        size_t len = strlen(path);
        if (len > 0 && path[len-1] == '\n') path[len-1] = 0;
        pclose(f);
        return path;
    }
    pclose(f);
    return "";
}

int main(int argc, char* argv[]) {
    std::string path;
    
    if (argc > 1) {
        struct stat st;
        if (stat(argv[1], &st) == 0 && S_ISDIR(st.st_mode)) {
            path = argv[1];
        }
    } else {
        path = select_folder();
        if (path.empty()) return 0;
    }
    
    ImageViewer viewer;
    viewer.run(path);
    return 0;
}
