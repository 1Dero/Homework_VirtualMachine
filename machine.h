#ifndef _MACHINE_H
#define _MACHINE_H

#include "machine_types.h"
#include "instruction.h"

// a size for the memory (2 ^ 16 = 32K words)
#define MEMORY_SIZE_IN_WORDS 32768

// amount of general purpose registers
#define REGISTER_SIZE 8 

// index of important registers
#define GP 0
#define SP 1
#define FP 2
#define RA 7

typedef union mem_u {
    word_type words[MEMORY_SIZE_IN_WORDS];
    uword_type uwords[MEMORY_SIZE_IN_WORDS];
    bin_instr_t instrs[MEMORY_SIZE_IN_WORDS];
}mem_u;

typedef struct registers {
    // General registers
    word_type general[REGISTER_SIZE];

    // Special registers
    word_type PC;
    word_type HI;
    word_type LO;
}registers;

// mem_u and registers global variables
extern mem_u mem;
extern registers reg;

// Number of words in text and data sections for the bof file.
extern word_type text_words;
extern word_type data_words;

// Global variable for trace output
extern int doTraceOutput;

void printTraceOutput();


// Loads the bof file's information into memory
void load_into_memory(BOFFILE bf);

// Takes an instruction and prints it in assembly language format
/* Example:
Address Instruction
    0: STRA
    1: ADDI $sp, -1, 2
    2: EXIT 0
    1024: 0         ...
*/
void print_in_assembly();

// Takes an instruction and does what it says
int execute_instructions();

// Functions for each instruction:
int doComputational(comp_instr_t instruct);
int doSystemCalls(syscall_instr_t instruct);
int doOtherComputation(other_comp_instr_t instruct);
int doImmediate(immed_instr_t instruct);
int doJump(jump_instr_t instruct);

#endif