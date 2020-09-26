cmake_minimum_required(VERSION 3.1)
project(mandelbrot C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/include/X11)
find_library(libsimpleGraphics libsimpleGraphics.a ~/CLionProjects/simpleGraphics)
add_executable(mandelbrot main.c mandelbrot.c mandelbrot.h graphics_utilities.c graphics_utilities.h xwindows_utilities.h xwindows_utilities.c)
target_link_libraries(mandelbrot PRIVATE  X11 m ${libsimpleGraphics})