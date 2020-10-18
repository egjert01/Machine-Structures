/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                           brightness.c
 *         by Minnie Xie (mxie03) and Emily Gjertsson (egjert01)
 *                           09/20/2020
 *
 *                            filesnpix
 *
 *     Calculate the average brightness of a greyscale image (pgm)
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */


#include <stdio.h>
#include <stdlib.h>
#include <pnmrdr.h>

Except_T CRE;
Except_T badMapType = { "UNACCEPTABLE: must be pgm file" };
Except_T extraFiles = {"Too many files"};

void makePnmrdr(FILE*fp);
double calcAvgBrightness(Pnmrdr_mapdata mapping, Pnmrdr_T rdr);

int main (int argc, char *argv[])
{
    if (argc == 1) {
        makePnmrdr(stdin);

    } else if (argc == 2) {
        FILE *fp = fopen(argv[1], "rb");

        if(fp == NULL) {
            RAISE(CRE);
        }

        makePnmrdr(fp);
        fclose(fp);

    } else {
        RAISE(extraFiles);
    }

    exit(EXIT_SUCCESS);
}

/*
* Name: makePnmrdr
* Arguments: file pointer
* Returns: none
* Purpose: initialize Pnmrdr, map data, print out average, free alloc-ed
           memory (main driver)
*/
void makePnmrdr(FILE *fp)
{
    Pnmrdr_T rdr = Pnmrdr_new(fp);
    /*mapping gets and stores data from file containing image info*/
    Pnmrdr_mapdata mapping = Pnmrdr_data(rdr);

    if (mapping.type != Pnmrdr_gray) {
        Pnmrdr_free(&rdr);
        RAISE(badMapType);
    }

    fprintf(stdout, "%.3f\n", calcAvgBrightness(mapping, rdr));

    Pnmrdr_free(&rdr);
}

/*
* Name: calcAvgBrightness
* Arguments: Pnmrdr_mapdata (struct with file information) and the Pnmrdr_T rdr
* Returns: double representing the calculated average brightness
* Purpose: calculate the average brightness.
*/
double calcAvgBrightness(Pnmrdr_mapdata mapping, Pnmrdr_T rdr)
{
    unsigned pixelCount = mapping.width * mapping.height;
    double sum = 0;

    for (unsigned i = 0; i < pixelCount; i++) {
        sum = sum + Pnmrdr_get(rdr) / (double)mapping.denominator;
    }

    return sum / pixelCount;

}
