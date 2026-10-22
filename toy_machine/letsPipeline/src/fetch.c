
#include "../include/fetcher.h"

void fetch(CPU *cpu, Instrn currFetch, PC *predict) {

    // cpu's instruction register is set to the program at the current global PC index
    cpu->IR = currFetch;

    if (cpu->IR.type == BRANCH) { // if the instruction in the instruction register within the cpu is of operation BRANCH

        predict->branch = cpu->PC; // set the branchPC to the global PC representation of the location of the BRANCH instruction in the program array ...
    }

    cpu->PC += 1;
}