/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                       instructionStruct.h
 *      by Branch Seidenman (bseide01) and Emily Gjertsson (egjert01)
 *                            11/23/2020
 *
 *                            UM (hw6)
 *
 *        Definition of the instruction struct which holds all of the 
 *        relevant information of an instruction. 
 *            
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
 
 /*
 *  op is the opcode (0 - 13)
 *  ra, rb, and rc are the corresponding register numbers
 *  val is used to store the value being loaded (is 0 in all cases where
 *      the opcode is not 13)
 */
typedef struct instruction {
    uint8_t  op;
    uint8_t  ra;
    uint8_t  rb;
    uint8_t  rc;
    uint32_t val;
} instruction;