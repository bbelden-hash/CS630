#ifndef PROGRAM_H
#define PROGRAM_H


#include "../architectureLayer/machine_state.h"

#define MAX_PROGRAM_SIZE 1000

int load_program(const char *filename, Instrn *program);

#endif