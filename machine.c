// This file will contain all the VM methods

#include "machine.h"

/* 
Global Variables that can be used in this file:
    mem_u mem;
    registers reg;
    word_type text_words;
    word_type data_words;
*/

void print_instructions() {
    for(reg.PC; reg.PC < text_words; reg.PC++) {
        printf("%s\n", instruction_mnemonic(mem.instrs[reg.PC]));
    }
}

void execute_instructions() {

}