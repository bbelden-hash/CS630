#ifndef PROGRAM_LOADER_H
#define PROGRAM_LOADER_H

#include "../../architectureLayer/machine_state.h"

int load_program(const char *filename, Instrn *program, LABEL *labels, int *program_size, int *label_size);

#endif