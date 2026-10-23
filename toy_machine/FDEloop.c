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

// relative path to header for branch predictors
#include "thePredictors/include/predictors.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {

        fprintf(stderr, "error: ./<my_submission> <program.asm>\n");
        return EXIT_FAILURE;
    }

    CPU cpu;
    MEM ram;
    BHT bht;
    BranchPredictors predictor; // changes values as the program runs, represents the current guess from each branch predictor, only guesses upon a instruction of type BRANCH
    MisPrediction mispredictions = {0}; // total number of mispredictions from all branch predictors
    Record record = {0}; // records number of conditional branches executed and how many of those branches were taken

    initialize_machine(&cpu);
    initialize_MEM(&ram);
    initialize_BHT(&bht); // all indices in 1BIT = 0, all indices in 2BIT = 1
    
    Instrn program[MAX_PROGRAM_SIZE]; // holds the entire program from the .asm (minus the lines consisting of labels), type struct Instrn
    LABEL labels[MAX_LABELS]; // holds all lines consisting of labels from the .asm file, ex: loop:, inner:, etc.

    int program_size;
    int label_size;
    int taken; // 1 -> if a branch is taken, 0 -> if a branch is not taken

    if (load_program(argv[1], program, labels, &program_size, &label_size) != 0) {

        fprintf(stderr, "error: failed to load a valid program\n");
        return EXIT_FAILURE;
    }

    // sends the program array full with instructions through another full iteration ...
    // every branch instruction in 'program' destination labels have been found through first parse in program_loader.c ...
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

        PC predict = {0}; // every iteration of the while loop the branchPC resets to 0, targetPC resets to 0

        fetch(&cpu, program[cpu.PC], &predict);

        Instrn IR = decode(&cpu, &predict);

        if (IR.type == BRANCH) {
            // if the current instruction sending to execute is of type BRANCH
            // find the predicition values for all of the branch predictors

            predictor.NT = NT();
            predictor.BTFN = BTFN(predict.branch, predict.target);
            predictor.BHT_1BIT = calculate_1BIT_use(predict.branch, bht.BHT_1BIT);
            predictor.BHT_2BIT = calculate_2BIT_use(predict.branch, bht.BHT_2BIT);
        }

        switch(IR.type) {

            case ALU:
                executeALU(&cpu, IR);
                break;

            case LOAD_STORE:
                executeLOADSTORE(&cpu, &ram, IR);
                break;

            case BRANCH: // returns 1 if branch is taken, 0 if branch is not taken
                taken = executeBRANCH(&cpu, IR);
                record.BRANCHES += 1;

                if (taken == 1) {

                    record.TAKEN += 1;
                }
                break;

            case DATA_MOVEMENT:
                executeDATAMOVEMENT(&cpu, IR);
                break;

            case JUMPING:
                executeJUMP(&cpu, IR);
                break;

            case HALTED:
                executeHALT(&cpu, IR);
                break;

            default:
                fprintf(stderr, "error: unknown instruction type\n");
                return -1;      
        }

        if (IR.type == BRANCH) {
            // checks if the predictions predicted by the branch predictors is correct ...
            // if not correct for a particular predictors prediction -> particular predictors misprediction variable is incremented by one
            // values in branch tables updated as well to reflect the output value of taken ...

            check_prediction(taken, &predictor, &mispredictions);
            update_1BIT_index(taken, predict.branch, bht.BHT_1BIT);
            update_2BIT_index(taken, predict.branch, bht.BHT_2BIT);
        }
    }

    for (int i = 0; i < NUM_REGISTERS; i++) {

        fprintf(stdout, "R%d=%d\n", i, cpu.R[i]);
    }

    fprintf(stdout, "BRANCHES=%d\n", record.BRANCHES);
    fprintf(stdout, "TAKEN=%d\n", record.TAKEN);
    fprintf(stdout, "MISS_NT=%d\n", mispredictions.MISS_NT);
    fprintf(stdout, "MISS_BTFN=%d\n", mispredictions.MISS_BTFN);
    fprintf(stdout, "MISS_1BIT=%d\n", mispredictions.MISS_1BIT);
    fprintf(stdout, "MISS_2BIT=%d\n", mispredictions.MISS_2BIT);

    // printf("\n");
    // for (int i = 0; i < NUM_MEM_LOCATIONS; i++) {

        // fprintf(stdout, "MEM[%d]=%d\n", i, ram.M[i]);
    // }

    // float percentage = (float)record.TAKEN / record.BRANCHES;
    // fprintf(stdout, "\npercentage of how many of the total number of branches were TAKEN: %f\n", percentage);

    return 0;
}
