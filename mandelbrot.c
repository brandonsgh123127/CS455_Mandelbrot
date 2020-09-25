//
// Created by joel on 9/10/20.
//
#include "graphics_utilities.h"
#include "xwindows_utilities.h"
#include "mandelbrot.h"
#include <stdlib.h>
#include <error.h>
#include <complex.h>
#include <stdio.h>
#include <argp.h>

unsigned int iterations(double complex p0,unsigned int max_iteration);

/* A description of the arguments we accept. */
static char args_doc[] = "-r=# -i=# -s=# -t=# -f=\"\"";

/* Keys for options without short-options. */
#define OPT_ABORT  1            /* –abort */

/* Current options available. */
static struct argp_option options[] = {
        {"real",    'r', 0,       OPTION_ARG_OPTIONAL, "Real center relative to coordinate plane from -2 to 1" },
        {"imaginary",   'i', 0,OPTION_ARG_OPTIONAL,"Imaginary center relative to coordinate plane from -1.5 to 1.5"},
        {"scale",   's', 0,  OPTION_ARG_OPTIONAL, "Scale relative to coordinate plane" },
        {"threads",   't', 0,  OPTION_ARG_OPTIONAL, "Number of threads for processing" }};
/* Program documentation. */
static char doc[] =
        "Mandelbrot Set- Generate Mandelbrot set with ability to set coordinate values through commandline.\
\
\vNo flags will run a default program.\
";
/* Used by main to communicate with parse_opt. */
struct arguments
{
    char *arg1;                   /* arg1 */
    char **strings;               /* [string…] */
    int coord,threading;          /* quick flag check*/
    int real,imaginary,scale;      /* vals of args*/
    char *output_file;            /* file arg to ‘-f’ */
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
    /* Get the input argument from argp_parse, which we
       know is a pointer to our arguments structure. */
    struct arguments *arguments = state->input;

    switch (key)
    {
        case 'r':
            arguments->real = arg;
            arguments->coord = 1;
            break;
        case 'i':
            arguments->imaginary = arg;
            arguments->coord = 1;
            break;
        case 's':
            arguments->scale = arg;
            arguments->coord = 1;
            break;
        case 'f':
            arguments->output_file = arg;
            break;
        case 't':
            arguments->threading = 1;
            break;
        case ARGP_KEY_NO_ARGS:
            break;
        case ARGP_KEY_ARG:
            arguments->arg1 = arg;
            arguments->strings = &state->argv[state->next];
            state->next = state->argc;
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc};

int main(int argc, char **argv)
{
    struct rgb_image_t image;
    struct arguments arguments;

    /* Default values. */
    arguments.threading = 0;
    arguments.real = 0;
    arguments.imaginary = 0;
    arguments.scale=0;
    arguments.output_file = "-";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    image = calculate_mandelbrot(arguments.real,arguments.imaginary,arguments.scale,&arguments.output_file);
}


rgb_image_t * calculate_mandelbrot(double complex center,
                                   double complex imaginary,
                                   double scale,
                                   char out)
{
    const double extent = 3.0;
    const double delta = extent/(1.0 * 1*scale);
    const double start_real = creal(center)-extent/(2.*scale);
    const double start_imaginary = cimag(center)-extent/(2.*scale);
    const unsigned int background=2;

    rgb_image_t * image;
    image = malloc(sizeof(rgb_image_t));
    image->image_size_x=1;
    image->image_size_y=1;
    image->image_data = malloc(image->image_size_x * image->image_size_y * 3);

    double im = start_imaginary;
    int image_ptr=0;
/*
    for (int c=0;c<n;c++) {
        double re = start_real;

        for (int r = 0; r < n; r++) {
            int count = iterations(re+im*1i, max_iteration);
            rgb_t p;
            if(count>background)
                hsv_to_rgb(count, background, max_iteration, &p);
            else{p.r=0;p.g=0;p.b=0;}
            image->image_data[image_ptr++]=p.r;
            image->image_data[image_ptr++]=p.g;
            image->image_data[image_ptr++]=p.b;

            re += delta;
        }
        im += delta;

    }*/
    return image;
}



void write_rgb_pipe(rgb_image_t *image)
{
    int i, j,im_ptr;
    FILE *fp = stdout;
    fprintf(fp, "P6\n%d %d\n255\n", image->image_size_x, image->image_size_y);
    im_ptr =0;
    for (j = 0; j < image->image_size_y; ++j) {
        for (i = 0; i < image->image_size_y; ++i) {
            static unsigned char color[3];
            color[0] = image->image_data[im_ptr++];  // red
            color[1] = image->image_data[im_ptr++];  // green
            color[2] = image->image_data[im_ptr++];  // blue
            fwrite(color, 1, 3, fp);
        }
    }
    fclose(fp);
}
