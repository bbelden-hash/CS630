
#include "../include/resolve_labeler.h"

#include <stdio.h>
#include <string.h>

int resolve_labels(Instrn *program, int program_size, LABEL *labels, int label_size) {

    // iterates through all instructions in program array
    for (int p = 0; p < program_size; p++) {

        if (program[p].type != BRANCH && program[p].type != JUMPING) { // only applies to instructions of types BRANCH, JUMP
            continue;
        }

        int found = 0;

        // iterates through all labels from .asm file in labels array
        for (int l = 0; l < label_size; l++) {

            if (strcmp(program[p].L.L, labels[l].L) == 0) { // if label in program array assigned to a branch instruction equals the label in the labels array ...

                program[p].L.PC = labels[l].PC; // assign PC to branch, jump to for BRANCH
                found = 1;
                break;
            }
        }

        if (!found) {
            
            fprintf(stderr, "error: undefined label: %s\n", program[p].L.L);
            return -1;
        }
    }

    return 0;
}