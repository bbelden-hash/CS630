#include <stdio.h>

// relative paths to header files
#include "architectureLayer/machine_state.h"
#include "architectureLayer/initial_state.h"

#include "programmingLayer/fetch.h"
#include "programmingLayer/decoder.h"
#include "programmingLayer/execute.h"
#include "programmingLayer/program.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {

        fprintf(stderr, "error: ./<my_submission> <program.asm>\n");
        return -1;
    }

    CPU cpu;
    initialize_machine(&cpu);

    // holds the entire program from the .asm
    // if one 'Instrn' holds one instruction, an array of 'Instrn' ...
    Instrn program[MAX_PROGRAM_SIZE];

    int program_size; // could prevent fetch from running past the loaded program
    // 'program' now contains all information from the .asm (argv[1])
    program_size = load_program(argv[1], program);

    if (program_size == -1) {

        fprintf(stderr, "error: could not open .asm program file in 'load_program'\n");
        return -1;
    }

    while (!cpu.halted) {

        // fetch the program at index PC in the program array of instructions
        // puts the whole Instrn into cpu.IR and increment PC++
        fetch(&cpu, program);

        // returns the fetched instruction above
        Instrn instruction = decode(cpu.IR);

        // perform the proper operations to the instruction from decode
        execute(&cpu, instruction);
    }

    for (int i = 0; i < NUM_REGISTERS; i++) {

        fprintf(stdout, "R%d=%d\n", i, cpu.R[i]);
    }

    return 0;
}
