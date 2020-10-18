/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                            bit2.c
 *         by Minnie Xie (mxie03) and Emily Gjertsson (egjert01)
 *                           10/02/2020 (1 late token)
 *
 *                             iii
 *
 *     Implementation of a Bit2_T, a 2D version of Hanson Bit_T
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdio.h>
#include <stdlib.h>

#include "assert.h"
#include "except.h"

#include "bit.h"
#include "uarray.h"

#include "bit2.h"

Except_T CRE;

#define G Bit2_T
struct G {
    UArray_T bit2;
};

G Bit2_new(int width, int height)
{
    if (width <= 0 || height <= 0) {
        RAISE(CRE);
    }
    UArray_T bit_grid = UArray_new(width, sizeof(UArray_T));

    for (int i = 0; i < width; i++) {
        *((Bit_T *)UArray_at(bit_grid, i)) = Bit_new(height);
    }

    G grid2_T = malloc(sizeof(struct G));
    assert(grid2_T != NULL);
    grid2_T->bit2 = bit_grid;

    return grid2_T;
}

int Bit2_width(G grid)
{
    return UArray_length(grid->bit2);
}

int Bit2_height(G grid)
{
    Bit_T col_array = *((Bit_T *)UArray_at(grid->bit2, 0));
    return  Bit_length(col_array);
}

int Bit2_put(G grid, int width, int height, int bit_inserting)
{
    Bit_T column = *((Bit_T *)UArray_at(grid->bit2, width));
    return Bit_put(column, height, bit_inserting);
}

int Bit2_get(G grid, int width, int height)
{
    Bit_T column = *((Bit_T *)UArray_at(grid->bit2, width));
    return Bit_get(column, height);
}

void Bit2_map_col_major(G grid, void apply(int width, int height,
                         G bit_grid, int bit, void *p1), void *cl)
{
    for (int i = 0; i < Bit2_width(grid); i++) {
        for (int j = 0; j < Bit2_height(grid); j++) {

            apply(i, j, grid, Bit2_get(grid, i, j), cl);
        }
    }
}

void Bit2_map_row_major(G grid, void apply(int width, int height,
                        G bit_grid, int bit, void *p1), void *cl)
{
    for (int j = 0; j < Bit2_height(grid); j++) {
        for (int i = 0; i < Bit2_width(grid); i++) {
            apply(i, j, grid, Bit2_get(grid, i, j), cl);
        }
    }
}

void Bit2_free(G *grid_ptr)
{
    UArray_T grid = (*grid_ptr)->bit2;

    for (int i = 0; i < Bit2_width(*grid_ptr); i++) {
        Bit_free(UArray_at(grid, i));
    }

    UArray_free(&grid);
    free(*grid_ptr);
}

#undef G
