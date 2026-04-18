cmake_minimum_required(VERSION 3.10)
project(xfce-liview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
find_package(SDL2 REQUIRED)

pkg_check_modules(IMLIB2 REQUIRED imlib2)

find_path(SDL2_IMAGE_INCLUDE_DIRS SDL_image.h PATH_SUFFIXES SDL2)
find_library(SDL2_IMAGE_LIBRARIES SDL2_image)
find_path(SDL2_TTF_INCLUDE_DIRS SDL_ttf.h PATH_SUFFIXES SDL2)
find_library(SDL2_TTF_LIBRARIES SDL2_ttf)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS} ${IMLIB2_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/viewer.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${IMLIB2_LIBRARIES} X11)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
