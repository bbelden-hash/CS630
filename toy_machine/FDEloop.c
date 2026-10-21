#include <stdio.h>
#include <stdlib.h>

// relative paths to header files for architecture
#include "architectureLayer/machine_state.h"
#include "architectureLayer/initial_state.h"

// relative paths to headers for program
#include "getProgram/include/program_config.h"
#include "getProgram/include/program_loader.h"
#include "getProgram/include/resolve_labeler.h"

// relative paths to header for pipeline
#include "letsPipeline/include/fetcher.h"
#include "letsPipeline/include/decoder.h"
#include "letsPipeline/include/executioner.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {

        fprintf(stderr, "error: ./<my_submission> <program.asm>\n");
        return EXIT_FAILURE;
    }

    CPU cpu;
    MEM ram;
    initialize_machine(&cpu);
    initialize_MEM(&ram);
    
    Instrn program[MAX_PROGRAM_SIZE]; // holds the entire program from the .asm (minus the lines consisting of labels), type struct Instrn
    LABEL labels[MAX_LABELS]; // holds all lines consisting of labels from the .asm file, ex: loop:, inner:, etc.

    int program_size;
    int label_size;

    if (load_program(argv[1], program, labels, &program_size, &label_size) != 0) {

        fprintf(stderr, "error: failed to load a valid program\n");
        return EXIT_FAILURE;
    }

    // sends the program array full with instructions through another full iteration ...
    // every branch instruction in 'program' destination labels have been found through first parse in program.c ...
    // finds the correct PC for execute to use in which branch instruction uses for the branch, jump
    if (resolve_labels(program, program_size, labels, label_size) != 0) {

        fprintf(stderr, "error: failed to resolve program labels\n");
        return EXIT_FAILURE;
    }

    while (!cpu.halted) {

        if (cpu.PC < 0 || cpu.PC >= program_size) {

            fprintf(stderr, "error: PC out of bounds: %d (program size: %d)\n", cpu.PC, program_size);
            return -1;
        }

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
