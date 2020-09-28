#include <stdio.h>
#include <argp.h>
#include "mandelbrot.h"
/* A description of the arguments we accept. */
static char args_doc[] = "ARG1 [STRING...]";

/* Keys for options without short-options. */
#define OPT_ABORT  1            /* –abort */

/* Current options available. */
static struct argp_option options[] = {
        {"real",    'r', "NUM",       0
                , "Real center relative to coordinate plane from -2 to 1" },
        {"imaginary",   'i', "NUM",0
                ,"Imaginary center relative to coordinate plane from -1.5 to 1.5"},
        {"scale",   's', "NUM",  0
                , "Scale relative to coordinate plane" },
        {"threads",   't', 0,  0
                , "Number of threads for processing" },
        {"file",   'f', "FILE_NAME.pgm",  0
                , "save output to file" },
                { 0 }};
/* Program documentation. */
static char doc[] =
        "Mandelbrot Set- Generate Mandelbrot set with ability to set coordinate values through commandline.\
\
\vNo flags will run a default program.\
";
/* Used by main to communicate with parse_opt. */
static struct arguments
{
    char *arg1;                   /* arg1 */
    char **strings;               /* [string…] */
    unsigned int coord,threading;          /* quick flag check*/
    double real,imaginary;
    double scale;      /* vals of args*/
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
        // Memory allocated to copy over arg without the '=' sign.
        char realDest[5],imaginaryDest[5],scaleDest[5],outDest[5];
    case 'r':
            memcpy( realDest, &arg[1], 5 );
            arguments->real = atof(realDest);
            arguments->coord = 1;
            break;
        case 'i':
            memcpy( imaginaryDest, &arg[1], 5 );
            arguments->imaginary = atof(imaginaryDest);
            arguments->coord = 1;
            break;
        case 's':
            memcpy( scaleDest, &arg[1], 5 );
            arguments->scale = atof(scaleDest);
            arguments->coord = 1;
            break;
        case 'f':
            arguments->output_file=arg;
            break;
        case 't':
            arguments->threading = atoi(arg);
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
static struct argp argp = { options, parse_opt, 0, doc};
struct rgb_image img;

rgb_image_t * main(int argc, char **argv) {
    rgb_image_t *image = &img;
    x_event_t mouse_event;
    struct arguments arguments;
    arguments.threading = 1;
    arguments.real = 0.0f;
    arguments.imaginary = 0.0f;
    arguments.scale = 1;
    arguments.output_file = NULL;
    arguments.threading=1;

    /*
     * Extern variables used for mandelbrot calculations.
     */
    extern double mandelbrot_real_center;
    extern double mandelbrot_imaginary_center;
    extern double mandelbrot_scale;
    extern double mandelbrot_radius;
    /* Default values. */
    argp_parse(&argp, argc, argv, 0, 0, &arguments);
    if (arguments.coord) {
        mandelbrot_real_center = arguments.real;
        mandelbrot_imaginary_center = arguments.imaginary;
        mandelbrot_scale = arguments.scale;
    }
    image = read_ppm_rgb_mandy(mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale);
    // Initialize xWindows
    init_x();
    int done = 0;
    /* look for events while not done */
    while(!done) {

        // call xwindows function for click checking...
        done = process_event(&mouse_event);
        // map_window_to_complex calculate a new center
        mandelbrot_scale += (mouse_event.button==mouse_scroll_forward)?1.0:0.0;
        mandelbrot_scale -= (mouse_event.button==mouse_scroll_backward)?1.0:0.0;
        mandelbrot_scale = (mandelbrot_scale<1.0)?1.0:mandelbrot_scale;
        // Calculate fraction/offset vals.
        double real_fraction = mouse_event.mouse_x/512.0-0.5;
        double imaginary_fraction = mouse_event.mouse_y/512.0-0.5;
        double real_offset = real_fraction * mandelbrot_radius/mandelbrot_scale;
        double imaginary_offset = imaginary_fraction * mandelbrot_radius/mandelbrot_scale;

        //Append offsets to extern variables
        mandelbrot_real_center += real_offset;
        mandelbrot_imaginary_center +=imaginary_offset;

        // Reset to original scale by right click
        mandelbrot_scale=(mouse_event.button==mouse_right_button)?2.0:mandelbrot_scale;
        mandelbrot_real_center=(mouse_event.button==mouse_right_button)?-0.50:mandelbrot_real_center;
        mandelbrot_real_center= mandelbrot_real_center < -2? -2:mandelbrot_real_center;
        mandelbrot_real_center= mandelbrot_real_center > 1? 1:mandelbrot_real_center;
        mandelbrot_imaginary_center=(mouse_event.button==mouse_right_button)?-1.00:mandelbrot_imaginary_center;
        mandelbrot_imaginary_center= mandelbrot_imaginary_center < -1.5? -1.5:mandelbrot_imaginary_center;
        mandelbrot_imaginary_center= mandelbrot_imaginary_center > 1.5? 1.5:mandelbrot_imaginary_center;

        printf("%f, %f,%f\n",mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale);
        // here we will popen mandelbrot to get a new image
        //image = read_ppm_rgb_mandy(mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale);
        image = calculate_mandelbrot(mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale,arguments.output_file,512,200);
        // IF PIPE SPECIFIED BY USER -> SAVE TO PGM FILE-> CLOSE APPLICATION
        if(arguments.output_file != NULL) {
            write_grayscale_file(arguments.output_file, image);
            exit(0);
        }
        display_image(image);
        //rgb_image_t * image = calculate_mandelbrot(center,2,512,100);
        if (isatty(fileno(stdout)))
            write_rgb_file("First.ppm",image);
        else
            write_rgb_pipe(image);
    }
    free_rgb_image(image);
    close_x();
    return image;
}





