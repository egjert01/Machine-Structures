/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           sudoku.c
 *         by Minnie Xie (mxie03) and Emily Gjertsson (egjert01)
 *                           10/02/2020 (1 late token)
 *
 *                               iii
 *
 *      Determine if provided PGM is a correct representation of a
 *                          sudoku solutionn
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pnmrdr.h>

#include "except.h"
#include "assert.h"

#include "sudoku.h"
#include "uarray2.h"

Except_T CRE;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        run(stdin);

    } else if (argc == 2) {
        FILE *fp = fopen(argv[1], "rb");

        if (fp == NULL) {
            RAISE(CRE);
        }

        run(fp);
        fclose(fp);

    } else {
        RAISE(CRE);
    }

    exit(0);
}

void run(FILE *fp)
{
    assert(fp != NULL);
    Pnmrdr_T rdr = Pnmrdr_new(fp);
    Pnmrdr_mapdata mapping = Pnmrdr_data(rdr);

    if (mapping.type != Pnmrdr_gray) {
        Pnmrdr_free(&rdr);
        RAISE(CRE);
    }

    UArray2_T grid = UArray2_new(9, 9, sizeof(int));
    fillSudoku(&grid, fp, rdr);
    assert(grid != NULL);

    if (!checkGrid(grid)) {
        UArray2_free(&grid);
        Pnmrdr_free(&rdr);
        fclose(fp);
        exit(1);
    }

    UArray2_free(&grid);
    Pnmrdr_free(&rdr);
}

void fillSudoku(UArray2_T *grid, FILE *fp, Pnmrdr_T rdr)
{
    (void)fp;
    assert(grid != NULL && fp != NULL);
    UArray2_map_row_major(*grid, readFile, rdr);
}

void readFile(int width_i, int height_i, UArray2_T grid,
              void *p1, void *p2)
{
    (void)width_i;
    (void)height_i;
    (void)grid;
    *(int *)p1 = Pnmrdr_get(p2);
    if (*(int *)p1 > 9 || *(int *)p1 <= 0) {
        RAISE(CRE);
    }
}

bool checkGrid(UArray2_T grid)
{
    if (!checkLines(grid, true)
        || !checkLines(grid, false)
        || !checkAllBoxes(grid)) {
        return false;
    }
    return true;
}

bool checkLines(UArray2_T grid, bool is_row)
{
    for (int i = 0; i < 9; i++ ) {
        bool findNums[9];

        for (int j = 0; j < 9; j++) {
            findNums[j] = false;
        }

        for (int j = 0; j < 9; j++) {
            int n = numAt(grid, i, j, is_row);

            if (findNums[n - 1]) {
                return false;
            }

            findNums[n - 1] = true;
        }
    }
    return true;
}

bool checkAllBoxes(UArray2_T grid)
{
    int width_i = 0;
    int height_i = 0;
    bool isValidBox = true;

    while ( width_i != 9 && height_i != 9) {

        isValidBox = checkBox(grid, width_i, height_i);

        width_i += 3;
        height_i += 3;
    }

    return isValidBox;
}

bool checkBox(UArray2_T grid, int width_i, int height_i)
{
    for (int i = width_i; i < width_i + 3; i++) {
        bool findBoxNums[9];
        for (int j = 0; j < 9; j++) {
            findBoxNums[j] = false;
        }
        for (int j = height_i; j < height_i + 3; j++) {
            int n = *((int*)UArray2_at(grid, i, j));
            if (findBoxNums[n - 1]) {
                return false;
            }
            findBoxNums[n - 1] = true;
        }
    }
    return true;
}

int numAt(UArray2_T grid, int width_i,  int height_i, bool is_row)
{
    /* width_i and height_i are switched when checking column*/
    if (is_row) {
        return *((int*)UArray2_at(grid, width_i, height_i));
    }
    return *((int*)UArray2_at(grid, height_i, width_i));
}
