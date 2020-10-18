#include <stdio.h>

struct lineInfo {
    int lineNum;
    char *fileName;
};

/*  * * * * * * * * * * * * * * the meat ;) * * * * * * * * * * * * * * */
void processFiles(int argc, char *argv[], Table_T *hashT, Seq_T *keyNames);
void run(int lineNum, char *fileName, char *cstring, Table_T *hashT,
         Seq_T *keyNames);
void findMatches(Seq_T keyNames, Table_T hashT);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/*  * * * * * Standardizing cstrings to be only word chars * * * * * * */
char *standardizeLine(char *cstring, unsigned strLength);
bool findNextValidChar(unsigned *i, unsigned strLength, char *cstring);
bool validChar(char c);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * Data Structures * * * * * * * * * * * * * */
void makeDataStructures(Table_T *hashT, Seq_T *keyNames);
const char *setAtom(char *standard_line);
struct lineInfo *initStruct(int lineNumber, char *fileName);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * * Free Memory  * * * * * * * * * * * * * * */
void freeDataStructures(Table_T *hashT, Seq_T *keyNames);
void freeBuckets(const void *atom, void **keyNames, void *cl);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

/* * * * * * * * * * * * * Helper Functions  * * * * * * * * * * * * * */
bool containsList(Table_T *hashT, char *atom);
void addNullChar(char **cstring, unsigned size);
void printMatchGroup(Seq_T matches, char *cstring);
void printLineInfo(struct lineInfo *temp);
/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */