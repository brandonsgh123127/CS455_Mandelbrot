cmake_minimum_required(VERSION 3.1)
project(simpleGraphics C)

set(CMAKE_C_STANDARD 11)

include_directories(/usr/include/X11)

add_executable(simpleGraphics main.c graphics_utilities.c graphics_utilities.h xwindows_utilities.c xwindows_utilities.h)

target_link_libraries(simpleGraphics X11 m)
