// This file will contain all the VM methods

#include "machine.h"

/* Global Variables that can be used in this file:
    mem_u mem;
    registers reg;
    word_type;
    data_words;
*/

void print_in_assembly() {
    instruction_print_table_heading(stdout);

    for(int pc = reg.PC; pc < text_words; pc++) {
        instruction_print(stdout, pc, mem.instrs[pc]);
    }
    
    for(int i = 0; i <= data_words; i++) {
        printf("%8u: %d\t", i+reg.general[GP], mem.words[i+reg.general[GP]]);
        if((i+1)%5 == 0) printf("\n");
    }
    printf("        ...\n");
}

// Default: starts at true
int doTraceOutput = 1; 

// Returns 1 for Halt condition, and 0 otherwise.
int execute_instructions() {
    printTraceOutput();
    for(int i=0; i < text_words; i++) {
        // Invariants check
        int retVal = checkInvariants();
        if (retVal == 1) exit(retVal);

        // Fetch intruction
        bin_instr_t instruct = mem.instrs[reg.PC];
        if(doTraceOutput) printf("==>%8u: %s\n", reg.PC, instruction_assembly_form(reg.PC, instruct));

        // Update PC
        reg.PC += 1;

        // Execute Instruction
        switch(instruction_type(instruct)) {
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
                printf("Error instruction type\n");
                exit(EXIT_FAILURE);
            }
            default: {
                printf("Not a valid instruction.\n");
                exit(EXIT_FAILURE);
            }
        }
        if(doTraceOutput) {
            printTraceOutput();
        }
    }
    return 0;
}

int checkInvariants() {
    if (!(0 <= reg.general[GP]))
    {
        printf("$gp negative\n");
        return 1;
    }
    if (!(reg.general[GP] < reg.general[SP]))
    {
        printf("$gp not less than $sp\n");
        return 1;
    }
    if (!(reg.general[SP] <= reg.general[FP]))
    {
        printf("$sp not less than or equal to $fp\n");
        return 1;
    }
    if (!(reg.general[FP] < MEMORY_SIZE_IN_WORDS))
    {
        printf("$fp not less than MEMORY_SIZE_IN_WORDS\n");
        return 1;
    }
    if (!(0 <= reg.PC && reg.PC < MEMORY_SIZE_IN_WORDS))
    {
        printf("Invalid PC: %d\n", reg.PC);
        return 1;
    }
    return 0;
}

void printTraceOutput() {
    printf("      PC: %d\n", reg.PC);
    for(int i = 0; i < NUM_REGISTERS; i++) {
        printf("GPR[%s]: %-4d \t", regname_get(i), reg.general[i]);
        if((i+1)%5 == 0) printf("\n");
    }
    printf("\n");
    for(int i = 0; i <= data_words; i++) {
        printf("%8u: %d\t", i+reg.general[GP], mem.words[i+reg.general[GP]]);
        if((i+1)%5 == 0) printf("\n");
    }
    printf("        ...\n");
    for(int i = reg.general[SP]; i <= reg.general[FP]; i++) {
        if(i != reg.general[SP] && mem.words[i] == 0 && mem.words[i-1] == 0) {
            printf("        ...");
            while(mem.words[i] == 0) i++;
        }
        printf("%8u: %d\t", i, mem.words[i]);
        if((i+1)%5 == 0) printf("\n");
    }
    printf("\n\n");
}

int doComputational(comp_instr_t instruct) {
    // Ryan

    return 0;
}
int doSystemCalls(syscall_instr_t instruct) {
    switch(instruct.code) {
        case exit_sc: {
            printf("\n");
            exit(instruct.offset);
        }
        case print_str_sc: {
            printf("%s", &mem.words[reg.general[instruct.reg]+instruct.offset]);
            break;
        }
        case print_char_sc: {
            fputc(mem.words[reg.general[instruct.reg]+instruct.offset], stdout);
            break;
        }
        case read_char_sc: {
            mem.words[reg.general[instruct.reg]+instruct.offset] = getc(stdin);
            break;
        }
        case start_tracing_sc: {
            doTraceOutput = 1;
            break;
        }
        case stop_tracing_sc: {
            doTraceOutput = 0;
            break;
        }
        default: {
            printf("Invalid syscall_instr_t\n");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}
int doOtherComputation(other_comp_instr_t instruct) {

    return 0;
}
int doImmediate(immed_instr_t instruct) {
     switch(instruct.op){
        case 2: {
            
           mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)]+= machine_types_sgnExt(instruct.immed);

            break;
        }
    
        case 3: {
            
           mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] &= machine_types_zeroExt(instruct.immed);


            break;
        }
        case 4: {
            
            mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] |= machine_types_zeroExt(instruct.immed);


            break;
        }
        case 5: {
            
            mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] = ~mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] | machine_types_zeroExt(instruct.immed);


            break;
        }
        case 6: {
            
            mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] ^= machine_types_zeroExt(instruct.immed);


            break;
        }
        case 7: {
            if(reg.general[SP]==mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)]){
                reg.PC--;
                reg.PC+= machine_types_formOffset(instruct.immed);
            }
           
            break;
        }
        case 8: {
            if(mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)] >= 0){
                reg.PC--;
                reg.PC+= machine_types_formOffset(instruct.immed);
            }
           
            break;
        }
        case 9: {
            
            if(mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)] > 0){
                reg.PC--;
                reg.PC+= machine_types_formOffset(instruct.immed);
            }

            break;
        }
        case 10: {
            
            if(mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)] <= 0){
                reg.PC--;
                reg.PC+= machine_types_formOffset(instruct.immed);
            }

            break;
        }
        case 11: {
            
            if(mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)] < 0){
                reg.PC--;
                reg.PC+= machine_types_formOffset(instruct.immed);
            }

            break;
        }
        case 12: {
            
            if(mem.words[reg.general[SP]]!= mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)]){
                reg.PC--;
                reg.PC+= machine_types_formOffset(instruct.immed);
            }
           
            break;
        }
       
    }
    return 0;
}
int doJump(jump_instr_t instruct) {
 switch(instruct.op){
        case 13:{
            reg.PC = machine_types_formAddress(reg.PC -1, instruct.addr);
        }
        case 14:{
            reg.general[RA]= reg.PC;
            reg.PC = machine_types_formAddress(reg.PC -1, instruct.addr);
        }
        case 15:{
            reg.PC = reg.general[RA];
        }
    }

    return 0;
}
