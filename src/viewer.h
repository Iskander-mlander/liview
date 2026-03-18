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

#ifndef VIEWER_H
#define VIEWER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <vector>

struct MenuItem {
    std::string label;
    int x, y, w, h;
    int open_submenu;
};

class ImageViewer {
public:
    ImageViewer();
    ~ImageViewer();
    void run(const std::string& path);
    
private:
    void load_dir(const std::string& path);
    void load_img();
    void draw();
    void draw_menu();
    void draw_text(const std::string& text, int x, int y, SDL_Color color, int size);
    void open_folder_dialog();
    void show_about();
    int menu_bar_height() const { return 30; }
    
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Texture* tex;
    SDL_Texture* orig_tex;
    TTF_Font* font;
    TTF_Font* font_small;
    std::vector<std::string> imgs;
    int idx;
    bool full;
    
    std::vector<MenuItem> menu_items;
    int active_menu;
    bool show_about_dialog;
    int mouse_x, mouse_y;
    
    std::string current_lang;
    std::string translate(const std::string& text);
    void set_language(const std::string& lang);
};

#endif
