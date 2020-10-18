#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "bit2.h"

const int WIDTH = 3;
const int HEIGHT = 2;

const int MARKER = 1;  /* can only be 1 or 0 */

void
check_and_print(int i, int j, Bit2_T bit_grid, int b, void *p1) 
{

        if ( (i == (WIDTH - 1) ) && (j == (HEIGHT - 1) ) ) {
                /* we got the corner */
                *((bool *)p1) &= Bit2_get(bit_grid, i, j) == b;
                *((bool *)p1) &= (b == MARKER);
        }

        printf("ar[%d,%d]: %d\n", i, j, Bit2_get(bit_grid, i, j));
}

int main(){
    Bit2_T bit_grid;
    bit_grid = Bit2_new(WIDTH, HEIGHT);
    printf("width: %d, Height: %d\n", Bit2_width(bit_grid), Bit2_height(bit_grid));

    /* Note: we are only setting a value on the corner of the array */
    Bit2_put(bit_grid, WIDTH - 1, HEIGHT - 1, MARKER);
    /* Note: &= means logical (boolean) AND of the condition
       on the right with the existing OK. OK is updated. */

    printf("Should be 0: %d\n", Bit2_get(bit_grid, 1, 1));
    Bit2_put(bit_grid, 1, 1, 1);
    printf("Should be 1: %d\n", Bit2_get(bit_grid, 1, 1));
    
    bool OK = true;
    printf("Trying column major\n");
    Bit2_map_col_major(bit_grid, check_and_print, &OK);

    printf("Trying row major\n");
    Bit2_map_row_major(bit_grid, check_and_print, &OK);

    Bit2_free(&bit_grid);

    
}