cmake_minimum_required(VERSION 3.10)
project(xfce-liview LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS} ${SDL2_IMAGE_INCLUDE_DIRS} ${SDL2_TTF_INCLUDE_DIRS})

add_executable(${PROJECT_NAME} src/main.cpp src/viewer.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} SDL2_image SDL2 SDL2_ttf)

set(CPACK_PACKAGE_NAME "xfce-liview")
set(CPACK_PACKAGE_VERSION "1.0.0")
set(CPACK_PACKAGE_CONTACT "xfce-liview")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "xfce-liview")
set(CPACK_GENERATOR "DEB")
include(CPack)
