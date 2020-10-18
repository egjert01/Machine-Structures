/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          unblackedges.c
 *         by Minnie Xie (mxie03) and Emily Gjertsson (egjert01)
 *                           10/02/2020
 *
 *                              iii
 *
 *         Finds and replaces black edge of pbm to a white edge
 *                   Prints out resulting pbm
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include <stdio.h>
#include <stdlib.h>
#include "unblackedges.h"
#include "unblackDFS.h"

#include <pnmrdr.h>
#include "bit2.h"
#include "except.h"
#include "assert.h"

Except_T CRE;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        run(stdin);

    } else if (argc == 2) {
        FILE *fp = fopen(argv[1], "rb");

        assert(fp != NULL);

        run(fp);
        fclose(fp);

    } else {
        RAISE(CRE);
    }

    exit(EXIT_SUCCESS);
}

void run(FILE *fp)
{
    Pnmrdr_T rdr = Pnmrdr_new(fp);
    Pnmrdr_mapdata mapping = Pnmrdr_data(rdr);

    if (mapping.type != Pnmrdr_bit) {
        Pnmrdr_free(&rdr);
        RAISE(CRE);
    }
    int width = mapping.width;
    int height = mapping.height;

    assert(width > 0);
    assert(height > 0);

    Bit2_T bitmap = Bit2_new(width, height);
    fillBit2(&bitmap, rdr);

    findBlackEdge(&bitmap);
    pbmwrite(stdout, bitmap);

    Bit2_free(&bitmap);
    Pnmrdr_free(&rdr);
}

void fillBit2(Bit2_T *bitmap, Pnmrdr_T rdr)
{
    assert(bitmap != NULL);
    Bit2_map_row_major(*bitmap, readFile, rdr);
}

void readFile(int width_i, int height_i, Bit2_T bitmap,
              int bit, void *p2)
{
    bit = Pnmrdr_get(p2);
    Bit2_put(bitmap, width_i, height_i, bit);
}

void findBlackEdge (Bit2_T *bitmap)
{
    assert(bitmap != NULL);

    int width = Bit2_width(*bitmap);
    int height = Bit2_height(*bitmap);

    for (int y = 0; y < height; y++) {
        int x = -1;
        if (checkHeight(*bitmap, &x, &y, true, width)) { /*check left col*/
            depthFirstSearch(bitmap, x, y);
        }
        if (checkHeight(*bitmap, &x, &y, false, width)) { /*check right col*/
            depthFirstSearch(bitmap, x, y);
        }
    }

    for (int x = 0; x < width; x++) {
        int y = -1;
        if (checkWidth(*bitmap, &x, &y, true, height)) {/*check top row*/
            depthFirstSearch(bitmap, x, y);
        }
        if (checkWidth(*bitmap, &x, &y, false, height)) {/*check bottom row*/
            depthFirstSearch(bitmap, x, y);
        }
    }
}

bool checkWidth(Bit2_T bitmap, int *x, int *y, bool isTop, int height)
{
    int bit = -1;
    if (isTop) {
        bit = Bit2_get(bitmap, *x, 0);
        if (bit == 1) {
            *y = 0;
            return true;
        }
    } else {
        bit = Bit2_get(bitmap, *x, height - 1);
        if (bit == 1) {
            *y = height - 1;
            return true;
        }
    }
    return false;
}

bool checkHeight(Bit2_T bitmap, int *x, int *y, bool isLeftCol, int width)
{
    int bit = -1;
    if (isLeftCol) {
        bit = Bit2_get(bitmap, 0, *y);
        if (bit == 1) {
            *x = 0;
            return true;
        }
    } else {
        bit = Bit2_get(bitmap, width - 1, *y);
        if (bit == 1) {
            *x = width - 1;
            return true;
        }
    }
    return false;
}

void pbmwrite(FILE *outputfp, Bit2_T bitmap)
{
    int width = Bit2_width(bitmap);
    int height = Bit2_height(bitmap);

    fprintf(outputfp, "P1\n#removed black edges\n%d %d\n", width, height);
    Bit2_map_row_major(bitmap, printElems, outputfp);
}

void printElems(int width_i, int height_i, Bit2_T bitmap,
             int bit, void *p2)
{
    (void)height_i;

    fprintf((FILE *)p2, "%d ", bit);

    if (width_i == Bit2_width(bitmap) - 1) {
        printf("\n");
    }
}
