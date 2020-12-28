/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *  
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.  
 * 
 * Any additional functions and unit tests written for the lab go
 * here. 
 *  
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */


Um_instruction three_register(Um_opcode op, int ra, int rb, int rc)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, op);
        word = Bitpack_newu(word, 3, 6, ra);
        word = Bitpack_newu(word, 3, 3, rb);
        word = Bitpack_newu(word, 3, 0, rc);
        return word;
}

Um_instruction loadval(unsigned ra, unsigned val)
{
        Um_instruction word = 0;
        word = Bitpack_newu(word, 4, 28, 13);
        word = Bitpack_newu(word, 3, 25, ra);
        word = Bitpack_newu(word, 25, 0, val);
        return word;
}

/* Wrapper functions for each of the instructions */

static inline Um_instruction halt(void) 
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

static inline Um_instruction add(Um_register a, Um_register b, Um_register c) 
{
        return three_register(ADD, a, b, c);
}

static inline Um_instruction mul(Um_register a, Um_register b, Um_register c) 
{
        return three_register(MUL, a, b, c);
}

static inline Um_instruction div(Um_register a, Um_register b, Um_register c) 
{
        return three_register(DIV, a, b, c);
}

static inline Um_instruction nand(Um_register a, Um_register b, Um_register c) 
{
        return three_register(NAND, a, b, c);
}

static inline Um_instruction cond_move(Um_register a, Um_register b, 
                                       Um_register c) 
{
        return three_register(CMOV, a, b, c);
}

static inline Um_instruction map(Um_register b, Um_register c)
{
        return three_register(ACTIVATE, 0, b, c);
}

static inline Um_instruction unmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}

static inline Um_instruction input(Um_register c)
{
        return three_register(IN, 0, 0, c);
}

static inline Um_instruction output(Um_register c)
{
        return three_register(OUT, 0, 0, c);
}

static inline Um_instruction seg_store(Um_register a, Um_register b, 
                                       Um_register c)
{
        return three_register(SSTORE, a, b, c);
}

static inline Um_instruction seg_load(Um_register a, Um_register b, 
                                      Um_register c)
{
        return three_register(SLOAD, a, b, c);
}

static inline Um_instruction load_prog(Um_register b, Um_register c)
{
        return three_register(LOADP, 0, b, c);
}

/* Functions for working with streams */
static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}
const uint32_t Um_word_width = 32;
void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }
      
}


/* Unit tests for the UM */

void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}

void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}

void build_add_test(Seq_T stream)
{
        append(stream, loadval(r2, 99));
        append(stream, loadval(r3, 1));
        append(stream, add(r1, r2, r3));
        append(stream, output(r1)); //d
        append(stream, halt());
}

void build_print_six_test(Seq_T stream)
{
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r2, r1));
        append(stream, output(r3));
        append(stream, halt());
}

void build_mul_div_test(Seq_T stream)
{
        append(stream, loadval(r1, 10));
        append(stream, loadval(r2, 8));
        append(stream, mul(r3, r1, r2));
        append(stream, output(r3)); //P
        append(stream, loadval(r1, 900));
        append(stream, loadval(r2, 10));
        append(stream, div(r3, r1, r2)); 
        append(stream, output(r3)); //Z
        append(stream, halt());
}

void build_nand_test(Seq_T stream)
{
        append(stream, loadval(r1, 108)); 
        append(stream, loadval(r2, 110));
        append(stream, nand(r3, r1, r2)); 
        append(stream, nand(r4, r3, r3)); 
        append(stream, output(r4));
        append(stream, halt());
}

void build_cond_move_test(Seq_T stream)
{
        append(stream, loadval(r1, 100));
        append(stream, loadval(r2, 120));
        append(stream, loadval(r3, 0));
        append(stream, cond_move(r1, r2, r3)); //d
        append(stream, output(r1));
        append(stream, loadval(r3, 1));
        append(stream, cond_move(r1, r2, r3)); //x
        append(stream, output(r1));
        append(stream, halt());
}

void build_map_unmap_test(Seq_T stream)
{
        append(stream, loadval(r2, 20));
        append(stream, map(r1, r2)); //map seg length 20, id 1
        append(stream, map(r3, r2)); //map seg length 20, id 2
        append(stream, unmap(r1));   //unmap seg id 1
        append(stream, map(r1, r2)); //map seg id 1
        append(stream, unmap(r3));   //unmap seg id 2
        append(stream, map(r4, r2)); //map seg id 2
        append(stream, unmap(r4));   //unmap seg id 2
        append(stream, map(r4, r2)); //map seg id 2
        append(stream, unmap(r4));   //unmap seg id 2
        append(stream, map(r4, r2)); //map seg id 2
        append(stream, map(r5, r2)); //map seg id 3
        append(stream, unmap(r5));   //unmap seg id 3
        append(stream, map(r5, r2)); //map seg id 3
        append(stream, halt());
}

void build_io_test(Seq_T stream)
{
        append(stream, input(r1)); 
        append(stream, input(r2));
        append(stream, input(r3)); 
        append(stream, input(r4)); 
        append(stream, input(r5));
        append(stream, input(r6)); 
        append(stream, output(r1)); 
        append(stream, output(r2));
        append(stream, output(r3)); 
        append(stream, output(r4)); 
        append(stream, output(r5));
        append(stream, output(r6)); 
        append(stream, halt());
}


void build_pcounter_test(Seq_T stream)
{
        append(stream, loadval(r0, 5));
        append(stream, loadval(r1, 5));
        append(stream, add(r7, r0, r1));
        append(stream, loadval(r2, 5));
        append(stream, add(r7, r7, r2));
        append(stream, loadval(r3, 0));
        append(stream, loadval(r4, 2));
        append(stream, loadval(r5, 10));
        append(stream, cond_move(r4, r5, r1));
        append(stream, load_prog(r3, r4));
        append(stream, halt());
}


void build_loadp_segloadstore_test(Seq_T stream)
{

    append(stream, loadval(r4, 5)); //r4 holds segment length 5
    append(stream, map(r5, r4)); //new segment of length r4, stored in r5

    append(stream, loadval(r7, 128)); 
    append(stream, loadval(r0, 27525120)); 
    append(stream, mul(r0, r0, r7)); //load val instrution
    append(stream, loadval(r1, 65));
    append(stream, add(r0, r0, r1)); //load val 65 instruction for r1

    append(stream, loadval(r4, 0));
    append(stream, seg_store(r5, r4, r0)); //r5 is correct, 
    
    append(stream, loadval(r1, 28049408));
    append(stream, mul(r1, r1, r7));
    append(stream, loadval(r2, 30));
    append(stream, add(r1, r1, r2)); //load val 30 instruction for r3

    append(stream, loadval(r4, 1));
    append(stream, seg_store(r5, r4, r1));
    
    append(stream, loadval(r2, 6291456));
    append(stream, mul(r2, r2, r7));
    append(stream, loadval(r3, 139)); //registers, r2 will hold addition
    append(stream, add(r2, r2, r3)); //addition command

    append(stream, loadval(r4, 2));
    append(stream, seg_store(r5, r4, r2));

    append(stream, loadval(r4, 20971520));
    append(stream, mul(r4, r4, r7));
    append(stream, loadval(r3, 2));
    append(stream, add(r4, r4, r3)); //output command

    append(stream, loadval(r6, 3));
    append(stream, seg_store(r5, r6, r4));

    append(stream, loadval(r3, 14680064));
    append(stream, mul(r3, r3, r7)); //halt command

    append(stream, loadval(r7, 4));
    append(stream, seg_store(r5, r7, r3));
    
    append(stream, loadval(r7, 0));
    append(stream, load_prog(r5, r7));
    append(stream, halt());
}

void build_extended_map_unmap_test(Seq_T stream)
{
    append(stream, loadval(r2, 5));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    
    append(stream, loadval(r1, 1));
    append(stream, unmap(r1));
    
    append(stream, map(r1, r2));
    
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    
    append(stream, loadval(r1, 1));
    append(stream, unmap(r1));
    append(stream, loadval(r1, 2));
    append(stream, unmap(r1));
    append(stream, loadval(r1, 3));
    append(stream, unmap(r1));
    
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, map(r1, r2));
    append(stream, loadval(r1, 3));
    append(stream, unmap(r1));
    append(stream, halt());
}