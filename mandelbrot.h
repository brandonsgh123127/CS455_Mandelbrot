//
// Created by joel on 9/10/20.
//
#include <complex.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "xwindows_utilities.h"


#ifndef MANDELBROT_MANDELBROT_H
#define MANDELBROT_MANDELBROT_H
extern double mandelbrot_real_center;
extern double mandelbrot_imaginary_center;
extern double mandelbrot_scale;
extern double mandelbrot_radius;

unsigned int iterations(double complex p0,unsigned int max_iteration);
rgb_image_t * calculate_mandelbrot(double complex center,
                                   double complex imaginary,
                                   double scale,
                                   char *out,
                                   int n);

#endif //MANDELBROT_MANDELBROT_H
