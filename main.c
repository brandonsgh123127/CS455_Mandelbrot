#include <stdio.h>
#include <unistd.h>

#include "graphics_utilities.h"
#include "mandelbrot.h"


int main() {
    // parse_input_args
    double complex center = -0.5-0.0i;
    rgb_image_t * image = calculate_mandelbrot(center,2,512,100);
    if (isatty(fileno(stdout)))
        write_rgb_file("Mandy.ppm",image);
    else
        write_rgb_pipe(image);
    return 0;
}
