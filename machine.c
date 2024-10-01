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
    
    int char_per_line = 0;
    int zero_flag = 0;
    for(int i = reg.general[GP]; i < reg.general[SP]; i++) {
        if(zero_flag && mem.words[i] == 0) {
            char_per_line += printf("        ...     ");
            if(char_per_line > 59) {
                printf("\n");
                char_per_line = 0;
            }
            while(mem.words[i] == 0) i++;
            if(i >= reg.general[SP]) break;
        }

        char_per_line += printf("%8u: %d\t", i, mem.words[i]);
        if(mem.words[i] == 0) zero_flag = 1;
        else zero_flag = 0;
        if(char_per_line > 59) {
            printf("\n");
            char_per_line = 0;
        }
    }
    printf("\n");
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
    printf("      PC: %d", reg.PC);
    if(reg.HI != 0 || reg.LO != 0) printf("\t      HI: %d\t      LO: %d", reg.HI, reg.LO);
    printf("\n");
    for(int i = 0; i < NUM_REGISTERS; i++) {
        printf("GPR[%s]: %-4d \t", regname_get(i), reg.general[i]);
        if((i+1)%5 == 0) printf("\n");
    }
    printf("\n");

    int char_per_line = 0;
    int zero_flag = 0;
    for(int i = reg.general[GP]; i < reg.general[SP]; i++) {
        if(zero_flag && mem.words[i] == 0) {
            char_per_line += printf("        ...     ");
            if(char_per_line > 59) {
                printf("\n");
                char_per_line = 0;
            }
            while(mem.words[i] == 0) i++;
            if(i >= reg.general[SP]) break;
        }
        
        
        char_per_line += printf("%8u: %d\t", i, mem.words[i]);
        if(mem.words[i] == 0) zero_flag = 1;
        else zero_flag = 0;
        
        if(char_per_line > 59) {
            printf("\n");
            char_per_line = 0;
        }
    }
    printf("\n");
    char_per_line = 0;
    zero_flag = 0;
    for(int i = reg.general[SP]; i <= reg.general[FP]; i++) {
        if(zero_flag && mem.words[i] == 0) {
            char_per_line += printf("        ...     ");
            if(char_per_line > 59) {
                printf("\n");
                char_per_line = 0;
            }
            while(mem.words[i] == 0) i++;
            if(i > reg.general[FP]) break;
        }
        
        char_per_line += printf("%8u: %d\t", i, mem.words[i]);
        if(mem.words[i] == 0) zero_flag = 1;
        else zero_flag = 0;
        if(char_per_line > 59) {
            printf("\n");
            char_per_line = 0;
        }
    }
    printf("\n\n");
}

int doComputational(comp_instr_t instruct) {
    switch(instruct.func){
        case 1:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.words[reg.general[SP]] + (mem.words[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]);
            break;
        }
        case 2:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.words[reg.general[SP]] - (mem.words[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]);
            break;
        }
        case 3:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.words[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)];
            break;
        }
        case 5:{
            mem.uwords[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.uwords[reg.general[SP]] & (mem.uwords[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]);
            break;
        }
        case 6:{
            mem.uwords[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.uwords[reg.general[SP]] | (mem.uwords[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]);
            break;
        }
        case 7:{
            mem.uwords[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = !(mem.uwords[reg.general[SP]] | (mem.uwords[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]));
            break;
        }
        case 8:{
            mem.uwords[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.uwords[reg.general[SP]] ^ (mem.uwords[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]);
            break;
        }
        case 9:{
            reg.general[instruct.rt] = mem.words[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)];
            break;
        }
        case 10:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = reg.general[instruct.rs];   
            break;
        }
        case 11:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = (reg.general[instruct.rs] + machine_types_formOffset(instruct.os));
            break;
        }
        case 12:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = mem.words[mem.words[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]];
            break;
        }
        case 13:{
            mem.words[reg.general[instruct.rt] + machine_types_formOffset(instruct.ot)] = -(mem.words[reg.general[instruct.rs] + machine_types_formOffset(instruct.os)]);
            break;
        }
    }

    return 0;
}
int doSystemCalls(syscall_instr_t instruct) {
    switch(instruct.code) {
        case exit_sc: {
            printf("\n");
            exit(machine_types_formOffset(instruct.offset));
        }
        case print_str_sc: {
            mem.words[reg.general[SP]] = printf("%s", &mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)]);
            break;
        }
        case print_int_sc: {
            // PINT instruction
            mem.words[reg.general[SP]] = printf("%d", mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)]);
            break;
        }
        case print_char_sc: {
            mem.words[reg.general[SP]] = fputc(mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)], stdout);
            break;
        }
        case read_char_sc: {
            mem.words[reg.general[instruct.reg]+machine_types_formOffset(instruct.offset)] = getc(stdin);
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
    switch(instruct.func){
		case 1:{
			mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] = machine_types_sgnExt(instruct.arg);
			break;
		}
		case 2:{
			reg.general[instruct.reg] = (reg.general[instruct.reg] + machine_types_sgnExt(instruct.arg));
			break;
		}
		case 3:{
			reg.general[instruct.reg] = (reg.general[instruct.reg] - machine_types_sgnExt(instruct.arg));
			break;
		}
		case 4:{
			word_type result = (mem.words[reg.general[SP]] * mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)]);
			reg.HI = result & 0xFFFF0000;
			reg.LO = result & 0x0000FFFF;
			break;
		}
		case 5:{
			reg.HI = (mem.words[reg.general[SP]]) % (mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)]);
			reg.LO = (mem.words[reg.general[SP]]) / (mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)]);
			break;
		}
		case 6:{
			mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] = reg.HI;
			break;
		}
		case 7:{
			mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] = reg.LO;
			break;
		}
		case 8:{
			mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] = (mem.uwords[reg.general[SP]] << (instruct.arg));
			break;
		}
		case 9:{
			mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)] = (mem.uwords[reg.general[SP]] >> (instruct.arg));
			break;
		}
		case 10:{
			reg.PC = mem.uwords[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)];
			break;
		}
		case 11:{
			reg.general[RA] = reg.PC;
			reg.PC = mem.words[reg.general[instruct.reg] + machine_types_formOffset(instruct.offset)];
			break;
		}
		case 12:{
			reg.PC = ((reg.PC - 1) + machine_types_formOffset(instruct.arg));
			break;
		}
	}
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
            break;
        }
        case 14:{
            reg.general[RA]= reg.PC;
            reg.PC = machine_types_formAddress(reg.PC -1, instruct.addr);
            break;
        }
        case 15:{
            reg.PC = reg.general[RA];
            break;
        }
    }

    return 0;
}
