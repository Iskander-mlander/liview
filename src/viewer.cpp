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
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <algorithm>
#include <map>
#include <sys/stat.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>

namespace {
    bool is_image(const std::string& f) {
        std::string e = f.size() > 4 ? f.substr(f.size()-4) : "";
        std::transform(e.begin(), e.end(), e.begin(), ::tolower);
        return e == ".jpg" || e == ".png" || e == ".gif" || e == ".bmp" 
            || f.size() > 5 && (f.substr(f.size()-5) == ".jpeg" || f.substr(f.size()-5) == ".webp");
    }
    
    void scan(const std::string& p, std::vector<std::string>& v, bool r) {
        DIR* d = opendir(p.c_str());
        if (!d) return;
        struct dirent* ent;
        while ((ent = readdir(d))) {
            std::string n = ent->d_name;
            if (n == "." || n == "..") continue;
            std::string fp = p + "/" + n;
            struct stat st;
            if (stat(fp.c_str(), &st) == 0 && S_ISDIR(st.st_mode) && r) {
                scan(fp, v, r);
            } else if (is_image(n)) {
                v.push_back(fp);
            }
        }
        closedir(d);
    }
    
    std::string run_zenity(const std::string& cmd) {
        FILE* f = popen(cmd.c_str(), "r");
        if (!f) return "";
        char buf[1024];
        std::string result;
        while (fgets(buf, sizeof(buf), f)) result += buf;
        pclose(f);
        if (!result.empty() && result.back() == '\n') result.pop_back();
        return result;
    }
}

ImageViewer::ImageViewer() : win(nullptr), ren(nullptr), idx(0), full(false), 
    tex(nullptr), orig_tex(nullptr), font(nullptr), font_small(nullptr), 
    active_menu(-1), show_about_dialog(false), mouse_x(0), mouse_y(0), current_lang("es") {
    
    const char* lang_env = getenv("LANG");
    if (lang_env) {
        std::string lang = lang_env;
        if (lang.find("es") == 0) current_lang = "es";
        else if (lang.find("en") == 0) current_lang = "en";
        else if (lang.find("de") == 0) current_lang = "de";
        else if (lang.find("fr") == 0) current_lang = "fr";
        else if (lang.find("el") == 0) current_lang = "el";
        else if (lang.find("hi") == 0) current_lang = "hi";
        else if (lang.find("id") == 0) current_lang = "id";
        else if (lang.find("it") == 0) current_lang = "it";
        else if (lang.find("ja") == 0) current_lang = "ja";
        else if (lang.find("ko") == 0) current_lang = "ko";
        else if (lang.find("fa") == 0) current_lang = "fa";
        else if (lang.find("pl") == 0) current_lang = "pl";
        else if (lang.find("pt") == 0) current_lang = "pt";
        else if (lang.find("ru") == 0) current_lang = "ru";
        else if (lang.find("tr") == 0) current_lang = "tr";
        else if (lang.find("ur") == 0) current_lang = "ur";
        else if (lang.find("vi") == 0) current_lang = "vi";
        else if (lang.find("ar") == 0) current_lang = "ar";
        else if (lang.find("zh") == 0) current_lang = "zh";
        else current_lang = "en";
    }
}

void ImageViewer::set_language(const std::string& lang) {
    current_lang = lang;
}

std::string ImageViewer::translate(const std::string& text) {
    static std::map<std::string, std::map<std::string, std::string>> dict = {
        {"es", {
            {"Archivo", "Archivo"}, {"Ver", "Ver"}, {"Ayuda", "Ayuda"},
            {"Abrir carpeta...", "Abrir carpeta..."}, {"Cerrar", "Cerrar"},
            {"Pantalla completa", "Pantalla completa"}, {"Ventana", "Ventana"},
            {"Ajustar", "Ajustar"}, {"Rellenar", "Rellenar"}, {"Estirar", "Estirar"}, {"Original", "Original"},
            {"Zoom +", "Zoom +"}, {"Zoom -", "Zoom -"}, {"Tamano original", "Tamano original"},
            {"Thumbnails: ON", "Miniaturas: SI"}, {"Thumbnails: OFF", "Miniaturas: NO"},
            {"Acerca de...", "Acerca de..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Version 1.0.0"},
            {"Objetivo:", "Objetivo:"},
            {"Visor ultra-liviano para sistemas", "Visor ultra-liviano para sistemas"},
            {"con pocos recursos.", "con pocos recursos."},
            {"Dependencias:", "Dependencias:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Autor:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Hecho en OpenCode/Big Pickle"},
            {"Click para cerrar", "Pulse para cerrar"},
            {"Seleccionar carpeta", "Seleccionar carpeta"},
            {"Lenguaje", "Idioma"}
        }},
        {"en", {
            {"Archivo", "File"}, {"Ver", "View"}, {"Ayuda", "Help"},
            {"Abrir carpeta...", "Open folder..."}, {"Cerrar", "Close"},
            {"Pantalla completa", "Fullscreen"}, {"Ventana", "Window"},
            {"Ajustar", "Fit"}, {"Rellenar", "Fill"}, {"Estirar", "Stretch"}, {"Original", "Original"},
            {"Zoom +", "Zoom +"}, {"Zoom -", "Zoom -"}, {"Tamano original", "Original size"},
            {"Thumbnails: ON", "Thumbnails: ON"}, {"Thumbnails: OFF", "Thumbnails: OFF"},
            {"Acerca de...", "About..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Version 1.0.0"},
            {"Objetivo:", "Purpose:"},
            {"Visor ultra-liviano para sistemas", "Ultra-lightweight viewer for systems"},
            {"con pocos recursos.", "with few resources."},
            {"Dependencias:", "Dependencies:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Author:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Made in OpenCode/Big Pickle"},
            {"Click para cerrar", "Click to close"},
            {"Seleccionar carpeta", "Select folder"},
            {"Lenguaje", "Language"}
        }},
        {"de", {
            {"Archivo", "Datei"}, {"Ver", "Ansicht"}, {"Ayuda", "Hilfe"},
            {"Abrir carpeta...", "Ordner offnen..."}, {"Cerrar", "Schliessen"},
            {"Pantalla completa", "Vollbild"}, {"Ventana", "Fenster"},
            {"Ajustar", "Anpassen"}, {"Rellenar", "Fullen"}, {"Estirar", "Strecken"}, {"Original", "Original"},
            {"Zoom +", "Zoom +"}, {"Zoom -", "Zoom -"}, {"Tamano original", "Originalgrosse"},
            {"Thumbnails: ON", "Miniaturansicht: AN"}, {"Thumbnails: OFF", "Miniaturansicht: AUS"},
            {"Acerca de...", "Uber..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Version 1.0.0"},
            {"Objetivo:", "Zweck:"},
            {"Visor ultra-liviano para sistemas", "Ultraleichtes Bildbetrachtungsprogramm"},
            {"con pocos recursos.", "fur Systeme mit wenig Ressourcen."},
            {"Dependencias:", "Abhangigkeiten:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Autor:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Erstellt in OpenCode/Big Pickle"},
            {"Click para cerrar", "Klicken zum Schliessen"},
            {"Seleccionar carpeta", "Ordner auswahlen"},
            {"Lenguaje", "Sprache"}
        }},
        {"fr", {
            {"Archivo", "Fichier"}, {"Ver", "Affichage"}, {"Ayuda", "Aide"},
            {"Abrir carpeta...", "Ouvrir un dossier..."}, {"Cerrar", "Fermer"},
            {"Pantalla completa", "Plein ecran"}, {"Ventana", "Fenetre"},
            {"Ajustar", "Ajuster"}, {"Rellenar", "Remplir"}, {"Estirar", "Etirer"}, {"Original", "Original"},
            {"Zoom +", "Zoom +"}, {"Zoom -", "Zoom -"}, {"Tamano original", "Taille originale"},
            {"Thumbnails: ON", "Vignettes: OUI"}, {"Thumbnails: OFF", "Vignettes: NON"},
            {"Acerca de...", "A propos..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Version 1.0.0"},
            {"Objetivo:", "Objectif:"},
            {"Visor ultra-liviano para sistemas", "Visionneuse ultra-legere pour systemes"},
            {"con pocos recursos.", "a ressources limitees."},
            {"Dependencias:", "Dependances:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Auteur:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Fait dans OpenCode/Big Pickle"},
            {"Click para cerrar", "Cliquez pour fermer"},
            {"Seleccionar carpeta", "Selectionner un dossier"},
            {"Lenguaje", "Langue"}
        }},
        {"it", {
            {"Archivo", "File"}, {"Ver", "Visualizza"}, {"Ayuda", "Aiuto"},
            {"Abrir carpeta...", "Apri cartella..."}, {"Cerrar", "Chiudi"},
            {"Pantalla completa", "Schermo intero"}, {"Ventana", "Finestra"},
            {"Ajustar", "Adatta"}, {"Rellenar", "Riempi"}, {"Estirar", "Allunga"}, {"Original", "Originale"},
            {"Zoom +", "Zoom +"}, {"Zoom -", "Zoom -"}, {"Tamano original", "Dimensione originale"},
            {"Thumbnails: ON", "Miniature: SI"}, {"Thumbnails: OFF", "Miniature: NO"},
            {"Acerca de...", "Informazioni..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Versione 1.0.0"},
            {"Objetivo:", "Obbiettivo:"},
            {"Visor ultra-liviano para sistemas", "Visualizzatore ultra-leggero per sistemi"},
            {"con pocos recursos.", "a risorse limitate."},
            {"Dependencias:", "Dipendenze:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Autore:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Creato in OpenCode/Big Pickle"},
            {"Click para cerrar", "Clicca per chiudere"},
            {"Seleccionar carpeta", "Seleziona cartella"},
            {"Lenguaje", "Lingua"}
        }},
        {"pt", {
            {"Archivo", "Arquivo"}, {"Ver", "Ver"}, {"Ayuda", "Ajuda"},
            {"Abrir carpeta...", "Abrir pasta..."}, {"Cerrar", "Fechar"},
            {"Pantalla completa", "Tela cheia"}, {"Ventana", "Janela"},
            {"Ajustar", "Ajustar"}, {"Rellenar", "Preencher"}, {"Estirar", "Esticar"}, {"Original", "Original"},
            {"Zoom +", "Zoom +"}, {"Zoom -", "Zoom -"}, {"Tamano original", "Tamanho original"},
            {"Thumbnails: ON", "Miniaturas: SIM"}, {"Thumbnails: OFF", "Miniaturas: NAO"},
            {"Acerca de...", "Sobre..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Versao 1.0.0"},
            {"Objetivo:", "Objetivo:"},
            {"Visor ultra-liviano para sistemas", "Visualizador ultraleve para sistemas"},
            {"con pocos recursos.", "com poucos recursos."},
            {"Dependencias:", "Dependencias:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Autor:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Feito em OpenCode/Big Pickle"},
            {"Click para cerrar", "Clique para fechar"},
            {"Seleccionar carpeta", "Selecionar pasta"},
            {"Lenguaje", "Idioma"}
        }},
        {"ru", {
            {"Archivo", "Fayl"}, {"Ver", "Vid"}, {"Ayuda", "Pomoshch"},
            {"Abrir carpeta...", "Otkryt papku..."}, {"Cerrar", "Zakryt"},
            {"Pantalla completa", "Polnyy ekran"}, {"Ventana", "Okno"},
            {"Ajustar", "Razmer"}, {"Rellenar", "Zapolnit"}, {"Estirar", "Rastyanut"}, {"Original", "Original"},
            {"Zoom +", "Uvelichit"}, {"Zoom -", "Umenshit"}, {"Tamano original", "Oshibka razmer"},
            {"Thumbnails: ON", "Miniatyury: VKLYUCHENO"}, {"Thumbnails: OFF", "Miniatyury: VYKLYUCHENO"},
            {"Acerca de...", "O programme..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Versiya 1.0.0"},
            {"Objetivo:", "Tsel:"},
            {"Visor ultra-liviano para sistemas", "Ultralegkiy prosmotrshchik dlya sistem"},
            {"con pocos recursos.", "s ogranichennymi resursami."},
            {"Dependencias:", "Zavisimosti:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Avtor:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Sozdano v OpenCode/Big Pickle"},
            {"Click para cerrar", "Nazhmite dlya zakrytiya"},
            {"Seleccionar carpeta", "Vybrat papku"},
            {"Lenguaje", "Yazyk"}
        }},
        {"zh", {
            {"Archivo", "Wenjian"}, {"Ver", "Chakan"}, {"Ayuda", "Bangzhu"},
            {"Abrir carpeta...", "Dakai wenjianjia..."}, {"Cerrar", "Guanbi"},
            {"Pantalla completa", "Quanping"}, {"Ventana", "Chuangkou"},
            {"Ajustar", "Peihe"}, {"Rellenar", "Tianchong"}, {"Estirar", "La Sheng"}, {"Original", "Yuanshi"},
            {"Zoom +", "Fangda"}, {"Zoom -", "Suoxiao"}, {"Tamano original", "Yuanshi daxiao"},
            {"Thumbnails: ON", "Suolue tu: Kai"}, {"Thumbnails: OFF", "Suolue tu: Guan"},
            {"Acerca de...", "Guanyu..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Banben 1.0.0"},
            {"Objetivo:", "Mudi:"},
            {"Visor ultra-liviano para sistemas", "Weixing Tupu Guan Kan Qi"},
            {"con pocos recursos.", "Yong yu Zi Yuan You Xian De Xi Tong."},
            {"Dependencias:", "Yilai:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Zuozhe:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Zai OpenCode/Big Pickle Zhong Chuang Jian"},
            {"Click para cerrar", "Dianji Guanbi"},
            {"Seleccionar carpeta", "Xuanze Wenjianjia"},
            {"Lenguaje", "Yuyan"}
        }},
        {"ja", {
            {"Archivo", "Fairu"}, {"Ver", "Hyouji"}, {"Ayuda", "Herupu"},
            {"Abrir carpeta...", "Fairu wo Hiraku..."}, {"Cerrar", "Tojiru"},
            {"Pantalla completa", "Full Zenransu"}, {"Ventana", "Windou"},
            {"Ajustar", "Tekiyou"}, {"Rellenar", "Mi-ruke"}, {"Estirar", "Nobasu"}, {"Original", "Genzai"},
            {"Zoom +", "Kakudai"}, {"Zoom -", "Shukushou"}, {"Tamano original", "Oridjinaru Saizu"},
            {"Thumbnails: ON", "Sumatoppu: On"}, {"Thumbnails: OFF", "Sumatoppu: Ofu"},
            {"Acerca de...", "Kono Appu ni Tsuite..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Baajon 1.0.0"},
            {"Objetivo:", "Mokuteki:"},
            {"Visor ultra-liviano para sistemas", "Sourei Kan Kanransu"},
            {"con pocos recursos.", "Sousei no Sisutemu no Tame no Aplied."},
            {"Dependencias:", "Iraneki:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Sakusei:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "OpenCode/Big Pickle de Sakusei"},
            {"Click para cerrar", "Kuritto de Tojiru"},
            {"Seleccionar carpeta", "Fairu wo Erabu"},
            {"Lenguaje", "Gengo"}
        }},
        {"ko", {
            {"Archivo", "Faireu"}, {"Ver", "Boan"}, {"Ayuda", "Deowoon"},
            {"Abrir carpeta...", "Namgyeong Moklok..."}, {"Cerrar", "Dongkyung"},
            {"Pantalla completa", "Jeonhyeong"}, {"Ventana", "Windeo"},
            {"Ajustar", "Jalyeo"}, {"Rellenar", "Mokrok"}, {"Estirar", "Deokgidae"}, {"Original", "Wonhon"},
            {"Zoom +", "Daehyeong"}, {"Zoom -", "Soehyeong"}, {"Tamano original", "Wonhon Keukjing"},
            {"Thumbnails: ON", "Seomyeong: Ye"}, {"Thumbnails: OFF", "Seomyeong: Aniyo"},
            {"Acerca de...", "Yeok...-e Daehayeo..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Beojeon 1.0.0"},
            {"Objetivo:", "Mokjeok:"},
            {"Visor ultra-liviano para sistemas", "Chaegyeong Sagan Pimyeon Gag-ui Ttaemune"},
            {"con pocos recursos.", "Jahyeog-i Huhan keigon."},
            {"Dependencias:", "Junbae:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Jayeongja:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "OpenCode/Big Pickle-eulo Baekseong"},
            {"Click para cerrar", "Klikkeul Wihan Dongkyung"},
            {"Seleccionar carpeta", "Namgyeong-eul Seontaek"},
            {"Lenguaje", "Eoneo"}
        }},
        {"ar", {
            {"Archivo", "Waraqa"}, {"Ver", "Marrara"}, {"Ayuda", "Musaada"},
            {"Abrir carpeta...", "Iftah al-majmu'a..."}, {"Cerrar", "Ighlaq"},
            {"Pantalla completa", "Shasha Kamila"}, {"Ventana", "Nafidha"},
            {"Acerca de...", "Hawiiliyat..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Al-nasr 1.0.0"},
            {"Objetivo:", "Al-hadaf:"},
            {"Visor ultra-liviano para sistemas", "Muarir Suwar Khafif Li Nizam"},
            {"con pocos recursos.", "laa yamlik ma Shawq al-muaddiyat."},
            {"Dependencias:", "Al-tawatturat:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Mualsif:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Suni'a fi OpenCode/Big Pickle"},
            {"Click para cerrar", "Iklak le al-ighlaq"},
            {"Seleccionar carpeta", "Ikhtar al-majmu'a"},
            {"Lenguaje", "Al-Lugha"},
            {"Licencia:", "Al-Rukhas"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"tr", {
            {"Archivo", "Dosya"}, {"Ver", "Gorunum"}, {"Ayuda", "Yardim"},
            {"Abrir carpeta...", "Klasoru ac..."}, {"Cerrar", "Kapat"},
            {"Pantalla completa", "Tam ekran"}, {"Ventana", "Pencere"},
            {"Acerca de...", "Hakkinda..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Surum 1.0.0"},
            {"Objetivo:", "Ama:"},
            {"Visor ultra-liviano para sistemas", "Az kaynakli sistemler icin hafif goruntuleyici"},
            {"con pocos recursos.", "Kaynaklarin az oldugu sistemler icin."},
            {"Dependencias:", "Bagimliliklar:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Yazar:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "OpenCode/Big Pickle'de Yapildi"},
            {"Click para cerrar", "Kapatmak icin tiklayin"},
            {"Seleccionar carpeta", "Klasor secin"},
            {"Lenguaje", "Dil"},
            {"Licencia:", "Lisans:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"el", {
            {"Archivo", "Arxeio"}, {"Ver", "Provoli"}, {"Ayuda", "Boitheia"},
            {"Abrir carpeta...", "Anoigma fakelou..."}, {"Cerrar", "Kleisimo"},
            {"Pantalla completa", "Pliris othoni"}, {"Ventana", "Parathiro"},
            {"Acerca de...", "Peri..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Ekdosi 1.0.0"},
            {"Objetivo:", "Skopos:"},
            {"Visor ultra-liviano para sistemas", "Elan anagnoristikos gia susthma me ligous porcous"},
            {"con pocos recursos.", "Me ligous porcous."},
            {"Dependencias:", "Exartiseis:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Sunthetis:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Fthiachtike sto OpenCode/Big Pickle"},
            {"Click para cerrar", "Pieste giakleisimo"},
            {"Seleccionar carpeta", "Epilogi fakelou"},
            {"Lenguaje", "Glossa"},
            {"Licencia:", "Adeia:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"hi", {
            {"Archivo", "Prasaran"}, {"Ver", "Dekan"}, {"Ayuda", "Madad"},
            {"Abrir carpeta...", "Foldar khole..."}, {"Cerrar", "Band kare"},
            {"Pantalla completa", "Poorn pardhansh"}, {"Ventana", "Khirki"},
            {"Acerca de...", "Bare mein..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Sanskaran 1.0.0"},
            {"Objetivo:", "Lakshya:"},
            {"Visor ultra-liviano para sistemas", "Kam sansadhanon vale pancho ke liye halka prakriya"},
            {"con pocos recursos.", "Kam sansadhanon vale systemon ke liye."},
            {"Dependencias:", "Apekhiyan:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Lekhak:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "OpenCode/Big Pickle mein banaya gaya"},
            {"Click para cerrar", "Band karne ke liye click kare"},
            {"Seleccionar carpeta", "Foldar chunen"},
            {"Lenguaje", "Bhasha"},
            {"Licencia:", "Lainses:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"id", {
            {"Archivo", "Berkas"}, {"Ver", "Lihat"}, {"Ayuda", "Bantuan"},
            {"Abrir carpeta...", "Buka folder..."}, {"Cerrar", "Tutup"},
            {"Pantalla completa", "Layar penuh"}, {"Ventana", "Jendela"},
            {"Acerca de...", "Tentang..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Versi 1.0.0"},
            {"Objetivo:", "Tujuan:"},
            {"Visor ultra-liviano para sistemas", "Penampil gambar ringan untuk sistem dengan sumber daya terbatas"},
            {"con pocos recursos.", "Dengan sumber daya terbatas."},
            {"Dependencias:", "Ketergantungan:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Pembuat:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Dibuat di OpenCode/Big Pickle"},
            {"Click para cerrar", "Klik untuk menutup"},
            {"Seleccionar carpeta", "Pilih folder"},
            {"Lenguaje", "Bahasa"},
            {"Licencia:", "Lisensi:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"fa", {
            {"Archivo", "Payvast"}, {"Ver", "Didan"}, {"Ayuda", "Komak"},
            {"Abrir carpeta...", "Kollab ro bokon..."}, {"Cerrar", "Bastan"},
            {"Pantalla completa", "Tamamiye safhe"}, {"Ventana", "Panjereh"},
            {"Acerca de...", "Dar morede..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Noskhe 1.0.0"},
            {"Objetivo:", "Hadaf:"},
            {"Visor ultra-liviano para sistemas", "Barnameye enteqari baraye sistem ba manbae mahdod"},
            {"con pocos recursos.", "Ba manbae mahdod."},
            {"Dependencias:", "Virayeshkhane:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Nehvand:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Dar OpenCode/Big Pickle sazide shode"},
            {"Click para cerrar", "Baraye bastan click konid"},
            {"Seleccionar carpeta", "Folder ra entekhab konid"},
            {"Lenguaje", "Zaban"},
            {"Licencia:", "Mozariat:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"pl", {
            {"Archivo", "Plik"}, {"Ver", "Widok"}, {"Ayuda", "Pomoc"},
            {"Abrir carpeta...", "Otworz folder..."}, {"Cerrar", "Zamknij"},
            {"Pantalla completa", "Pelny ekran"}, {"Ventana", "Okno"},
            {"Acerca de...", "O programie..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Wersja 1.0.0"},
            {"Objetivo:", "Cel:"},
            {"Visor ultra-liviano para sistemas", "Lekki podgladacz obrazow dla systemow z malymi zasobami"},
            {"con pocos recursos.", "Z malymi zasobami."},
            {"Dependencias:", "Zaleznosci:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Autor:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Wykonano w OpenCode/Big Pickle"},
            {"Click para cerrar", "Kliknij, aby zamknac"},
            {"Seleccionar carpeta", "Wybierz folder"},
            {"Lenguaje", "Jezyk"},
            {"Licencia:", "Licencja:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"ur", {
            {"Archivo", "Fayl"}, {"Ver", "Dekho"}, {"Ayuda", "Madad"},
            {"Abrir carpeta...", "Foldar kholo..."}, {"Cerrar", "Band karo"},
            {"Pantalla completa", "Poora screen"}, {"Ventana", "Khirki"},
            {"Acerca de...", "Bare mein..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Version 1.0.0"},
            {"Objetivo:", "Maqsad:"},
            {"Visor ultra-liviano para sistemas", "Kamzor alaton walon ke liye halka tasweer dekhnay wala"},
            {"con pocos recursos.", "Kamzor alaton ke liye."},
            {"Dependencias:", "Taki:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Musannif:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "OpenCode/Big Pickle mein banaya gaya"},
            {"Click para cerrar", "Band karne ke liye click karo"},
            {"Seleccionar carpeta", "Foldar chuno"},
            {"Lenguaje", "Zaban"},
            {"Licencia:", "Ijazat:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }},
        {"vi", {
            {"Archivo", "Tap tin"}, {"Ver", "Xem"}, {"Ayuda", "Tro giup"},
            {"Abrir carpeta...", "Mo thu muc..."}, {"Cerrar", "Dong"},
            {"Pantalla completa", "Toan man hinh"}, {"Ventana", "Cuo so"},
            {"Acerca de...", "Ve..."},
            {"XFCE Light Image Viewer", "XFCE Light Image Viewer"},
            {"Version 1.0.0", "Phien ban 1.0.0"},
            {"Objetivo:", "Muc tieu:"},
            {"Visor ultra-liviano para sistemas", "Trinh xem hinh anh nhe cho he thong co it tai nguyen"},
            {"con pocos recursos.", "Co it tai nguyen."},
            {"Dependencias:", "Phu thuoc:"},
            {"SDL2, SDL_image, SDL_ttf", "SDL2, SDL_image, SDL_ttf"},
            {"Autor:", "Tac gia:"},
            {"Iskander MegaLander", "Iskander MegaLander"},
            {"GrijanderousLender", "GrijanderousLender"},
            {"Made in OpenCode/Big Pickle", "Duoc tao trong OpenCode/Big Pickle"},
            {"Click para cerrar", "Nhan de dong"},
            {"Seleccionar carpeta", "Chon thu muc"},
            {"Lenguaje", "Ngon ngu"},
            {"Licencia:", "Giay phep:"},
            {"GNU GPL v3+", "GNU GPL v3+"}
        }}
    };
    
    if (dict.count(current_lang) && dict[current_lang].count(text)) {
        return dict[current_lang][text];
    }
    return text;
}

ImageViewer::~ImageViewer() {
    if (tex) SDL_DestroyTexture(tex);
    if (orig_tex) SDL_DestroyTexture(orig_tex);
    if (font) TTF_CloseFont(font);
    if (font_small) TTF_CloseFont(font_small);
    TTF_Quit();
    if (ren) SDL_DestroyRenderer(ren);
    if (win) SDL_DestroyWindow(win);
    SDL_Quit();
}

void ImageViewer::draw_text(const std::string& text, int x, int y, SDL_Color color, int size) {
    if (size == 12) {
        if (!font_small) return;
        SDL_Surface* s = TTF_RenderUTF8_Blended(font_small, text.c_str(), color);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(ren, s);
            SDL_Rect r = {x, y, s->w, s->h};
            SDL_RenderCopy(ren, t, nullptr, &r);
            SDL_DestroyTexture(t);
            SDL_FreeSurface(s);
        }
    } else {
        if (!font) return;
        SDL_Surface* s = TTF_RenderUTF8_Blended(font, text.c_str(), color);
        if (s) {
            SDL_Texture* t = SDL_CreateTextureFromSurface(ren, s);
            SDL_Rect r = {x, y, s->w, s->h};
            SDL_RenderCopy(ren, t, nullptr, &r);
            SDL_DestroyTexture(t);
            SDL_FreeSurface(s);
        }
    }
}

void ImageViewer::load_dir(const std::string& p) {
    imgs.clear();
    if (tex) { SDL_DestroyTexture(tex); tex = nullptr; }
    if (orig_tex) { SDL_DestroyTexture(orig_tex); orig_tex = nullptr; }
    scan(p, imgs, true);
    std::sort(imgs.begin(), imgs.end(), [](auto& a, auto& b){ return a < b; });
    if (!imgs.empty()) load_img();
}

void ImageViewer::load_img() {
    if (imgs.empty() || idx < 0) return;
    if (idx >= (int)imgs.size()) idx = 0;
    if (tex) { SDL_DestroyTexture(tex); tex = nullptr; }
    if (orig_tex) { SDL_DestroyTexture(orig_tex); orig_tex = nullptr; }
    
    SDL_Surface* s = IMG_Load(imgs[idx].c_str());
    if (s) {
        int win_w = 1200, win_h = 700;
        SDL_GetWindowSize(win, &win_w, &win_h);
        int img_w = s->w;
        int img_h = s->h;
        
        orig_tex = SDL_CreateTextureFromSurface(ren, s);
        tex = orig_tex;
        SDL_FreeSurface(s);
    }
}

void ImageViewer::open_folder_dialog() {
    SDL_MinimizeWindow(win);
    SDL_Delay(100);
    std::string path = run_zenity("zenity --file-selection --directory --title='" + translate("Seleccionar carpeta") + "'");
    SDL_RestoreWindow(win);
    if (!path.empty()) load_dir(path);
}

void ImageViewer::show_about() {
    show_about_dialog = true;
}

void ImageViewer::draw_menu() {
    int w, h;
    SDL_GetWindowSize(win, &w, &h);
    int mh = menu_bar_height();
    
    SDL_SetRenderDrawColor(ren, 50, 50, 50, 255);
    SDL_Rect mb = {0, 0, w, mh};
    SDL_RenderFillRect(ren, &mb);
    
    SDL_SetRenderDrawColor(ren, 70, 70, 70, 255);
    SDL_RenderDrawLine(ren, 0, mh, w, mh);
    
    SDL_Color white = {220, 220, 220, 255};
    SDL_Color blue = {74, 144, 217, 255};
    
    std::vector<std::string> items = {translate("Archivo"), translate("Ver"), translate("Ayuda")};
    
    int x = 10;
    menu_items.clear();
    
    for (size_t i = 0; i < items.size(); i++) {
        int item_w = items[i].size() * 8 + 20;
        MenuItem mi = {items[i], x, 5, item_w, mh - 10};
        menu_items.push_back(mi);
        
        if (mouse_x >= x && mouse_x < x + item_w && mouse_y >= 5 && mouse_y < mh) {
            SDL_SetRenderDrawColor(ren, blue.r, blue.g, blue.b, 255);
            SDL_Rect hr = {x, 5, item_w, mh - 5};
            SDL_RenderFillRect(ren, &hr);
            draw_text(items[i], x + 10, 8, white, 14);
        } else {
            draw_text(items[i], x + 10, 8, white, 14);
        }
        
        x += item_w + 5;
    }
}

void ImageViewer::draw() {
    int w = 1200, h = 700;
    SDL_GetWindowSize(win, &w, &h);
    int mh = menu_bar_height();
    
    SDL_SetRenderDrawColor(ren, 30, 30, 30, 255);
    SDL_RenderClear(ren);
    
    if (tex) {
        int tw, th;
        SDL_QueryTexture(tex, nullptr, nullptr, &tw, &th);
        
        int img_x = 0, img_y = mh, img_w = w, img_h = h - mh;
        
        int draw_w = tw;
        int draw_h = th;
        if (tw > 0 && th > 0) {
            double scale_x = (double)img_w / tw;
            double scale_y = (double)img_h / th;
            double scale = std::min(scale_x, scale_y);
            if (scale < 1.0) {
                draw_w = (int)(tw * scale);
                draw_h = (int)(th * scale);
            }
        }
        int draw_x = img_x + (img_w - draw_w) / 2;
        int draw_y = img_y + (img_h - draw_h) / 2;
        
        SDL_Rect src = {0, 0, tw, th};
        SDL_Rect dst = {draw_x, draw_y, draw_w, draw_h};
        SDL_RenderCopy(ren, tex, &src, &dst);
    }
    
    draw_menu();
    
    if (!imgs.empty() && idx >= 0 && idx < (int)imgs.size()) {
        SDL_SetRenderDrawColor(ren, 35, 35, 35, 255);
        SDL_Rect sb = {0, h - 30, w, 30};
        SDL_RenderFillRect(ren, &sb);
        
        std::string name = imgs[idx].substr(imgs[idx].find_last_of('/') + 1);
        std::string info = name + " (" + std::to_string(idx + 1) + "/" + std::to_string(imgs.size()) + ")";
        SDL_Color gray = {180, 180, 180, 255};
        draw_text(info, 10, h - 22, gray, 12);
    }
    
    if (active_menu >= 0) {
        std::vector<std::string> items;
        if (active_menu == 0) items = {translate("Abrir carpeta..."), "", translate("Cerrar")};
        else if (active_menu == 1) items = {translate("Pantalla completa"), translate("Ventana")};
        else if (active_menu == 2) items = {translate("Acerca de..."), "", translate("Lenguaje")};
        
        int mh_bar = menu_bar_height();
        int drop_w = 180;
        int drop_h = items.size() * 24 + 10;
        
        SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
        SDL_Rect dd = {menu_items[active_menu].x, mh_bar, drop_w, drop_h};
        SDL_RenderFillRect(ren, &dd);
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        SDL_RenderDrawRect(ren, &dd);
        
        SDL_Color white = {220, 220, 220, 255};
        SDL_Color gray = {150, 150, 150, 255};
        int sx = menu_items[active_menu].x + 10;
        int sy = mh_bar + 8;
        
        for (size_t i = 0; i < items.size(); i++) {
            if (items[i].empty()) {
                SDL_SetRenderDrawColor(ren, 80, 80, 80, 255);
                SDL_RenderDrawLine(ren, sx - 5, sy + 8, sx + drop_w - 10, sy + 8);
                sy += 16;
                continue;
            }
            
            if (mouse_x >= sx - 5 && mouse_x < sx + drop_w - 10 && 
                mouse_y >= sy && mouse_y < sy + 20) {
                SDL_SetRenderDrawColor(ren, 74, 144, 217, 255);
                SDL_Rect hi = {sx - 5, sy, drop_w - 5, 20};
                SDL_RenderFillRect(ren, &hi);
            }
            
            draw_text(items[i], sx, sy + 2, white, 14);
            sy += 24;
        }
    }
    
    if (show_about_dialog) {
        int about_w = 600;
        int about_h = 400;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 200);
        SDL_Rect overlay = {w/2 - about_w/2, h/2 - about_h/2, about_w, about_h};
        SDL_RenderFillRect(ren, &overlay);
        
        SDL_SetRenderDrawColor(ren, 60, 60, 60, 255);
        SDL_Rect box = {w/2 - about_w/2 + 5, h/2 - about_h/2 + 5, about_w - 10, about_h - 10};
        SDL_RenderFillRect(ren, &box);
        SDL_SetRenderDrawColor(ren, 100, 100, 100, 255);
        SDL_RenderDrawRect(ren, &box);
        
        int icon_frame_x = w/2 - about_w/2 + 20;
        int icon_frame_y = h/2 - about_h/2 + 20;
        SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
        SDL_Rect icon_frame = {icon_frame_x, icon_frame_y, 220, 200};
        SDL_RenderFillRect(ren, &icon_frame);
        
        SDL_Surface* icon_surf = IMG_Load("/usr/share/pixmaps/xfce-liview.png");
        if (!icon_surf) icon_surf = IMG_Load("liview-ico.png");
        SDL_Texture* icon_tex = nullptr;
        if (icon_surf) {
            icon_tex = SDL_CreateTextureFromSurface(ren, icon_surf);
            SDL_Rect icon_dst = {icon_frame_x + 35, icon_frame_y + 20, 150, 160};
            SDL_RenderCopy(ren, icon_tex, nullptr, &icon_dst);
            SDL_FreeSurface(icon_surf);
        }
        
        SDL_Color white = {255, 255, 255, 255};
        SDL_Color blue = {74, 144, 217, 255};
        SDL_Color gray = {180, 180, 180, 255};
        
        int text_x = w/2;
        int text_y_base = h/2 - about_h/2 + 50;
        
        draw_text(translate("XFCE Light Image Viewer"), text_x, text_y_base, blue, 18);
        draw_text(translate("Version 1.0.0"), text_x, text_y_base + 30, gray, 12);
        draw_text(translate("Objetivo:"), text_x, text_y_base + 60, white, 14);
        draw_text(translate("Visor ultra-liviano para sistemas"), text_x, text_y_base + 80, gray, 12);
        draw_text(translate("con pocos recursos."), text_x, text_y_base + 95, gray, 12);
        draw_text(translate("Dependencias:"), text_x, text_y_base + 125, white, 14);
        draw_text(translate("SDL2, SDL_image, SDL_ttf"), text_x, text_y_base + 145, gray, 12);
        draw_text(translate("Autor:"), text_x, text_y_base + 175, white, 14);
        draw_text(translate("Iskander MegaLander"), text_x, text_y_base + 195, gray, 12);
        draw_text(translate("GrijanderousLender"), text_x, text_y_base + 210, gray, 12);
        draw_text(translate("Licencia:"), text_x, text_y_base + 235, white, 14);
        draw_text("GNU GPL v3+", text_x, text_y_base + 255, gray, 12);
        draw_text(translate("Made in OpenCode/Big Pickle"), text_x, text_y_base + 275, gray, 12);
        
        draw_text("[ " + translate("Click para cerrar") + " ]", w/2 - 65, h/2 + about_h/2 - 30, gray, 12);
        
        if (icon_tex) SDL_DestroyTexture(icon_tex);
    }
    
    SDL_RenderPresent(ren);
}

void ImageViewer::run(const std::string& path) {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG);
    
    font = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 14);
    font_small = TTF_OpenFont("/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf", 12);
    
    if (!font) {
        font = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 14);
        font_small = TTF_OpenFont("/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf", 12);
    }
    
    win = SDL_CreateWindow("XFCE Light Image Viewer", 100, 100, 1200, 700, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    load_dir(path);
    
    bool running = true;
    while (running) {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            mouse_x = e.motion.x;
            mouse_y = e.motion.y;
            
            if (e.type == SDL_QUIT) running = false;
            
            if (e.type == SDL_KEYDOWN) {
                switch (e.key.keysym.sym) {
                    case SDLK_ESCAPE: 
                        if (full) { full = false; SDL_SetWindowFullscreen(win, 0); }
                        else if (active_menu >= 0) active_menu = -1;
                        else if (show_about_dialog) show_about_dialog = false;
                        else running = false; 
                        break;
                    case SDLK_F11: 
                        full = !full;
                        SDL_SetWindowFullscreen(win, full ? SDL_WINDOW_FULLSCREEN_DESKTOP : 0);
                        break;
                    case SDLK_LEFT: case SDLK_UP: case SDLK_PAGEUP:
                        if (!imgs.empty() && active_menu < 0) { idx = (idx - 1 + imgs.size()) % imgs.size(); load_img(); }
                        break;
                    case SDLK_RIGHT: case SDLK_DOWN: case SDLK_PAGEDOWN: case SDLK_SPACE:
                        if (!imgs.empty() && active_menu < 0) { idx = (idx + 1) % imgs.size(); load_img(); }
                        break;
                    case SDLK_HOME:
                        if (!imgs.empty()) { idx = 0; load_img(); }
                        break;
                    case SDLK_END:
                        if (!imgs.empty()) { idx = imgs.size() - 1; load_img(); }
                        break;
                }
            }
            
            if (e.type == SDL_MOUSEWHEEL) {
                if (active_menu < 0) {
                    if (e.wheel.y > 0 && !imgs.empty()) { idx = (idx - 1 + imgs.size()) % imgs.size(); load_img(); }
                    else if (e.wheel.y < 0 && !imgs.empty()) { idx = (idx + 1) % imgs.size(); load_img(); }
                }
            }
            
            if (e.type == SDL_MOUSEBUTTONDOWN && e.button.button == 1) {
                int w, h;
                SDL_GetWindowSize(win, &w, &h);
                int mh = menu_bar_height();
                
                if (show_about_dialog) {
                    show_about_dialog = false;
                    continue;
                }
                
                if (e.button.y < mh) {
                    int x = 10;
                    for (size_t i = 0; i < menu_items.size(); i++) {
                        if (e.button.x >= x && e.button.x < x + menu_items[i].w) {
                            active_menu = (active_menu == (int)i) ? -1 : i;
                            break;
                        }
                        x += menu_items[i].w + 5;
                    }
                } else if (active_menu >= 0) {
                    std::vector<std::string> items;
                    int drop_w = 180;
                    if (active_menu == 0) {
                        items = {translate("Abrir carpeta..."), "", translate("Cerrar")};
                    } else if (active_menu == 1) {
                        items = {translate("Pantalla completa"), translate("Ventana"), "", translate("Ajustar"), translate("Rellenar"), translate("Estirar"), translate("Original"), "", translate("Zoom +"), translate("Zoom -"), translate("Tamano original")};
                    } else if (active_menu == 2) {
                        items = {translate("Acerca de..."), "", translate("Lenguaje")};
                    }
                    
                    int sx = menu_items[active_menu].x + 10;
                    int sy = mh + 8;
                    
                    for (size_t i = 0; i < items.size(); i++) {
                        if (items[i].empty()) { sy += 16; continue; }
                        
                        if (e.button.x >= sx - 5 && e.button.x < sx + drop_w && e.button.y >= sy && e.button.y < sy + 24) {
                            if (active_menu == 0) {
                                if (i == 0) { open_folder_dialog(); }
                                else if (i == 2) running = false;
                            }
                            else if (active_menu == 1) {
                                if (i == 0) { full = true; SDL_SetWindowFullscreen(win, SDL_WINDOW_FULLSCREEN_DESKTOP); }
                                else if (i == 1) { full = false; SDL_SetWindowFullscreen(win, 0); }
                            }
                            else if (active_menu == 2) {
                                if (i == 0) show_about();
                                else if (i == 2) {
                                    SDL_MinimizeWindow(win);
                                    SDL_Delay(100);
                                    std::string lang = run_zenity("zenity --list --radiolist --title='Idioma' --column='Select' --column='Language' TRUE Espanol FALSE Deutsch FALSE English FALSE Francais FALSE Greigo FALSE Hindi FALSE Indonesio FALSE Italiano FALSE Japones FALSE Koreano FALSE Persa FALSE Polaco FALSE Portugues FALSE Ruso FALSE Turco FALSE Urdu FALSE Vietnamita FALSE Arabe FALSE Chino FALSE Japones");
                                    if (!lang.empty()) {
                                        if (lang == "Espanol") set_language("es");
                                        else if (lang == "Espanol") set_language("es");
                                        else if (lang == "Deutsch") set_language("de");
                                        else if (lang == "English") set_language("en");
                                        else if (lang == "Francais") set_language("fr");
                                        else if (lang == "Greigo") set_language("el");
                                        else if (lang == "Hindi") set_language("hi");
                                        else if (lang == "Indonesio") set_language("id");
                                        else if (lang == "Italiano") set_language("it");
                                        else if (lang == "Japones") set_language("ja");
                                        else if (lang == "Koreano") set_language("ko");
                                        else if (lang == "Persa") set_language("fa");
                                        else if (lang == "Polaco") set_language("pl");
                                        else if (lang == "Portugues") set_language("pt");
                                        else if (lang == "Ruso") set_language("ru");
                                        else if (lang == "Turco") set_language("tr");
                                        else if (lang == "Urdu") set_language("ur");
                                        else if (lang == "Vietnamita") set_language("vi");
                                        else if (lang == "Arabe") set_language("ar");
                                        else if (lang == "Chino") set_language("zh");
                                    }
                                    SDL_RestoreWindow(win);
                                }
                            }
                            active_menu = -1;
                            break;
                        }
                        sy += 24;
                    }
                } else if (active_menu < 0) {
                    if (!full) {
                        if (e.button.x < w/3) { if (!imgs.empty()) { idx = (idx - 1 + imgs.size()) % imgs.size(); load_img(); } }
                        else if (e.button.x > w*2/3) { if (!imgs.empty()) { idx = (idx + 1) % imgs.size(); load_img(); } }
                    } else {
                        if (e.button.x < w/2) { if (!imgs.empty()) { idx = (idx - 1 + imgs.size()) % imgs.size(); load_img(); } }
                        else { if (!imgs.empty()) { idx = (idx + 1) % imgs.size(); load_img(); } }
                    }
                }
            }
        }
        draw();
    }
}
