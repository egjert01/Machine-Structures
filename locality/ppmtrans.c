/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              ppmtrans.c
 *         by Emily Gjertsson (egjert01) and Minnie Xie (mxie03)
 *                             10/12/2020
 *
 *                              locality
 *
 *            Reads in a ppm file or stdin and transforms it
 *       according to specified commands. Outputs transformed image
 *           to stdout, if timing is requested ouputs results
 *                          to specified file.
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "a2blocked.h"
#include "pnm.h"
#include "cputiming.h"

/* * * * * * * * * * * provided functions * * * * * * * * * * * */
#define SET_METHODS(METHODS, MAP, WHAT) do {                    \
        methods = (METHODS);                                    \
        assert(methods != NULL);                                \
        map = methods->MAP;                                     \
        if (map == NULL) {                                      \
                fprintf(stderr, "%s does not support "          \
                                WHAT "mapping\n",               \
                                argv[0]);                       \
                exit(1);                                        \
        }                                                       \
} while (0)

static void
usage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-rotate <angle>] "
                        "[-{row,col,block}-major] [filename]\n",
                        progname);
        exit(1);
}

static void
flipUsage(const char *progname)
{
        fprintf(stderr, "Usage: %s [-flip <horizontal or vertical>] "
                        "[filename]\n",
                        progname);
        exit(1);
}
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


/*
* Struct used only for mapping (closure variable)
* Holds the array that will be written to in the mapping functions (i.e.
* stores the rotated image) and the methods set in main
*/
typedef struct trans_supplies {
    A2Methods_UArray2 rotated;
    A2Methods_T methods;
} trans_supplies;

/* * * * * * * * * * * Our function declarations * * * * * * * * * * * */
A2Methods_UArray2 rotate(char *time_file_name, int degrees,
                         A2Methods_T methods, A2Methods_mapfun *map,
                         Pnm_ppm *pixmap);

A2Methods_UArray2 flipImage(char *time_file_name, char* flipDirection,
                            A2Methods_T methods, A2Methods_mapfun *map,
                            Pnm_ppm pixmap);

A2Methods_UArray2 transposeImage(char *time_file_name, A2Methods_T methods,
                                A2Methods_mapfun *map, Pnm_ppm *pixmap);

void flipHorizontal(int col, int row, A2Methods_UArray2 array2, void *elem,
                    void *methodsAndArray);

void flipVertical(int col, int row, A2Methods_UArray2 array2, void *elem,
                  void *methodsAndArray);

void transpose_ppm(int col, int row, A2Methods_UArray2 array2, void *elem,
             void *methodsAndArray);

void rotate90(int i, int j, A2Methods_UArray2 array2, void *elem,
                void *trans_supplies);

void rotate180(int i, int j, A2Methods_UArray2 array2, void *elem,
                void *trans_supplies);

void rotate270(int i, int j, A2Methods_UArray2 array2, void *elem,
                void *trans_supplies);

A2Methods_UArray2 callRotation(int width,
                               int height,
                               A2Methods_mapfun *map,
                               trans_supplies *methodsAndArray,
                               Pnm_ppm *pixmap,
                               char *time_file_name,
                               void (*rotate_func) (int col, int row,
                                                   A2Methods_UArray2 array2,
                                                   void *elem,
                                                   void *methodsAndArray)
                                );

void printTime(char *time_file_name, double duration, int num_pixels);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


int main(int argc, char *argv[])
{
    char *time_file_name = NULL;
    int   rotation       = 0;
    int   i;

    char *flipDirection = NULL;
    bool flip           = false;
    bool transform      = false;

    /* default to UArray2 methods */
    A2Methods_T methods = uarray2_methods_plain;
    assert(methods);

    /* default to best map */
    A2Methods_mapfun *map = methods->map_default;
    assert(map);

    for (i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-row-major") == 0) {
                    SET_METHODS(uarray2_methods_plain, map_row_major,
                        "row-major");
            } else if (strcmp(argv[i], "-col-major") == 0) {
                    SET_METHODS(uarray2_methods_plain, map_col_major,
                        "column-major");
            } else if (strcmp(argv[i], "-block-major") == 0) {
                    SET_METHODS(uarray2_methods_blocked, map_block_major,
                                "block-major");

            } else if (strcmp(argv[i], "-rotate") == 0) {
                    if (!(i + 1 < argc)) {      /* no rotate value */
                            usage(argv[0]);
                    }
                    char *endptr;
                    rotation = strtol(argv[++i], &endptr, 10);
                    if (!(rotation == 0 || rotation == 90 ||
                        rotation == 180 || rotation == 270)) {
                            fprintf(stderr,
                                    "Rotation must be 0, 90 180 or 270\n");
                            usage(argv[0]);
                    }
                    if (!(*endptr == '\0')) {    /* Not a number */
                            usage(argv[0]);
                    }
            } else if (strcmp(argv[i], "-flip") == 0) {
                if (!(i + 1 < argc)) {      /* no flip specification */
                        flipUsage(argv[0]);
                }
                if ((strcmp(argv[i + 1], "horizontal") != 0 &&
                    strcmp(argv[i + 1], "vertical") != 0)){
                        fprintf(stderr,
                                "Flip must be horizontal or vertical\n");
                        flipUsage(argv[0]);
                }
                flip = true;
                flipDirection = argv[i + 1];
                i++;

            } else if (strcmp(argv[i], "-transpose") == 0) {
                transform = true;
            } else if (strcmp(argv[i], "-time") == 0) {
                    time_file_name = argv[++i];
            } else if (*argv[i] == '-') {
                    fprintf(stderr, "%s: unknown option '%s'\n", argv[0],
                            argv[i]);
            } else if (argc - i > 1) {
                    fprintf(stderr, "Too many arguments\n");
                    usage(argv[0]);
            } else {
                    break;
            }
    }

    Pnm_ppm pixmap;
    if (argc == i) {
        /* read with stdin */
        pixmap = Pnm_ppmread(stdin, methods);
        pixmap->pixels = rotate(time_file_name, rotation, methods, map,
                                &pixmap);
        if (flip == true) {
            pixmap->pixels = flipImage(time_file_name, flipDirection, methods,
                                       map, pixmap);
        }

        if (transform == true) {
            pixmap->pixels = transposeImage(time_file_name, methods, map,
                                            &pixmap);
        }
        Pnm_ppmwrite(stdout, pixmap);

    } else {
        FILE *fp = fopen(argv[i], "rd");
        assert(fp);

        pixmap = Pnm_ppmread(fp, methods);
        pixmap->pixels = rotate(time_file_name, rotation, methods, map,
                                &pixmap);
        if (flip == true) {
            pixmap->pixels = flipImage(time_file_name, flipDirection, methods,
                                       map, pixmap);
        }

        if (transform == true) {
            pixmap->pixels = transposeImage(time_file_name, methods, map,
                                                                    &pixmap);
        }
        Pnm_ppmwrite(stdout, pixmap);

        fclose(fp);
    }

    Pnm_ppmfree(&pixmap);
    exit(0);
}

/*
* Intializes a file pointer for timing output file and prints duration of
* mapping the entire image and of one pixel in nanoseconds.
* C.R.E.: none
* Memory allocation: file pointer created for timing file, freed in scope
*/
void printTime(char *time_file_name, double duration, int num_pixels)
{
    if (time_file_name != NULL) {
        FILE *time_fp = NULL;
        time_fp = fopen(time_file_name, "w");
        fprintf(time_fp, "Transformation completed in %.0f nanoseconds\n",
                duration);
        fprintf(time_fp, "Each pixel mapped in %.0f nanoseconds\n",
                duration / num_pixels);
        fclose(time_fp);
    }
}

/*
* Transposes images, measure time taken to transpose
* C.R.E.: if methods, map, or pixmap are NULL
* Memory allocation: for the CPU timer, for the new  array that will store the
*                   final rotated image, for the trans_supplies struct and the
*                   array it stores -- all freed in this function.
*/
A2Methods_UArray2 transposeImage(char *time_file_name, A2Methods_T methods,
                                A2Methods_mapfun *map, Pnm_ppm *pixmap)
{
    assert(methods && map && pixmap);
    CPUTime_T timer = CPUTime_New();
    double time_used;

    int width = (*pixmap)->width;
    int height = (*pixmap)->height;

    trans_supplies *methodsAndArray = malloc(sizeof(struct trans_supplies));
    methodsAndArray->methods = methods;

    A2Methods_UArray2 transposed;

    transposed = methods->new(height, width, (sizeof (struct Pnm_rgb)));

    methodsAndArray->rotated = transposed;
    CPUTime_Start(timer);
    map((*pixmap)->pixels, transpose_ppm, methodsAndArray);
    time_used = CPUTime_Stop(timer);

    (*pixmap)->width = height;
    (*pixmap)->height = width;

    printTime(time_file_name, time_used, width * height);

    CPUTime_Free(&timer);
    methods->free(&((*pixmap)->pixels));
    free(methodsAndArray);

    return transposed;
}


/*
* Flip images either horizontally or vertically, time execution of flip
* C.R.E.: if methods, map, or pixmap are NULL
* Memory allocation: for the CPU timer, for the new  array that will store the
*                   final rotated image, for the trans_supplies struct and the
*                   array it stores -- all freed in this function.
*/
A2Methods_UArray2 flipImage(char *time_file_name, char* flipDirection,
                            A2Methods_T methods, A2Methods_mapfun *map,
                            Pnm_ppm pixmap)
{
    assert(methods && map && pixmap);
    CPUTime_T timer = CPUTime_New();
    double time_used;

    int width = pixmap->width;
    int height = pixmap->height;

    trans_supplies *methodsAndArray = malloc(sizeof(struct trans_supplies));
    methodsAndArray->methods = methods;

    A2Methods_UArray2 flipped;

    if (strcmp(flipDirection, "vertical") == 0) {
        flipped = methods->new(width, height, (sizeof (struct Pnm_rgb)));

        methodsAndArray->rotated = flipped;
        CPUTime_Start(timer);
        map(pixmap->pixels, flipVertical, methodsAndArray);
        time_used = CPUTime_Stop(timer);

    } else if (strcmp(flipDirection, "horizontal") == 0) {
        flipped = methods->new(width, height, (sizeof (struct Pnm_rgb)));

        methodsAndArray->rotated = flipped;
        CPUTime_Start(timer);
        map(pixmap->pixels, flipHorizontal, methodsAndArray);
        time_used = CPUTime_Stop(timer);
    }

    printTime(time_file_name, time_used, width * height);

    CPUTime_Free(&timer);
    methods->free(&(pixmap->pixels));
    free(methodsAndArray);

    return flipped;
}

/*
* Call the callRotation function with the associated function pointer
* (i.e.) rotate90 if the rotation we're executing is 90 degrees.
* C.R.E.: if methods is NULL, if map is NULL, or if pixmap is NULL
* Memory allocation: for the trans_supplies struct. freed in callRotation.
*/
A2Methods_UArray2 rotate(char *time_file_name, int degrees,
                         A2Methods_T methods, A2Methods_mapfun *map,
                         Pnm_ppm *pixmap)
{
    assert(methods && map && pixmap);

    int width = (*pixmap)->width;
    int height = (*pixmap)->height;

    trans_supplies *methodsAndArray = malloc(sizeof(struct trans_supplies));
    methodsAndArray->methods = methods;

    if (degrees == 90) {
        return callRotation(width, height, map, methodsAndArray, pixmap,
                             time_file_name, rotate90);

    } else if (degrees == 180) {
        return callRotation(height, width, map, methodsAndArray, pixmap,
                             time_file_name, rotate180);

    } else if (degrees == 270) {
        return callRotation(width, height, map, methodsAndArray, pixmap,
                             time_file_name, rotate270);

    }
    return (*pixmap)->pixels; /* if rotation == 0 return original array*/
}

/*
* Call mapping functions, use CPUTime_T to measure how long it takes to
* rotate or flip images
* C.R.E.: if any of the following are NULL: map, methodsAndArray struct,
*         pixmap, or rotate_func (function pointer)
* Memory allocation: for the CPU timer, freed in this function, and the new
*        array that will store the final rotated image, freed in this function
*        along with the rest of the struct
*/
A2Methods_UArray2 callRotation(int width,
                               int height,
                               A2Methods_mapfun *map,
                               trans_supplies *methodsAndArray,
                               Pnm_ppm *pixmap,
                               char *time_file_name,
                               void (*rotate_func) (int col, int row,
                                                   A2Methods_UArray2 array2,
                                                   void *elem,
                                                   void *methodsAndArray)
                                )
{
    assert(map && methodsAndArray && pixmap && rotate_func);
    CPUTime_T timer = CPUTime_New();
    double time_used;

    A2Methods_UArray2 rotated;

    A2Methods_T methods = methodsAndArray->methods;

    rotated = methods->new(height, width, (sizeof (struct Pnm_rgb)));
    methodsAndArray->rotated = rotated;

    CPUTime_Start(timer);
    map((*pixmap)->pixels, rotate_func, methodsAndArray);
    time_used = CPUTime_Stop(timer);

    (*pixmap)->width = height;
    (*pixmap)->height = width;

    printTime(time_file_name, time_used, width * height);

    CPUTime_Free(&timer);
    methods->free(&((*pixmap)->pixels));
    free(methodsAndArray);

    return rotated;
}



/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*  The following are apply functions for mapping rotations from
*  one 2D array to another (passed to the function in a struct
*  along with associated methods).
*  C.R.E.: If Array2 is NULL, if the methodsAndArray struct is
*          NULL, or if the array or methods in the struct are NULL
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void rotate90(int col, int row, A2Methods_UArray2 array2, void *elem,
              void *methodsAndArray)
{
    assert(array2 && methodsAndArray);

    A2Methods_T methods = ((trans_supplies *)methodsAndArray)->methods;
    A2Methods_UArray2 rotated = ((trans_supplies *)methodsAndArray)->rotated;
    assert(rotated && methods);

    int height = methods->height(array2);

    *((Pnm_rgb)methods->at(rotated, height - row - 1, col)) = *((Pnm_rgb)elem);
}

void rotate180(int col, int row, A2Methods_UArray2 array2, void *elem,
               void *methodsAndArray)
{
    assert(array2 && methodsAndArray);

    A2Methods_T methods = ((trans_supplies *)methodsAndArray)->methods;
    A2Methods_UArray2 rotated = ((trans_supplies *)methodsAndArray)->rotated;
    assert(methods && rotated);

    int height = methods->height(array2);
    int width = methods->width(array2);

    *((Pnm_rgb)methods->at(rotated, width - col - 1, height - row - 1))
                                                    = *((Pnm_rgb)elem);
}

void rotate270(int col, int row, A2Methods_UArray2 array2, void *elem,
               void *methodsAndArray)
{
    assert(array2 && methodsAndArray);

    A2Methods_T methods = ((trans_supplies *)methodsAndArray)->methods;
    A2Methods_UArray2 rotated = ((trans_supplies *)methodsAndArray)->rotated;
    assert(methods && rotated);

    int width = methods->width(array2);

    *((Pnm_rgb)methods->at(rotated, row, width - col - 1)) = *((Pnm_rgb)elem);
}

void flipHorizontal(int col, int row, A2Methods_UArray2 array2, void *elem,
               void *methodsAndArray)
{
    assert(array2 && methodsAndArray);

    A2Methods_T methods = ((trans_supplies *)methodsAndArray)->methods;
    A2Methods_UArray2 flipped = ((trans_supplies *)methodsAndArray)->rotated;
    assert(methods && flipped);

    int width = methods->width(array2);

    *((Pnm_rgb)methods->at(flipped, width - col - 1, row)) = *((Pnm_rgb)elem);
}

void flipVertical(int col, int row, A2Methods_UArray2 array2, void *elem,
               void *methodsAndArray)
{
    assert(array2 && methodsAndArray);

    A2Methods_T methods = ((trans_supplies *)methodsAndArray)->methods;
    A2Methods_UArray2 flipped = ((trans_supplies *)methodsAndArray)->rotated;
    assert(methods && flipped);

    int height = methods->height(array2);

    *((Pnm_rgb)methods->at(flipped, col, height - row - 1)) = *((Pnm_rgb)elem);
}

void transpose_ppm(int col, int row, A2Methods_UArray2 array2, void *elem,
               void *methodsAndArray)
{
    assert(array2 && methodsAndArray);

    A2Methods_T methods = ((trans_supplies *)methodsAndArray)->methods;
    A2Methods_UArray2 transposed
                        = ((trans_supplies *)methodsAndArray)->rotated;
    assert(methods && transposed);

    *((Pnm_rgb)methods->at(transposed, row, col)) = *((Pnm_rgb)elem);
}
