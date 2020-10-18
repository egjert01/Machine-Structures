#include <stdio.h>
#include <stdlib.h>
#include "readaline.h"

//size_t readaline (FILE *inputfd, char **datapp){

int main(int argc, char *argv[])
{
    char *lineInfo = NULL; //ask TA
    size_t numChars = 0;

        if (argc == 1) {
            numChars = readaline(stdin, &lineInfo);
        } else {
            for (int i = 1; i < argc; i++) {
                FILE *fp = fopen(argv[i], "r");
                if (fp == NULL) {
                    fprintf(stderr, "%s: Could not open file %s for reading\n",
                    argv[0], argv[i]);
                    exit(1);
                }
                for (int i = 0; i < 14; i++){
                    numChars = readaline(fp, &lineInfo);
                    printf("\nLINEINFO: %s\n", lineInfo);
                    printf("NUMCHARS: %lu\n", numChars);

                    free(lineInfo);
                    if (numChars == 0){
                        printf("breaking\n");
                        break;
                    }

                }

                fclose(fp);
            }
        }

        return(EXIT_SUCCESS);
}
