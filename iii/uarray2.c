/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              uarray2.c
 *         by Minnie Xie (mxie03) and Emily Gjertsson (egjert01)
 *                           10/02/2020 (1 late token)
 *
 *                               iii
 *
 *     Implementation of UArray2_T, 2D version of Hanson UArray_T
 *                         2D umboxed array
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "except.h"

#include "uarray2.h"
#include "uarray.h"

#define G UArray2_T
struct G {
    UArray_T uarr2;
};

Except_T CRE;

G UArray2_new(int width, int height, int size)
{
    if (width <= 0 || height <= 0 || size <= 0) {
        RAISE(CRE);
    }

    UArray_T grid = UArray_new(width, sizeof(UArray_T));

    for (int i = 0; i < width; i++) {
        *((UArray_T *)UArray_at(grid, i)) = UArray_new(height, size);
    }

    G grid2_T = malloc(sizeof(struct G));
    assert(grid2_T != NULL);

    grid2_T->uarr2 = grid;

    return grid2_T;
}

void *UArray2_at(G grid, int width, int height)
{
    UArray_T column = *((UArray_T *)UArray_at(grid->uarr2, width));

    assert(width < UArray_length(grid->uarr2) && width >= 0);
    assert(height < UArray2_height(grid) && width >= 0);

    return UArray_at(column, height);
}

int UArray2_width(G grid)
{
    return UArray_length(grid->uarr2);
}

int UArray2_height(G grid)
{
    UArray_T col_array = *((UArray_T *)UArray_at(grid->uarr2, 0));
    return UArray_length(col_array);
}

int UArray2_size(G grid)
{
    return sizeof (UArray2_at(grid, 0, 0));
}

void UArray2_map_row_major(G grid, void apply(int width, int height, G uarr_2,
                           void *element, void *p2), void* cl)
{
    for (int j = 0; j < UArray2_height(grid); j++) {
        for (int i = 0; i < UArray2_width(grid); i++) {
            apply(i, j, grid, UArray2_at(grid, i, j), cl);
        }
    }
}
void UArray2_map_col_major(G grid, void apply(int width, int height, G uarr_2,
                           void *element, void *p2), void* cl)
{
    for (int i = 0; i < UArray2_width(grid); i++) {
        for (int j = 0; j < UArray2_height(grid); j++) {
            apply(i, j, grid, UArray2_at(grid, i, j), cl);
        }
    }
}

void UArray2_free(G *grid_ptr)
{
    assert(grid_ptr != NULL);

    UArray_T grid = (*grid_ptr)->uarr2;

    int width = UArray_length(grid);

    for (int i = 0; i < width; i++) {
        UArray_free(UArray_at(grid, i));
    }
    UArray_free(&grid);
    free(*grid_ptr);
}
