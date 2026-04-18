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
#include <unistd.h>
#include <string.h>

std::string select_folder() {
    FILE* f = popen("yad --file --directory --title='Seleccionar carpeta'", "r");
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
    std::string single_file;
    bool recursive = true;
    
    if (argc > 1) {
        std::string arg = argv[1];
        
        if (arg == "%f" || arg == "%F" || arg == "%u" || arg == "%U") {
            path = select_folder();
        } else {
            struct stat st;
            if (stat(arg.c_str(), &st) == 0) {
                if (S_ISDIR(st.st_mode)) {
                    path = arg;
                } else if (S_ISREG(st.st_mode)) {
                    single_file = arg;
                    size_t last_slash = single_file.find_last_of('/');
                    path = (last_slash != std::string::npos) ? single_file.substr(0, last_slash) : ".";
                }
            } else {
                path = select_folder();
            }
        }
    } else {
        path = select_folder();
    }
    
    if (path.empty() && single_file.empty()) {
        return 0;
    }
    
    ImageViewer viewer;
    viewer.run(path, single_file, recursive);
    return 0;
}
