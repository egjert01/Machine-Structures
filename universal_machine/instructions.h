/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                          instructions.h
 *      by Branch Seidenman (bseide01) and Emily Gjertsson (egjert01)
 *                            11/23/2020
 *
 *                            UM (hw6)
 *
 *        Interface of instructions.c, which emulates a Universal Machine
 *        through 32-bit instructions.
 *            
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "instructionStruct.h"
#include "memory.h"
#include "assert.h"

#ifndef INSTRUCTIONS_H
#define INSTRUCTIONS_H

/*
* instruction_handler takes an instruction struct (which stores opcode,
*       registers, and value for loadval), and segments struct representing
*       segmented memory. Responsible for executing the given instruction.
*/
void instruction_handler(instruction instructions, segments memory);

#endif