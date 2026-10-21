
#include "../include/program_builder.h"
#include "../include/program_config.h"

#include <stdio.h>

int append_instruction(Instrn *program, int program_size, Instrn instruction) {

    if (program == NULL || program_size < 0 || program_size >= MAX_PROGRAM_SIZE) {

        fprintf(stderr, "error: program capacity exceeded\n");
        return -1;
    }

    program[program_size] = instruction;

    return 0;
}

int append_label(LABEL *labels, int label_size, LABEL label) {
    
    if (labels == NULL || label_size < 0 || label_size >= MAX_LABELS) {

        fprintf(stderr, "error: label capacity exceeded\n");
        return -1;
    }

    labels[label_size] = label;

    return 0;
}