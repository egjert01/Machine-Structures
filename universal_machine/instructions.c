/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          instructions.c
 *      by Branch Seidenman (bseide01) and Emily Gjertsson (egjert01)
 *                            11/23/2020
 *
 *                            UM (hw6)
 *
 *        Implement instructions which emulate a Universal Machine
 *        through 32-bit instructions. NOTE: map, unmap, and load_prog 
 *        are executed primarily in the memory.c implementation
 *            
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include "instructions.h"

static void cond_move(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void seg_load(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void seg_store(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void add(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void mul(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void divide(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void nand(uint8_t ra, uint8_t rb, uint8_t rc, segments memory);
static void map_seg_inst(uint8_t rb, uint8_t rc, segments memory);
static void unmap_seg_inst(uint8_t rc, segments memory);
static void input(uint8_t rc, segments memory);
static void output(uint8_t rc, segments memory);
static void load_prog_inst(uint8_t rb, uint8_t rc, segments memory);
static void load_val(uint8_t ra, uint32_t val, segments memory);

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


void instruction_handler(instruction instructions, segments memory)
{
    assert(memory != NULL);
    uint8_t op   = instructions.op;
    uint8_t ra   = instructions.ra;
    uint8_t rb   = instructions.rb;
    uint8_t rc   = instructions.rc;
    uint32_t val = instructions.val;

    switch (op) {
        case CMOV:
            cond_move(ra, rb, rc, memory);
            return;
        case SLOAD:
            seg_load(ra, rb, rc, memory);
            return;
        case SSTORE:
            seg_store(ra, rb, rc, memory);
            return;
        case ADD:
            add(ra, rb, rc, memory); 
            return;
        case MUL:
            mul(ra, rb, rc, memory);
            return;
        case DIV:
            divide(ra, rb, rc, memory);
            return;
        case NAND:
            nand(ra, rb, rc, memory);
            return;
        case HALT:
            return;
        case ACTIVATE:
            map_seg_inst(rb, rc, memory);
            return;
        case INACTIVATE:
            unmap_seg_inst(rc, memory);
            return;
        case OUT:
            output(rc, memory);
            return;
        case IN:
            input(rc, memory);
            return;
        case LOADP:
            load_prog_inst(rb, rc, memory);
            return;
        case LV:
            load_val(ra, val, memory);
            return;
        default:
            exit(EXIT_FAILURE);
    }
}

/*
* cond_move takes three ints representing registers ra, rb, and rc. if rc is 
*       not 0, ra gets the value of rb. Returns void, no mem alloced or CREs.
*/
static void cond_move(uint8_t ra, uint8_t rb, uint8_t rc, segments memory) {
    if (get_register(rc, memory) != 0) {
        set_register(ra, get_register(rb, memory), memory);
    }
}

/*
* seg_load takes three ints representing registers ra, rb, and rc, and the 
*       segments struct representing segment memory. Accesses segment with 
*       the id stored in register rb, goes to the index (stored in rc) and
*       stores the retrieved value in register ra.
*/
static void seg_load(uint8_t ra, uint8_t rb, uint8_t rc, segments memory)
{
    uint32_t val = get_seg_val(memory, get_register(rb, memory), 
                               get_register(rc, memory));
    set_register(ra, val, memory);
}

/*
* seg_store takes three ints representing registers ra, rb, and rc, and the 
*       segments struct representing segment memory. Accesses register rc, and 
*       loads the value stored there into the segment with the id stored in 
*       register ra at the index stored in rb.
*/
static void seg_store(uint8_t ra, uint8_t rb, uint8_t rc, segments memory)
{
    set_seg_val(memory, get_register(ra, memory), 
                get_register(rb, memory), get_register(rc, memory));
}

/*
* add takes takes three ints representing registers ra, rb, and rc, and adds
*       the values in rb and rc, mods them by 2^32, and stores the result in
*       ra.
*/
static void add(uint8_t ra, uint8_t rb, uint8_t rc, segments memory)
{
    uint64_t sum = (get_register(rb, memory) + get_register(rc, memory));
    uint32_t result = sum % ((uint64_t)1 << 32);
    set_register(ra, result, memory);
}

/*
* mul takes takes three ints representing registers ra, rb, and rc, and 
*       multiplies the values in rb and rc, mods them by 2^32, and stores the 
*       result in ra.
*/
static void mul(uint8_t ra, uint8_t rb, uint8_t rc, segments memory)
{
    uint64_t product = (get_register(rb, memory) * get_register(rc, memory));
    uint32_t result = product % ((uint64_t)1 << 32);
    set_register(ra, result, memory);
}

/*
* divide takes takes three ints representing registers ra, rb, and rc, and 
*       divides the values in rb and rc, floors them, and stores the result
*       in ra. C.R.E. if a division by 0 is attempted
*/
static void divide(uint8_t ra, uint8_t rb, uint8_t rc, segments memory)
{
    assert(get_register(rc, memory) != 0);
    uint32_t result = floor((get_register(rb, memory) / 
                             get_register(rc, memory)));
    set_register(ra, result, memory);
}

/*
* nand takes takes three ints representing registers ra, rb, and rc, and 
*       performs the bitwise "and" on the values in rb and rc, "not"s them, 
*       and stores the result in ra.
*/
static void nand(uint8_t ra, uint8_t rb, uint8_t rc, segments memory)
{
    uint32_t result = ~(get_register(rb, memory) & get_register(rc, memory));
    set_register(ra, result, memory);
}

/*
* map_seg_inst takes two ints representing registers rb and rc, and the 
*       segments struct representing segment memory. It then makes a new UArray
*       of the length specified in rc, and maps it to the segmented memory,
*       and stores the resulting id in rb.
*/
static void map_seg_inst(uint8_t rb, uint8_t rc, segments memory)
{
    uint32_t length = get_register(rc, memory);
    UArray_T new = UArray_new(length, sizeof(uint32_t));
    uint32_t id = map_seg(memory, new);
    set_register(rb, id, memory);
}

/*
* unmap_seg_inst takes an int representing register rc, and the segments
*       struct representing segment memory. It then unmaps the segment stored
*       at the id stored in rc (that operation will free associated memory).
* C.R.E. if attempt to unmap segment 0
*/
static void unmap_seg_inst(uint8_t rc, segments memory)
{
    assert(get_register(rc, memory) != 0);
    uint32_t seg_id = get_register(rc, memory);
    unmap_seg(memory, seg_id);
}

/*
* input retrieves a byte of data from standard in and stores it in the 
*      given register (rc). If the end of the file is signaled, it stores 
*      ~0 in the register.
*/
static void input(uint8_t rc, segments memory)
{
    uint32_t byte = fgetc(stdin);
    if (feof(stdin)) {
        byte = ~0;
    }
    set_register(rc, byte, memory);
}

/*
* output takes the value in rc and prints it to standard out
*/
static void output(uint8_t rc, segments memory)
{
    uint32_t byte = get_register(rc, memory);
    assert(byte <= 255);
    fputc(byte, stdout);
}

/*
* load_prog_inst takes two ints representing registers rb, and rc, and the 
*       segments struct representing segment memory. Calls a function in the 
*       memory module which replaces segment 0 with another segment. The 
*       replacement segment (ID stored in rb) gets duplicated and overrides 
*       segment 0. Then starts the program counter at the index of the new 
*       segment 0 stored in register rc.
*/
static void load_prog_inst(uint8_t rb, uint8_t rc, segments memory)
{
    load_prog(rb, rc, memory);
}

/*
* load_val stores the given value in register ra
*/
static void load_val(uint8_t ra, uint32_t val, segments memory)
{
    set_register(ra, val, memory);
}