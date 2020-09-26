//
// Created by joel on 9/10/20.
//
#include "mandelbrot.h"
#include <argp.h>
#include "graphics_utilities.h"

extern double mandelbrot_real_center = -0.5;
extern double mandelbrot_imaginary_center=0.0;
extern double mandelbrot_scale = 1.0;
extern  double mandelbrot_radius = 1.5;

rgb_image_t * calculate_mandelbrot(double complex center,
                                   double complex imaginary,
                                   double scale,
                                   char *out,
                                   int n)
{
    const double extent = 3.0;
    const double delta = extent/(1.0 * 1*scale);
    const double start_real = creal(center)-extent/(2.*scale);
    const double start_imaginary = cimag(center)-extent/(2.*scale);
    const unsigned int background=3;
    struct rgb_image t;
    rgb_image_t * image = &t;
    image = malloc(sizeof(t));
    image->image_size_x=n;
    image->image_size_y=n;
    image->image_data = malloc(image->image_size_x * image->image_size_y * 3);

    double im = start_imaginary;
    int image_ptr=0;
    struct rgb_image imgAlloc;
    rgb_t * p = &imgAlloc;
    for (int c=0;c<n;c++) {
        double re = start_real;
        for (int r = 0; r < n; r++) {
            int count = iterations(re+im*1i, n);
            if(count>background)
                hsv_to_rgb(background,count, n,&p);
            else{p->r=0;p->g=0;p->b=0;}
            image->image_data[image_ptr++]=p->r;
            image->image_data[image_ptr++]=p->g;
            image->image_data[image_ptr++]=p->b;

            re += delta;
        }
        im += delta;

    }

    return image;
}


unsigned int iterations(double complex p0,unsigned int max_iteration)
{
    unsigned int icount = 0;
    double complex p = p0;

    while (cabs(p)<2 && icount<max_iteration)
    {
        p = p*p+p0;
        icount++;
    }

    return icount;
}