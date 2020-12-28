/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              um.c
 *      by Branch Seidenman (bseide01) and Emily Gjertsson (egjert01)
 *                            11/23/2020
 *
 *                            UM (hw6)
 *
 *        Main driver of the Universal Machine. Responsible for 
 *        reading instructions from a file, executing them 
 *        accordingly, and freeing memory.
 *            
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include "bitpack.h"
#include "assert.h"
#include "uarray.h"
#include "memory.h"
#include "instructions.h"

static uint64_t calc_file_length(FILE *fp);
static instruction init_instruction(uint32_t word);
static instruction init_loadval_inst(uint32_t word);
static uint32_t input_byte(FILE *fp);

const uint8_t HALT = 7;
const uint8_t LOADVAL = 13;

int main(int argc, char *argv[])
{
    if (argc != 2) {
        fprintf(stderr, "Missing or too many files!\n");
        exit(EXIT_FAILURE);
    }
    
    char *filename = argv[1];
    FILE *fp = fopen(filename, "rb");
    if (fp == NULL) {
        fprintf(stderr, "%s: no such file or directory\n", filename);
        exit(EXIT_FAILURE);
    }

    segments memory = init_segs();
    
    uint64_t file_length = calc_file_length(fp);
    uint64_t num_words = (file_length / 4);
    UArray_T segment0 = UArray_new(num_words, sizeof(uint32_t));

    uint32_t word = 0;

    for (uint64_t i = 0; i < num_words; i++) {
        word = input_byte(fp);
        *((uint32_t *)UArray_at(segment0, i)) = word;
    }

    fclose(fp);
    
    map_seg(memory, segment0);

    /* will continue retrieving instructions until a halt command is read */
    while (true) {
        uint32_t curr_instruct = get_next_inst(memory);
        instruction decoded = init_instruction(curr_instruct);
        if (decoded.op == HALT) {
            break;
        }
        instruction_handler(decoded, memory);
    }
    
    free_segs(memory);
    return EXIT_SUCCESS;
}

/*
* calc_file_length takes a file pointer and returns the length of the file.
*/
static uint64_t calc_file_length(FILE *fp)
{
    fseek(fp, 0, SEEK_END); 
    uint64_t length = ftell(fp); 
    fseek(fp, 0, SEEK_SET); 
    return length; 
}

/*
* input_byte takes a file pointer. Reads from a file a byte at a time and 
*       stores the data in a 32bit word. Returns the word
* C.R.E. if file pointer is NULL
*/
static uint32_t input_byte(FILE *fp)
{
    assert(fp != NULL);
    uint32_t word = 0;

    for (int i = 3; i >= 0; i--) {
        assert(!feof(fp));
        uint32_t next_byte = fgetc(fp);
        next_byte = next_byte << (8 * i);
        word += next_byte;
    }

    return word;
}

/*
* init_instruction initializes a new instruction struct from the provided
*       bitpacked uint32_t and returns the struct.
*/
static instruction init_instruction(uint32_t word)
{
    if ((Bitpack_getu(word, 4, 28)) == LOADVAL) {
        return init_loadval_inst(word);
    }
    
    instruction decoded;
    decoded.op  = Bitpack_getu(word, 4, 28);
    decoded.ra  = Bitpack_getu(word, 3, 6);
    decoded.rb  = Bitpack_getu(word, 3, 3);
    decoded.rc  = Bitpack_getu(word, 3, 0);
    decoded.val = 0;
    return decoded;
}

/*
* init_loadval_inst initializes a new instruction struct from the provided
*       bitpacked uint32_t and returns the struct. NOTE: this is unique to 
*       the loadval instruction
*/
static instruction init_loadval_inst(uint32_t word)
{
    instruction decoded;
    decoded.op  = LOADVAL;
    decoded.ra  = Bitpack_getu(word, 3, 25);
    decoded.val = Bitpack_getu(word, 25, 0);
    decoded.rb  = 0;
    decoded.rc  = 0;
    return decoded;
}
