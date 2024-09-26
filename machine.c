// This file will contain all the VM methods

#include "machine.h"

/* 
Global Variables that can be used in this file:
    mem_u mem;
    registers reg;
    word_type text_words;
    word_type data_words;
*/

void print_in_assembly() {
    for(int pc = reg.PC; pc < text_words; pc++) {
        printf("    %2d: %s\n", pc, instruction_assembly_form(pc, mem.instrs[pc]));
    }
    
    for(int i = 0; i <= data_words; i++) {
        printf("    %4d: %d\t", i+reg.general[GP], mem.words[i+reg.general[GP]]);
        if((i+1)%5 == 0) printf("\n");
    }
    printf("    ...\n");
}

void execute_instructions() {

}