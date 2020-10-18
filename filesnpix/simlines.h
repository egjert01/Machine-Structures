/**************************************************************
 *
 *                     simlines.h
 *
 *     Assignment: filesnpix (hw1)
 *     Authors:  Minnie Xie (mxie03) & Emily Gjertsson (egjert01)
 *     Date:     9/21/2020
 *
 *     Summary:
 *          Header file for simlines.c (declares struct and functions)
 *
 **************************************************************/


struct lineInfo {
    int lineNum;
    char *fileName;
};

/*  * * * * * * * * * * * * * * main drivers * * * * * * * * * * * * * * */
/*
* Name: processFiles
* Arguments: Number of args passed into main and the arguments themselves,
*           the hanson table that will hold the line information, and the
*           hanson sequence that will hold the cstrings representing the lines
* Returns: none (void)
* Purpose: opens file(s) passed by user, checks for errors, then calls the
*          readaline function, and passes that to the storeLine function
*/
void processFiles(int argc, char *argv[], Table_T *hashT, Seq_T *keyNames);

/*
* Name: storeLine
* Arguments: line length, filename, line read in, hashTable, list of all lines
* Returns: none
* Purpose: fills all data structures with file info
*/
void storeLine(int lineNum, char *fileName, char *cstring, Table_T *hashT,
         Seq_T *keyNames);

 /*
 * Name: findMatches
 * Arguments: the Sequence holding the atoms and the hash table storing
 *            the line information
 * Returns: none (void)
 * Purpose: Main driver for match finding and printing. Goes through the
 *          sequence holding the atoms of the lines from the file, use them
 *          to go through the hash table. If a matchgroup is found, call
 *          printMatchGroup.
 */
 void findMatches(Seq_T keyNames, Table_T hashT);


/*  * * * * * Standardizing cstrings to be only word chars * * * * * * */
/*
* Name: makeStandardLine
* Arguments: line read in from file, length of line
* Returns: line formatted with only word-chars separated by signle spaces as
           a cstring
* Purpose: make space for new formatted line, standardize format of line to
           be matched correctly, and free original line space
*/
char *makeStandardLine(char *cstring, unsigned strLength);

/*
* Name: standardizeLine
* Arguments: pointer to reformatted line length, original line length,
             original line, new reformatted line
* Returns: line formatted with only word-chars separated by signle spaces but
           no '\0' char added for nonempty lines
* Purpose: standardize format of lines to be matched correctly
*/
char *standardizeLine(unsigned *newStrLength, unsigned strLength,
                        char *cstring, char *newCString);

/*
* Name: findNextValidChar
* Arguments: a pointer to the index (of the cstring), the length of the
*           cstring, and the string (char *).
* Returns: Boolean, returns true if a valid char is found, false if not.
* Purpose: starting at the passed in index, continue to move through the
*          cstring until the next valid char is found and update the index.
*/
bool findNextValidChar(unsigned *i, unsigned strLength, char *cstring);

/*
* Name: isvalidChar
* Arguments: a character
* Returns: true/false
* Purpose: deterrmines if the input char is a word-char or not
*/
bool isValidChar(char c);

/* * * * * * * * * * * * * Data Structures * * * * * * * * * * * * * */
/*
* Name: makeDataStructures
* Arguments: The hanson Table storing lineInfo, and the sequence holding the
*            atoms (representing the cstrings from the file) (passed by
*            reference)
* Returns: none (void)
* Purpose: initialize the table and the sequence
*/
void makeDataStructures(Table_T *hashT, Seq_T *keyNames);

/*
* Name: setAtom
* Arguments: a string holding the standardized line
* Returns: an Atom
* Purpose: make an atom point to the string, return the atom
*/
const char *setAtom(char *standard_line);

/*
* Name: initStruct
* Arguments: line number in file, name of host file
* Returns: pointer to a struct called lineInfo
* Purpose: create a struct that contains the arguments and allocs its memory
*/
struct lineInfo *initStruct(int lineNumber, char *fileName);

/*  * * * * * * * * * * * * * * Printing * * * * * * * * * * * * * * * */
/*
* Name: printMatchGroup
* Arguments: sequence of matches, standardized line that has matches
* Returns: none
* Purpose: prints out a matched line and file information of where line
          is located
*/
void printMatchGroup(Seq_T matches, char *cstring);

/*
* Name: printLineInfo
* Arguments: struct of current matchline instance's info
* Returns: nothing
* Purpose: Prints out a single formatted line containing filename and line
           number of that specific instance of the matched line
*/
void printLineInfo(struct lineInfo *temp);

/* * * * * * * * * * * * * * Free Memory  * * * * * * * * * * * * * * */
/*
* Name: freeDataStructures
* Arguments: The table holding the line information, and the sequence storing
*            the atoms (representing lines from the file)
* Returns: none (void)
* Purpose: Free memory associated with the Hanson table and the sequence
*/
void freeDataStructures(Table_T *hashT, Seq_T *keyNames);

/*
* Name: freeBuckets
* Arguments: follows the parameters for the Hanson "apply" function used by
*            Table_map. Takes a pointer to the key (an atom), a double pointer
*            to the value the key is paired with, and a pointer to the cl.
*            NOTE: key and cl are voided in the function
* Returns: none (void)
* Purpose: free the memory associated with the structs in every sequence
*          and then free the sequence
*/
void freeBuckets(const void *key, void **value, void *cl);

/* * * * * * * * * * * * * Helper Functions  * * * * * * * * * * * * * */
/*
* Name: containsSeq
* Arguments: hashTable, atom (cstring)
* Returns: true/false
* Purpose: determines whether or not the hash table contains the specified
           atom
*/
bool containsSeq(Table_T *hashT, char *atom);

/*
* Name: addNullChar
* Arguments: pointer to a cstring, the length of the cstring
* Returns: none
* Purpose: adds a '\0' char to the end of the specified cstring
*/
void addNullChar(char **cstring, unsigned size);
