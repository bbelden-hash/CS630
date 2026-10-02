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
    MEM ram;
    initialize_machine(&cpu);
    initialize_MEM(&ram);
    
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

        // switch lets you choose what code to execute based on the value of one expression, instruction.type
        switch (instruction.type) {

            case ALU:
                executeALU(&cpu, instruction);
                break;

            case DATA_MOVEMENT:
                executeDATAMOVEMENT(&cpu, instruction);
                break;

            case LOAD_STORE:
                executeLOADSTORE(&cpu, &ram, instruction);
                break;

            case BRANCH:
                // executeBRANCH(&cpu, instruction);
                break;

            case HALTED:
                executeHALT(&cpu, instruction);
                break;

            default:
                fprintf(stderr, "error: unknown instruction category\n");
                return -1;
        }

    }

    for (int i = 0; i < NUM_REGISTERS; i++) {

        fprintf(stdout, "R%d=%d\n", i, cpu.R[i]);
    }

    // printf("\n");
    // for (int i = 0; i < NUM_MEM_LOCATIONS; i++) {

        // fprintf(stdout, "MEM[%d]=%d\n", i, ram.M[i]);
    // }

    return 0;
}
