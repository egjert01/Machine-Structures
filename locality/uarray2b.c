/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *
 *                     uarray2b.c
 *
 *     Assignment:  Locality (hw3)
 *     Authors:  Minnie Xie (mxie03) & Emily Gjertsson (egjert01)
 *     Date:     10/12/2020
 *
 *     Summary:
 *          Implementation for uarray2b interface: allows user to
 *          create a blocked version of UArray2. This is accomplished
 *          by storing a UArray_T inside of a UArray2.
 *          **Note: importance of blocked implementation - when mapping,
 *                  each cell within a block will be visited before moving
 *                  on (cells and blocks visited in row-major order).
 *
 *          **Note: It is a checked run-time error to pass NULL
 *                   to any function in this interface
 *
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "assert.h"
#include "except.h"
#include "uarray.h"
#include "uarray2.h"
#include "uarray2b.h"

Except_T CRE;

const int MAX_BLOCK_MEM = 64*1024;

extern void fillGrid(int i, int j, UArray2_T array2, void *elem, void *cl);
extern void freeGrid(int i, int j, UArray2_T array2, void *elem, void *cl);


/*
* Blocked 2D array represented by a struct.
* UArray2_T stores all of the information (the essence of the blocked array)
* blocksize is the height of one block
*/
#define T UArray2b_T
struct T {
    UArray2_T blocked;
    int width, height;
    int size;
    int blocksize;
};

/*
* Create a new blocked UArray2 to the specifications given by
* client (width, height, size of what's being stored, and the
* height of a block -- square root of # of cells per block).
* C.R.E.: none
*/
T UArray2b_new (int width, int height, int size, int blocksize)
{
    if (blocksize < 1 || size < 1 || height < 1 || width < 1) {
        RAISE(CRE);
    }

    T uarr2b = malloc(sizeof(struct T));

    UArray2_T grid = UArray2_new(ceil((double)width / blocksize),
                                 ceil((double)height / blocksize),
                                 sizeof(UArray_T));

    uarr2b->blocked = grid;
    uarr2b->width = width;
    uarr2b->height = height;
    uarr2b->size = size;
    uarr2b->blocksize = blocksize;

    UArray2_map_row_major(grid, fillGrid, &uarr2b);

    return uarr2b;
}

/*
* Helper function for UArray2b_new, apply function which goes to
* every spot in the Array2 and initialized a UArray_T representing
* the cells in every block
* C.R.E.: raised if array2 is NULL or if the cl is NULL
*/
void fillGrid(int i, int j, UArray2_T array2, void *elem, void *uarr2b)
{
    (void)elem;
    assert(array2 && uarr2b);

    int blocksize = UArray2b_blocksize(*((UArray2b_T *)uarr2b));
    int size = UArray2b_size(*((UArray2b_T *)uarr2b));

    UArray_T cells = UArray_new(blocksize * blocksize, size);

    *((UArray_T *)UArray2_at(array2, i, j)) = cells;
}

/*
* New blocked 2d array: blocksize as large as possible provided
* block occupies at most 64KB (if possible). Calls UArray2b_new with
* the calculated blocksize
* C.R.E.: if width, height, or size are less than 1
*/
T UArray2b_new_64K_block(int width, int height, int size)
{
    if (size < 1 || height < 1 || width < 1) {
        RAISE(CRE);
    }

    double blocksize;
    double blockArea = MAX_BLOCK_MEM /size;

    if (blockArea < 1) {
        blocksize = 1.0;
    } else {
        blocksize = floor(sqrt(blockArea));
    }

    return UArray2b_new(width, height, size, (int)blocksize);
}

/*
* Free all memory associated with Array2b -- first free every UArray_T
* then free the UArray2_T, then the Array2b struct
* C.R.E.: raised if array2b_ptr is NULL or if array2b is NULL (*array2b_ptr)
*/
void UArray2b_free (T *array2b_ptr)
{
    assert(array2b_ptr && *array2b_ptr);

    T array2b = *array2b_ptr;
    assert(array2b->blocked);

    UArray2_map_row_major((array2b->blocked), freeGrid, NULL);

    UArray2_free(&array2b->blocked);
    free(*array2b_ptr);
}

/*
* Helper function for UArray2b_free, apply function which goes to
* every spot in the Array2 and frees the UArray_T at that location
* C.R.E.: raised if array2 is NULL
*/
void freeGrid(int i, int j, UArray2_T array2, void *elem, void *cl)
{
    (void)elem;
    (void)cl;

    assert(array2);

    UArray_free((UArray_T *)UArray2_at(array2, i, j));
}

/* return width of array2b
* C.R.E.: raised if array2 is NULL */
int UArray2b_width (T array2b)
{
    assert(array2b);
    return array2b->width;
}

/* return height of array2b
* C.R.E.: raised if array2 is NULL */
int UArray2b_height (T array2b)
{
    assert(array2b);
    return array2b->height;
}

/* return size of elements held in array2b
* C.R.E.: raised if array2 is NULL */
int UArray2b_size (T array2b)
{
    assert(array2b);
    return array2b->size;
}

/* return blocksize in array2b
* C.R.E.: raised if array2 is NULL */
int UArray2b_blocksize(T array2b)
{
    assert(array2b);
    return array2b->blocksize;
}

/*
* Return a void pointer to the element at the given column and row in
* the blocked array
* C.R.E.: raised if array2 is NULL or if array2b->blocked (UArray2_T
* element in struct) is NULL. Also if index is out of range
*/
void *UArray2b_at(T array2b, int column, int row)
{
    assert(array2b  && array2b->blocked);
    int blocksize = array2b->blocksize;

    UArray_T *block = UArray2_at(array2b->blocked, column / blocksize,
                                row / blocksize);

    return UArray_at(*block, blocksize * (column % blocksize)
                     + (row % blocksize));
}

/*
* Call the apply function in every cell. Visits every cell in one block
* before moving on to another block
* C.R.E.: raised if array2 is NULL or if array2b->blocked (UArray2_T
* element in struct) is NULL.
*/
void UArray2b_map(T array2b, void apply(int col, int row, T array2b,
                        void *elem, void *cl), void *cl)
{
    assert(array2b  && array2b->blocked);

    int width = array2b->width;
    int height = array2b->height;
    int blocksize = array2b->blocksize;

    int blocksOnWidth = ceil((double)width / blocksize);
    int blocksOnHeight = ceil((double)height / blocksize);

    for (int block_x = 0; block_x < blocksOnWidth; ++block_x) {

        for (int block_y = 0; block_y < blocksOnHeight; ++block_y) {
            UArray_T *block = UArray2_at(array2b->blocked, block_x, block_y);

            for (int arr_i = 0; arr_i < UArray_length(*block); ++arr_i) {
                int col = (arr_i % blocksize) + (block_x * blocksize);
                int row = (arr_i / blocksize) + (block_y * blocksize);

                if ( col < width && row < height && col >= 0 && row >= 0) {
                    /* only apply if current cell is in-bounds */
                    apply(col, row, array2b,
                          UArray2b_at(array2b, col, row), cl);
                }
            }
        }
    }
}
