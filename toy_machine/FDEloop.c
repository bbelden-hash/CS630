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
    
    // holds the entire program from the .asm (minus the lines consisting of labels)
    // if one 'Instrn' holds one instruction, an array of 'Instrn' ...
    Instrn program[MAX_PROGRAM_SIZE];

    // holds all lines consisting of labels from the .asm file
    // hoping for a maximum of 10 lines consisting of labels in the .asm file
    LABEL labels[NUM_OF_LABELS];

    int program_size; // could prevent fetch from running past the loaded program
    // 'program' now contains all information from the .asm (argv[1])
    program_size = load_program(argv[1], program, labels);

    if (program_size == -1) {

        fprintf(stderr, "error: could not open .asm program file in 'load_program'\n");
        return -1;
    }

    resolve_labels(program, program_size, labels, NUM_OF_LABELS);

    while (!cpu.halted) {

        if (cpu.PC < 0 || cpu.PC >= program_size) {
            fprintf(stderr, "error: PC out of bounds: %d (program size: %d)\n", cpu.PC, program_size);
            return -1;
        }

        // diagnostic
        // fprintf(stderr, "PC=%d\n", cpu.PC);

        fetch(&cpu, program[cpu.PC]);

        Instrn IR = decode(&cpu);

        switch(IR.type) {

            case ALU:
                executeALU(&cpu, IR);
                break;

            case LOAD_STORE:
                executeLOADSTORE(&cpu, &ram, IR);
                break;

            case BRANCH:
                executeBRANCH(&cpu, IR);
                break;

            case DATA_MOVEMENT:
                executeDATAMOVEMENT(&cpu, IR);
                break;

            case HALTED:
                executeHALT(&cpu, IR);
                break;

            default:
                fprintf(stderr, "error: unknown instruction type\n");
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
