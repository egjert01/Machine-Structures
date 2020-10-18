//unblackedges.h

#ifndef UNBLACKEDEDGES_H
#define UNBLACKEDEDGES_H

#include <stdio.h>
#include <stdlib.h>
#include "unblackedges.h"

#include <pnmrdr.h>
#include "bit2.h"

/*
* Name: run
* Arguments: Already open file pointer (should be a pbm file)
* Returns: none (void)
* Purpose: Main driver of the program,creates Pnmrdr_T that will be used to
*          read from the file, create the Bit2_T that will store what is read
*          from the file, then call the function to remove black edges from the
*          bitmap, print the output, and free memory for the Bit2_T bitmap and
*          the Pnmrdr_T rdr
*   NOTE: if file given is not a pbm, program will quit with a CRE
*/
void run(FILE *fp);

/*
* Name: fillBit2
* Arguments: a pointer to a 2D bit array, bitmap and the Pnmrdr_T
* Returns: none (void)
* Purpose: call the Bit2_map_row_major function to fill the bitmap with
*          what the Pnmrdr_T reads from the file
*   NOTE: bitmap must not be NULL or program quits with a Hanson assertion
*/
void fillBit2(Bit2_T *bitmap, Pnmrdr_T rdr);

/*
* Name: readFile -- an apply function for mapping
* Arguments: bitmap, A Bit2_T struct pointer, coordinates on the grid (width
*            and height), the bit stored at the location given by the
*            coordinates, and a void *p2 which represents the Pnmrdr_T rdr
* Returns: none (void)
* Purpose: the apply function called from the Bit2_map_row_major, will fill
*          the bitmap with what the Pnmrdr_T reads from the file
*/
void readFile(int width_i, int height_i, Bit2_T bitmap,
              int bit, void *p2);

/*
* Name: findBlackEdge
* Arguments: bitmap, a pointer to a Bit2_T
* Returns: none (void)
* Purpose: Use checkWidth and checkHeight to find black bits along the edge of
*          the given bitmap, and pass the coordinates into the depthFirstSearch
*          function, which will swap the given pixel and any adjacent black
*          bits to white
*   NOTE: bitmap must not be NULL or program will exit with a Hanson assertion.
*         Further, the depthFirstSearch runs supplementary checks, further
*         possibility for Hanson assertions
*/
void findBlackEdge (Bit2_T *bitmap);

/*
* Name: checkWidth
* Arguments: bitmap, a Bit2_T, two integers (passed by reference) which will be
*            set to the x and y coordinates of any found black bit, a bool
*            isTop which indicates if the function should look at the top-most
*            row or the bottom-most row, and the height of the bitmap used
*            to calculate the y value of the bottom row of the grid
* Returns: will return true if a black bit is found, false if not
* Purpose: find all black bits along the top and bottom of the bitmap
*/
bool checkWidth(Bit2_T bitmap, int *x, int *y, bool isTop, int height);

/*
* Name: checkHeight
* Arguments: bitmap, a Bit2_T, two integers (passed by reference) which will be
*            set to the x and y coordinates of any found black bit, a bool
*            isLeftCol which indicates if the function should look at the
*            left-most column or the right-most column, and the width of the
*           bitmap used to calculate the x value of the row furthest right
* Returns: will return true if a black bit is found, false if not
* Purpose: find all black bits along the top and bottom of the bitmap
*/
bool checkHeight(Bit2_T bitmap, int *x, int*y, bool isTop, int width);

/*
* Name: pbmwrite
* Arguments: a file pointer which will be stdout and bitmap, a Bit2_T
* Returns: none (void)
* Purpose: format and print the output -- must be formatted as a pbm file
*/
void pbmwrite(FILE *outputfp, Bit2_T bitmap);
/*
* Name: printElems -- an apply function for mapping
* Arguments: bitmap, a Bit2_T, coordinates that exist on the bitmap, the bit at
*            that location, a void *p2 representing the file pointer (stdout)
* Returns: none (void)
* Purpose: print the output
*/
void printElems(int width_i, int height_i, Bit2_T bitmap,
              int bit, void *p2);

#endif
