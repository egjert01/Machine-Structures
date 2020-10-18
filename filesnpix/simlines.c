/**************************************************************
 *
 *                     simlines.c
 *
 *     Assignment: filesnpix (hw1)
 *     Authors:  Minnie Xie (mxie03) & Emily Gjertsson (egjert01)
 *     Date:     9/21/2020
 *
 *     Summary:
 *          Stores file(s) info, finds matching lines, and prints
 *          the matching lnes out.
 *
 **************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "atom.h"
#include "table.h"
#include "except.h"
#include "seq.h"
#include "readaline.h"
#include "simlines.h"

Except_T CRE;

int main(int argc, char *argv[])
{
    if (argc == 1) {
        RAISE(CRE);

    } else {
        Table_T hashTable = NULL;
        Seq_T keyNames = NULL;
        makeDataStructures(&hashTable, &keyNames);
        processFiles(argc, argv, &hashTable, &keyNames);
        findMatches(keyNames, hashTable);
        freeDataStructures(&hashTable, &keyNames);
    }
    exit(EXIT_SUCCESS);
}

/*  * * * * * * * * * * * * * * main drivers  * * * * * * * * * * * * * * */

void processFiles(int argc, char *argv[], Table_T *hashT, Seq_T *keyNames)
{
    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");

        if (fp == NULL) {
            RAISE(CRE);
        }

        int lineNumber = 0;
        size_t strLength = 0;
        char *cstring = NULL;

        while (!feof(fp) && !ferror(fp)) {
            strLength = readaline(fp, &cstring);
            lineNumber++;
            cstring = makeStandardLine(cstring, strLength);
            if (cstring != NULL) {
                storeLine(lineNumber, argv[i], cstring, hashT, keyNames);
            }
            free(cstring);
        }
        fclose(fp);
    }
}

void storeLine(int lineNumber, char *fileName, char *cstring, Table_T *hashT,
        Seq_T *keyNames)
{
    struct lineInfo *newLine = NULL;
    newLine = initStruct(lineNumber, fileName);
    if (newLine == NULL) {
        RAISE(CRE);
    }

    const char *newAtom = setAtom(cstring);

    if (containsSeq(hashT, (char *)newAtom)) {
        /* already a Seq. in the bucket -- found a line match! */
        Seq_T matchSet = Table_get(*hashT, newAtom);
        Seq_addhi(matchSet, newLine); /* add a new struct to existing seq */
    } else {
        /* must make a new Seq, first instance of a line */
        Seq_T newMatchSet = Seq_new(0);
        Seq_addhi(newMatchSet, newLine); /* add a new struct to new seq */
        Table_put(*hashT, newAtom, newMatchSet);

        Seq_addhi(*keyNames, (void *)newAtom); /* add the atom to keyNames */
    }
}

void findMatches(Seq_T keyNames, Table_T hashT)
{
    Seq_T currSeq;
    int len = Seq_length(keyNames);
    int firstMatch = 0;

    for (int i = 0; i < len; i++) {
        /*find and print first match group */
        currSeq = Table_get(hashT, (char *)Seq_get(keyNames, i) );
        if  (Seq_length(currSeq) > 1) { /*If the line has matches*/
            firstMatch = i;
            printMatchGroup(currSeq, (char *)Seq_get(keyNames, i));
            break;
        }
    }
    for (int i = firstMatch + 1; i < len; i++) {
        /* print remaining match groups */
        currSeq = Table_get(hashT, (char *)Seq_get(keyNames, i) );
        if  (Seq_length(currSeq) > 1) { /*If the line has matches*/
            printf("\n");
            printMatchGroup(currSeq, (char *)Seq_get(keyNames, i));
        }
    }
}

/*  * * * * *Standardizing cstrings to be only word chars  * * * * * */
char *makeStandardLine(char *cstring, unsigned strLength)
{
    /*malloc-ing an extra 2 spaces to accomodate a '\n' and a '\0'*/
    char *newCString =(char *)malloc(strLength + 2);
    if (newCString == NULL) {
        RAISE(CRE);
    }
    unsigned newStrLength = 0;

    /*filling newCString with reformatted line*/
    newCString = standardizeLine(&newStrLength, strLength, cstring,
                                   newCString);

    /*If the line is empty, free memory and return without adding '\0'*/
    if (newStrLength == 0) {
        free(cstring);
        free(newCString);
        return NULL;
    }
    /*add a '\0' to make string a valid c-string*/
    addNullChar(&newCString, newStrLength);
    free(cstring);
    return newCString;
}

char *standardizeLine(unsigned *newStrLength, unsigned strLength, char
                        *cstring, char *newCString)
{
    /* Go through the cstring, character by character */
    for (unsigned i = 0; i <= strLength; i++) {
        /* invalid first char. Continue until next valid char is found */
        if ((i == 0) && (isValidChar(cstring[i]) == false)) {
            findNextValidChar(&i, strLength, cstring);

        } else if (isValidChar(cstring[i]) == false) { /* found invalid char */
            if (findNextValidChar(&i, strLength, cstring) == true) {
                /* Add a space if another valid char exists after*/
                newCString[*newStrLength] = ' ';
                (*newStrLength)++;
            }

        } else {
            newCString[*newStrLength] = cstring[i];
            (*newStrLength)++;

        }
    }

    return newCString;
}

bool findNextValidChar(unsigned *i, unsigned strLength, char *cstring)
{
    for (unsigned j = *i + 1; j < strLength; j++) {
        if (isValidChar(cstring[j]) == true) {
            *i = j - 1; /* send back index of next valid char */
            return true; /* valid char is found */
        }
    }
    return false; /* no valid char found */
}

bool isValidChar(char c)
{
    /* return true if found word char (as specified in the spec.) */
    if ( c >= 'A' && c <= 'Z') {
        return true;
    }

    if (c >= 'a' && c <= 'z') {
        return true;
    }

    if (c >= '0' && c <= '9') {
        return true;
    }
    if (c == '_') {
        return true;
    }

    return false;
}

/* * * * * * * * * * * * * Data Structures * * * * * * * * * * * * * */
void makeDataStructures(Table_T *hashT, Seq_T *keyNames)
{
    (*hashT) =  Table_new(0, NULL, NULL);
    (*keyNames) = Seq_new(0);

}

const char *setAtom(char *standard_line)
{
    const char *atom = Atom_string(standard_line);
    return atom;
}

struct lineInfo *initStruct(int lineNumber, char *fileName)
{
    struct lineInfo *newStruct = NULL;
    newStruct = malloc(sizeof(struct lineInfo));

    if (newStruct == NULL) {
        RAISE(CRE);
    }

    newStruct->lineNum = lineNumber;
    newStruct->fileName = fileName;

    return newStruct;
}

/* * * * * * * * * * * * * * * Printing  * * * * * * * * * * * * * * * */
void printMatchGroup(Seq_T matches, char *cstring)
{
    struct lineInfo *temp;
    printf("%s\n", cstring);

    for (int i = 0; i < Seq_length(matches); i++) {
        temp = Seq_get(matches, i);
        printLineInfo(temp);
    }
}

void printLineInfo(struct lineInfo *temp)
{
    char *cStrFileName = temp->fileName;
    size_t fileLen = strlen(cStrFileName);
    /*lenDiff calculates how many spaces to add after the filename*/
    int lenDiff  = 20 - fileLen;
    if (lenDiff > 0) {
        /* make cstring holding spaces, print if filename < 20 chars */
        char *addSpaces = malloc(lenDiff + 1);

        if (addSpaces == NULL) {
            RAISE(CRE);
        }

        for  (int j = 0; j < lenDiff; j++) {
            *(addSpaces + j) = ' ';
            addNullChar(&addSpaces, j + 1);
        }

        printf("%s%s", cStrFileName, addSpaces);
        printf(" %7u\n", temp->lineNum);
        free(addSpaces);

    } else { /*file name is longer than 20 chars*/
        printf("%s", cStrFileName);
        printf(" %7u", temp->lineNum);
    }
}

/* * * * * * * * * * * * * * Free Memory  * * * * * * * * * * * * * * */
void freeDataStructures(Table_T *hashT, Seq_T *keyNames)
{
    Table_map(*hashT, freeBuckets, NULL);
    Table_free(hashT);
    hashT = NULL;
    Seq_free(keyNames);
}

void freeBuckets(const void *key, void **value, void *cl)
{
    /*these paramaters needed for Hanson functions, but never used*/
    (void)cl;
    (void)key;

    Seq_T structSeq = *((Seq_T*)value);

    /* remove structs from the seq, from the back to the front*/
    for (int i = Seq_length(structSeq) - 1; i >= 0 ; i--) {
        struct lineInfo *rmvStruct = Seq_remhi(structSeq);
        if ( rmvStruct == NULL) {
            RAISE(CRE);
        } else {
            free(rmvStruct);
        }
    }
    Seq_free(&structSeq);
}

/* * * * * * * * * * * * * Helper Functions  * * * * * * * * * * * * * */
bool containsSeq(Table_T *hashT, char *atom)
{
    if (Table_get(*hashT, atom) != NULL) {
        return true;
    }
    return false;
}

void addNullChar(char **cstring, unsigned size)
{
    *(*cstring + size) = '\0';
}
