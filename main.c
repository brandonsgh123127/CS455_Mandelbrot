#include <stdio.h>

#include "graphics_utilities.h"
#include "xwindows_utilities.h"

double mandelbrot_real_center = -0.5;
double mandelbrot_imaginary_center=0.0;
double mandelbrot_scale = 1.0;
const double mandelbrot_radius = 1.5;


int main () {

    rgb_image_t *image;
    x_event_t mouse_event;

  //  if (isatty(fileno(stdin)))
  //      image = read_ppm_rgb_file("Mandy.ppm");
  //  else
   //     image= read_ppm_rgb_pipe();
    image = read_ppm_rgb_mandy();
    init_x();
    int done = 0;
	/* look for events while not done */
	while(!done) {

	    done = process_event(&mouse_event);

	    // map_window_to_complex calculate a new center
	    mandelbrot_scale += (mouse_event.button==mouse_scroll_forward)?4.0:0.0;
        mandelbrot_scale -= (mouse_event.button==mouse_scroll_backward)?1.0:0.0;
        mandelbrot_scale = (mandelbrot_scale<1.0)?1.0:mandelbrot_scale;

        double real_fraction = mouse_event.mouse_x/512.0-0.5;
        double imaginary_fraction = mouse_event.mouse_y/512.0-0.5;

        double real_offset = real_fraction * mandelbrot_radius/mandelbrot_scale;
        double imaginary_offset = imaginary_fraction * mandelbrot_radius/mandelbrot_scale;

        mandelbrot_real_center += real_offset;
        mandelbrot_imaginary_center +=imaginary_offset;

        mandelbrot_scale=(mouse_event.button==mouse_right_button)?1.0:mandelbrot_scale;
        mandelbrot_real_center=(mouse_event.button==mouse_right_button)?-0.50:mandelbrot_real_center;
        mandelbrot_imaginary_center=(mouse_event.button==mouse_right_button)?0.00:mandelbrot_imaginary_center;

        printf("%f, %f,%f\n",mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale);
        // here we will popen mandelbrot to get a new image
        image = read_ppm_rgb_mandy();
        display_image(image);

	}
    free_rgb_image(image);
    close_x();
}





