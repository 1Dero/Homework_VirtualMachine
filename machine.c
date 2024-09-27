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

int execute_instructions() {
    for(; reg.PC < text_words; reg.PC++) {
        bin_instr_t instruct = mem.instrs[reg.PC];
        switch(instruction_type(instruct)) {
            case comp_instr_type: {
                // printf("comp_instr_t\n");
                break;
            }
            case syscall_instr_type: {
                // printf("syscall_instr_type\n");
                break;
            }
            case other_comp_instr_type: {
                // printf("other_comp_instr_type\n");
                break;
            }
            case immed_instr_type: {
                // printf("immed_instr_type\n");
                break;
            }
            case jump_instr_type: {
                // printf("jump_instr_type\n");
                break;
            }
            case error_instr_type: {
                // printf("error_instr_type\n");
                break;
            }
            default: {
                printf("Not a valid instruction.\n");
                return 1;
            }
        }
    return 0;
    }
}