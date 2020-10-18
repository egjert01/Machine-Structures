#ifndef UARRAY2_H
#define UARRAY2_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "uarray.h"

#define G UArray2_T
typedef struct G *G;

/*
* Name: UArray2_new
* Arguments: dimentions for the 2D array, width and height and the size of the
*            elements that are being stored
* Returns: A UArray2_T struct pointer, initialized to the given dimentions with
*          an outer array representing the width (rows) which stores UArray_Ts
*          representing the height (cols)
* Purpose: Allocate space for and Initialize a new UArray2_T struct pointer
*     NOTE: will throw  exception if width, height ot size are non-natural
*           numbers
*/
G UArray2_new(int length, int width, int size);

/*
* Name: UArray2_at
* Arguments: a pointer to the UArray2_T 2D grid, and the x and y values of the
*            location in the grid the caller is trying to access
* Returns: a void *, will return the value stored at the given location of the
*          given 2D array
* Purpose: Access an element in the grid at the given coordinates
*   NOTE: the given coordinates must be
*/
void *UArray2_at(G grid, int width, int height);

/*
* Name: UArray2_width
* Arguments: UArray2_T struct pointer
* Returns: an integer representing the width of the given UArray2_T (length of
*          outer UArray_T)
* Purpose: use the hanson UArray interface to find how wide the 2D table is
*/
int UArray2_width(G grid);

/*
* Name: UArray2_height
* Arguments: UArray2_T struct pointer
* Returns: an integer representing the height of the given UArray2_T (length of
*          inner UArray_T)
* Purpose: use the hanson UArray to find how tall the 2D table is
*/
int UArray2_height(G grid);

/*
* Name: UArray2_size
* Arguments: UArray2_T struct pointer
* Returns: an integer representing the size of the object being stored
*/
int UArray2_size(G grid);

/*
* Name: UArray2_map_row_major
* Arguments: UArray2_T struct pointer, a function pointer, and a void pointer
* Arguments for apply: UArray2_T struct pointer, coordinates for a location in
*          the grid, the element being stored at the given location in the
*          grid, and a void pointer
* Returns: none (void)
* Purpose: Traverse the 2D table in row-major order, calling the apply
*          function at every location in the table
*/
void UArray2_map_row_major(G grid, void apply(int width, int height, G uarr_2,
                          void *element, void *p2), void* cl);

/*
* Name: UArray2_map_col_major
* Arguments: UArray2_T struct pointer, a function pointer, and a void pointer
* Arguments for apply: UArray2_T struct pointer, coordinates for a location in
*          the grid, the element being stored at the given location in the
*          grid, and a void pointer
* Returns: none (void)
* Purpose: Traverse the 2D table in col-major order, calling the apply
*          function at every location in the table
*/
void UArray2_map_col_major(G grid, void apply(int width, int height, G uarr_2,
                        void *element, void *p2), void* cl);

/*
* Name: UArray2_free
* Arguments: A pointer to the UArray2_T struct pointer
* Returns: none (void)
* Purpose: free the memory associated with given UArray2_T
*      NOTE: will quit with a Hanson assertion if the given pointer is NULL
*/
void UArray2_free(G *grid);


#undef G
#endif
