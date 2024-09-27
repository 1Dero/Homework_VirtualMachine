// This file with read the command and the .bof file, and call VM functions as appropriate

#include "machine.h"

// Global Variables declaration
mem_u mem;
registers reg;
word_type text_words;
word_type data_words;

void load_into_memory(BOFFILE bf) {
    BOFHeader header = bof_read_header(bf);
    text_words = header.text_length;
    data_words = header.data_length;

    reg.PC = 0;
    reg.general[GP] = header.data_start_address;
    reg.general[SP] = header.stack_bottom_addr;
    reg.general[FP] = reg.general[SP];
    for(int pc = reg.PC; pc < text_words; pc++) { 
        mem.instrs[pc] = instruction_read(bf);
    }
    for(int gp = reg.general[GP]; gp < (reg.general[GP]+header.data_length); gp++) {
        mem.words[gp] = bof_read_word(bf);
    }
}

int main(int argc, char *argv[]) {
    BOFFILE bf;
    if(strcmp("-p", argv[1]) == 0) {
        // Load the .bof file and print what's in it, then exit.
        printf("Address Instruction\n");
        bf = bof_read_open(argv[2]);
        load_into_memory(bf);

        print_in_assembly();
    }
    else {
        // Execute instructions in .bof file.
        bf = bof_read_open(argv[1]);
        load_into_memory(bf);

        execute_instructions();
    }


    return EXIT_SUCCESS;
}