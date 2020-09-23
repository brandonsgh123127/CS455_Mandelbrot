cmake_minimum_required(VERSION 3.17)
project(mandelbrot C)

set(CMAKE_C_STANDARD 11)

add_executable(mandelbrot main.c graphics_utilities.c graphics_utilities.h mandelbrot.c mandelbrot.h)

target_link_libraries(mandelbrot m)