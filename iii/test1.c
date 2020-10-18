#include <stdio.h>
#include "uarray2.h"

void check_and_print(int width, int height, UArray2_T grid, void *p1, void *p2){
    long *entry_p = p1;

    *((bool *)p2) &= UArray2_at(grid, width, height) == entry_p;
    /* p2 (bool) = p2(bool) && =  */
    if ( (width == (UArray2_height(grid) - 1) ) && (height == (UArray2_width(grid) - 1) ) ) {
            /* we got the corner */
            *((bool *)p2) &= (*entry_p == 9); //don't hardcode 5 later
            /*p2 (bool) = p2(bool) && (entry_p == MARKER)*/
            /* p2 == true when corner is found */
    }

    printf("ar[%d,%d] : %c\n", width, height, *((char *)UArray2_at(grid, width, height)));
}


int main(){
    UArray2_T grid;
    grid = UArray2_new(9, 9, sizeof('c'));
    
    printf("0,0   "); *((int *)UArray2_at(grid, 0, 0)) = 'a';
    printf("AT  WORKS: %c\n", *(int*)UArray2_at(grid, 0, 0));
    
    printf("0,1   "); *((int *)UArray2_at(grid, 1, 0)) = 'b';
    printf("AT  WORKS: %c\n", *(int*)UArray2_at(grid, 1, 0));
    
    printf("0,2   "); *((int *)UArray2_at(grid, 2, 0)) = 'c';
    printf("AT  WORKS: %c\n", *(int*)UArray2_at(grid, 2, 0));
    
    printf("1,0   "); *((int *)UArray2_at(grid, 0, 1)) = 'd';
    printf("AT  WORKS: %c\n", *(int*)UArray2_at(grid, 0, 1));
    
    printf("1,1   "); *((int *)UArray2_at(grid, 1, 1)) = 'e';
    printf("AT  WORKS: %c\n", *(int*)UArray2_at(grid, 1, 1));
    
    printf("1,2   "); *((int *)UArray2_at(grid, 2, 1)) = 'f';
    printf("AT  WORKS: %c\n", *(int*)UArray2_at(grid, 2, 1));
    
    bool OK = true;
    
    printf("Trying row major\n");
    UArray2_map_row_major(grid, check_and_print, &OK);
    printf("Trying col major\n");
    UArray2_map_col_major(grid, check_and_print, &OK);


    UArray2_free(&grid);
    
}