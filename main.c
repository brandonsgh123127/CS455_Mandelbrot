#include <stdio.h>
#include <argp.h>
#include "mandelbrot.h"
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
    double complex real,imaginary;
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
        case 'r':
            arguments->real = atof(arg);
            arguments->coord = 1;
            break;
        case 'i':
            arguments->imaginary = atof(arg);
            arguments->coord = 1;
            break;
        case 's':
            arguments->scale = atof(arg);
            arguments->coord = 1;
            break;
        case 'f':
            arguments->output_file = arg;
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
static struct argp argp = { options, parse_opt, args_doc, doc};

rgb_image_t * main(int argc, char **argv)
{
    rgb_image_t * image;
    x_event_t mouse_event;
    struct arguments arguments;

    /* Default values. */
    arguments.threading = 0;
    arguments.real = 0;
    arguments.imaginary = 0;
    arguments.scale=1;
    *arguments.output_file = "-";
    argp_parse(&argp, argc, argv, 0, 0, &arguments);

    extern double mandelbrot_real_center;
    extern double mandelbrot_imaginary_center;
    extern double mandelbrot_scale;
    extern double mandelbrot_radius;


    image = read_ppm_rgb_mandy(arguments.real,arguments.imaginary,arguments.scale);
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
        image = read_ppm_rgb_mandy(mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale);
        //image = calculate_mandelbrot(mandelbrot_real_center,mandelbrot_imaginary_center,mandelbrot_scale,*arguments.output_file,512);
        display_image(image);
        // parse_input_args
        double center = -0.5-0.0i;
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





