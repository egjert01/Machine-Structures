#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

void reading (FILE *fp);
char *expand(char *currString, unsigned *size);
char *standardize(char *cstring, unsigned strLength);
bool validChar(char c);
char *findNextValidChar(unsigned *i, unsigned strLength, char **wordChars, char *cstring);


int main(int argc, char *argv[]) {

    if (argc == 1) {
        reading(stdin);
    } else {
        for (int i = 1; i < argc; i++) {
            FILE *fp = fopen(argv[i], "r");
            if (fp == NULL) {
                fprintf(stderr, "%s: Could not open file %s for reading\n",
                argv[0], argv[i]);
                exit(1);
            }
            for(int i = 0; i < 12; i++){
                reading(fp);

            }

            fclose(fp);
        }
    }

    return(EXIT_SUCCESS);
}

void reading(FILE *fp)
{
    unsigned bufferSize = 5;
    unsigned stringSize = 0;
    char *cstring = malloc(bufferSize);
    char c = fgetc(fp);

    while (c != '\n'){
        if (feof(fp)){
            printf("detect end of file\n");
            return;
        }
        /* always ensure extra space available for '\0' character */
        if (stringSize == bufferSize - 1){
            cstring = expand(cstring, &bufferSize);
        }

        cstring[stringSize] = c; 
        stringSize++;
        //printf("string: %s || string size: %u || buffer size: %u\n", cstring, stringSize, bufferSize);
        c = fgetc(fp);
    }

    printf("final cstring: %s\n", cstring);
    standardize(cstring, stringSize);
    free(cstring);
}

char *standardize(char *cstring, unsigned strLength){
    char *wordChars = malloc(strLength + 1);
    unsigned newStrLength = 0;

    for (unsigned i = 0; i <= strLength; i++){
        if ((i == 0) && (validChar(cstring[i]) == false)){
            printf("First char of string = invalid\n");
            findNextValidChar(&i, strLength, &wordChars, cstring);

        } else
         if (validChar(cstring[i]) == false){
             findNextValidChar(&i, strLength, &wordChars, cstring);
             wordChars[newStrLength] = ' ';
             newStrLength++;
        } else {
            wordChars[newStrLength] = cstring[i];
            newStrLength++;
            printf("wordChars: %s\n", wordChars);
        }
    }

    //cstring = wordChars;
    free(wordChars); //TODO
    return cstring;
}
char *findNextValidChar(unsigned *i, unsigned strLength, char **wordChars, 
                        char *cstring)
{
    for (unsigned j = *i + 1; j < strLength; j++){ //TODO : what if i is the last thing?
        if (validChar(cstring[j]) == true){
            printf("found valid char: %c\n", cstring[j]);
            *i = j - 1;
            break;
        }
    }
    return *wordChars;
}

bool validChar(char c)
{
    /* 'A' - 'Z' */
    for (unsigned i = 65; i <= 90; i++){
        if (c == i){
            return true;
        }
    }
    /* 'a' - 'z' */
    for (unsigned i = 97; i <= 122; i++){
        if (c == i){
            return true;
        }
    }
    /* '0' - '9' */
    for (unsigned i = 48; i <= 57; i++){
        if (c == i){
            return true;
        }
    }
    /* '_' */
    if (c == 95){
        return true;
    }

    return false;
}


char *expand(char *currString, unsigned *size){
    // printf("\nexpanding\n\n");
    *size = *size * 2;
    currString = realloc(currString, (size_t)*size);
    return currString;
}
