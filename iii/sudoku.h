//sudoku.h
#ifndef SUDOKU_H
#define SUDOKU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pnmrdr.h>
#include "sudoku.h"
#include "uarray2.h"

/*
* Name: run
* Arguments: (FILE *fp) file pointer to file of type PGM
* Returns: none
* Purpose: run acts as the main driver for sudoku.c . This function makes
           the pnm reader, reads in the provided file or stdin, and fills a
           new UArray2_T with those values. run then checks if the input is a
           correct sudoku solution, and exits sucessfully if so and exits
           with a failure if incorrect. The UArray2_T and pnm reader are freed.
           A checked runtime error is raised if the provided input is not a
           pgm, if the UArray2_T is NULL after filling, or if any other error
           occurs in the rest of the program
*/
void run(FILE *fp);

/*
* Name: fillSudoku
* Arguments: (UArray2_T *grid) a pointer to the new UArray2_T,
             (FILE *fp) a file pointer of given pgm,
             (Pnmrdr_T rdr) a Pnmrdr_T used to get value in file
* Returns: none
* Purpose: fillSudoku copies the values from the given file to a
           UArray2_T initialized in the program (UArray2_T grid) by using
           the UArray2_map_row_major() function. There is a CRE if the
           grid or fp is NULL.
*/
void fillSudoku(UArray2_T *grid, FILE *fp, Pnmrdr_T rdr);

/*
* Name: readFile
* Arguments: (int width_i) idex along width, (int height_i) index along
              height, (UArray2_T grid) grid containg sudoku board,
              (void *p1) a pointer to the element being mapped,
              (void *p2) a pointer to what will be changed (in this case
              only used for the Pnmrdr_T rdr)
* Returns: none
* Purpose: acts as apply function for the mapping function. Used in mapping
           function to read in each int one at a time from the given input,
           and pass the value by reference. There is a CRE if the passed
           value is not 1, 2, 3, 4, 5, 6, 7, 8, or 9.
*/
void readFile(int width_i, int height_i, UArray2_T grid, void *p1, void *p2);

/*
* Name: checkGrid
* Arguments: (UArray2_T grid) the grid holding the sudoku values
* Returns: true if the grid represents a correct sudoku solution, false if
           incorrect
* Purpose: checks whether or not the input grid represents a correct or
           incorrect sudoku solution.
*/
bool checkGrid(UArray2_T grid);

/*
* Name: checkLines
* Arguments: (UArray2_T grid) contains sudoku values,
             (bool is_row) true if function called to check
             the validity of a row, false otherwise
* Returns: true if lines (either rows or columns) are correct
           for a sudoku answer, false otherwise
* Purpose: checks whether or not rows or columns are correct
           for a sudoku solution.
*/
bool checkLines(UArray2_T grid, bool is_row);

/*
* Name: checkAllBoxes
* Arguments: (UArraay2_T grid) the grid representing the sudoku
             board
* Returns: true if boxes are valid for a correct sudoku solutionn,
           false otherwise
* Purpose: checks all the 3x3 boxes of the represented sudoku board
            and returns whether or not they all are valid for a
            correct sudoku solutionn
*/
bool checkAllBoxes(UArray2_T grid);

/*
* Name: checkBox
* Arguments: (UArray2_T grid) grid representation of sudoku board,
             (int width_i) the starting index along the width
             (int height_i) the starting  index along the height
* Returns: true if that 3x3 area of the grid represents a valid part
            of a sudoku solution, false otherwise
* Purpose: checks to see if that 3x3 area of the grid represents a
            valid part of a sudoku solution
*/
bool checkBox(UArray2_T grid, int width_i, int height_i);

/*
* Name: numAt
* Arguments: (UArray2_T grid) grid representation of sudoku board,
            (int width_i) index along width
            (int height_i) index along height
            (bool is_row) true if checking row, false if cheching column
* Returns: int representing the number stored at the specified
            coordinate in the provided grid
* Purpose: retrieves and returns the int value stored at the specified
            coordinates in the given UArray2_T grid.
*/
int numAt(UArray2_T grid, int width_i, int height_i, bool is_row);

#endif
