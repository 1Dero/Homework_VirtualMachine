// This file will contain all the VM methods

#include "machine.h"

/* Global Variables that can be used in this file:
    mem_u mem;
    registers reg;
    word_type;
    data_words;
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

int doTraceOutput = 0;

int execute_instructions() {
    for(; reg.PC < text_words; reg.PC++) {
        bin_instr_t instruct = mem.instrs[reg.PC];
        switch(instruction_type(instruct)) {
            if(doTraceOutput) printTraceOutput();

            case comp_instr_type: {
                doComputational(instruct.comp);
                break;
            }
            case syscall_instr_type: {
                doSystemCalls(instruct.syscall);
                break;
            }
            case other_comp_instr_type: {
                doOtherComputation(instruct.othc);
                break;
            }
            case immed_instr_type: {
                doImmediate(instruct.immed);
                break;
            }
            case jump_instr_type: {
                doJump(instruct.jump);
                break;
            }
            case error_instr_type: {
                printf("Error instruction type");
                return 1;
            }
            default: {
                printf("Not a valid instruction.\n");
                return 1;
            }
        }
    }
    return 0;
}

void printTraceOutput() {

}

int doComputational(comp_instr_t instruct) {

    return 0;
}
int doSystemCalls(syscall_instr_t instruct) {

    return 0;
}
int doOtherComputation(other_comp_instr_t instruct) {

    return 0;
}
int doImmediate(immed_instr_t instruct) {

    return 0;
}
int doJump(jump_instr_t instruct) {

    return 0;
}