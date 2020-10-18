#include <stdio.h>
#include <stdlib.h>

#include "assert.h" 
#include "except.h"
#include "uarray.h"
#include "uarray2.h"
#include "uarray2b.h"

void print_and_check(int col, int row, UArray2b_T array2b, void *elem, void *cl){
    (void)elem;
    *(int *)cl = *(int *)cl  + 1;
    //fprintf(stderr, "%d ", *(int *)cl);
    printf("AT  WORKS: %d\n", *((int*)UArray2b_at(array2b, col, row)));
    
    printf("col: %d, row: %d\n\n", col, row);
}

void put(int col, int row, UArray2b_T array2b, void *elem, void *cl){
    (void)elem; 
    *(int *)cl = *(int *)cl  + 1; //counter
    
    *((int *)UArray2b_at(array2b, col, row)) = *(int *)cl;
    
    // printf("col: %d, row: %d\n\n", col, row);
}


int main()
{
    UArray2b_T test = UArray2b_new(10, 10, 4, 4);
    UArray2b_width(test);
    
    // UArray2b_T testDefault = UArray2b_new_64K_block(9, 9, 16);
    // fprintf(stderr, "width is %d,  height is %d,  ", UArray2b_width(testDefault), UArray2b_height(testDefault));
    // fprintf(stderr, "size is %d,   blocksize is %d\n", UArray2b_size(testDefault), UArray2b_blocksize(testDefault));
    
    *((int *)UArray2b_at(test, 1, 2)) = 5;
    printf("AT  WORKS: %d\n", *((int*)UArray2b_at(test, 1, 2)));
    
    int counter = 0;
    UArray2b_map(test, put, &counter);

    UArray2b_map(test, print_and_check, &counter);
    
    //mapTesting(test);

    UArray2b_free(&test);
    //UArray2b_free(&testDefault);
    return 0;
}