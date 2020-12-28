/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              memory.h
 *      by Branch Seidenman (bseide01) and Emily Gjertsson (egjert01)
 *                            11/23/2020
 *
 *                            UM (hw6)
 *
 *         interface for memory.c, reponsible for register and 
 *         segment storage, and overall memory of the UM
 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "seq.h"
#include "assert.h"
#include "except.h"
#include "uarray.h"

#ifndef MEMORY_H
#define MEMORY_H

/* struct represents segmented memory in Universal Machine */
typedef struct segments *segments;

/*
* set_register takes a value and stores it in the register reg_i 
*       C.R.E. if reg_i is not an integer between 0 and 7
*/
void set_register(uint8_t reg_i, uint32_t value, segments memory);
/*
* get_register retrieves the value stored in the register reg_i 
*       C.R.E. if reg_i is not an integer between 0 and 7
*/
uint32_t get_register(uint8_t reg_i, segments memory);

/*
* init_segs allocates memory for the segments struct, initializes
*       associated sequences and sets next_index and p_counter to 0. Returns
*       the new segments struct
*/
segments init_segs();

/*
* get_seg_val takes a segments struct representing segment memory, and returns
*       the value at the given index of the segment with the given id.
* C.R.E if memory or either of the Sequences stored in the struct are NULL 
*/
uint32_t get_seg_val(segments memory, uint32_t id, uint32_t index);

/*
* set_seg_val takes a segments struct representing segment memory, and stores 
*       the given value in the segment with the given id at the given index.
* C.R.E if memory or either of the Sequences stored in the struct are NULL
*/
void set_seg_val(segments memory, uint32_t id, uint32_t index, uint32_t val);

/*
* get_next_inst goes to segment 0 in memory at the index in the program counter
*       and returns the 32 bit word at that spot, which represents the next
*       instruction.
* C.R.E if memory or either of the Sequences stored in the struct are NULL
*/
uint32_t get_next_inst(segments memory);

/*
* map_seg takes the segments struct representing segment memory and a UArray
*       that will be the newly mapped segment. Maps it to the segmented memory,
*       and stores the resulting id in rb.
* C.R.E if memory or either of the Sequences stored in the struct are NULL
*/
uint32_t map_seg(segments memory, UArray_T new);

/*
* unmap_seg frees the memory associated with the segment stored at seg_id,
*       and adds that seg_id to the unmapped segment sequence for reuse.
*       sets the previous spot in the mapped sequence to NULL until another
*       segment is mapped with that ID.
* C.R.E if memory or either of the Sequences stored in the struct are NULL
*/
void unmap_seg(segments memory, uint32_t seg_id);

/*
* load_prog takes two ints representing registers rb, and rc, and the 
*       segments struct representing segment memory. Replaces segment 0 with 
*       another segment. The  replacement segment (ID stored in rb) gets 
*       duplicated and overrides segment 0. Then starts the program counter 
*       at the index of the new segment 0 stored in register rc.
* C.R.E if memory or either of the Sequences stored in the struct are NULL
*/
void load_prog(uint8_t rb, uint8_t rc, segments memory);


/* free_segs frees all memory associated with the given segments struct 
* C.R.E if memory or either of the Sequences stored in the struct are NULL
*/
void free_segs(segments memory);


#endif