/**************************************************************
 *
 *                     readaline.c
 *
 *     Assignment: filesnpix (hw1)
 *     Authors: Minnie Xie (mxie03) & Emily Gjertsson (egjert01)
 *     Date: 9/21/2020
 *
 *     Summary
 *          Reads a line from a file, stores it in a c-string,
 *          returns the length of that cstring
 *
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "readaline.h"
#include "except.h"

Except_T CRE;

char *expand(char *currString, unsigned *size);
bool checkFile(FILE *inputfd, char **datapp);

size_t readaline (FILE *inputfd, char **datapp)
{
    unsigned bufferSize = 200;
    unsigned index = 0;
    *datapp = (char *)malloc(bufferSize);

    if (checkFile(inputfd, datapp) == false) {
        datapp = NULL;
        return 0;
    }

    char c = fgetc(inputfd);
    /* read in line, char by char, until endline character */
    while (feof(inputfd) == 0 && ferror(inputfd) == 0) {
        if (c == '\n') {
            *(*datapp + index) = c;
            index++;
            break; /* line is finished reading, add \0 then return!*/
        }
        if (index + 2 == bufferSize) {
            *datapp = expand(*datapp, &bufferSize);
        }
        *(*datapp + index) = c;
        index++;
        c = fgetc(inputfd);
    }
    *(*datapp + index) = '\0'; /* append null char to finish the string! */
    return index;
}

/*
* Name: checkFile
* Arguments: a file pointer and the pointer to the char*, datapp
* Returns: bool, false if the end of file is reached, true otherwise.
* Purpose: Check if everything is in order with the file, if so, signal
*          that the program should continue reading.
*           NOTE: this function checks for errors, may raise a "checked
*                 runtime exception".
*/
bool checkFile(FILE *inputfd, char **datapp)
{
    if (feof(inputfd)) {
        return false; /* end of file reached */

    } else if (*datapp == NULL) {
        RAISE(CRE);

    } else if (inputfd == NULL) {
        RAISE(CRE);

    } else if (ferror(inputfd)) {
        RAISE(CRE);

    }
    return true;
}

/*
* Name: expand
* Arguments: current char *, length of current char *
* Returns: new cstring with doubled malloc space and size
* Purpose: doubles the amount of space malloc-ed for the cstring and doubles
           the size
*/
char *expand(char *currString, unsigned *size)
{
    *size = *size * 2;

    currString = realloc(currString, (size_t)*size);
    if (currString == NULL) {
        RAISE(CRE);
    }

    return currString;
}
