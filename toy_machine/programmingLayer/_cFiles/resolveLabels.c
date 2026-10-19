
#include <stdio.h>
#include <string.h>
#include "../decoder.h"

int resolve_labels(Instrn *program, int program_size, LABEL *labels, int label_size) {

    for (int p = 0; p < program_size; p++) {

        if (program[p].type != BRANCH) {
            continue;
        }

        int found = 0;

        for (int l = 0; l < label_size; l++) {

            if (strcmp(program[p].L.L, labels[l].L) == 0) {
                program[p].L.PC = labels[l].PC;
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