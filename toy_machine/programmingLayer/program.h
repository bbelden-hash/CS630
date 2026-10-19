#ifndef PROGRAM_H
#define PROGRAM_H


#include "../architectureLayer/machine_state.h"

#define MAX_PROGRAM_SIZE 1000
#define NUM_OF_LABELS 20

int load_program(const char *filename, Instrn *program, LABEL *labels);

#endif