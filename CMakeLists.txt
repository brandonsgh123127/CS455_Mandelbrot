cmake_minimum_required(VERSION 3.1)
project(Mandelbrot C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/include/X11)

add_executable(Mandelbrot mandelbrot.c mandelbrot.h)
include_directories(~/CLionProjects/graphics)
include_directories(~/CLionProjects/simpleGraphics)
