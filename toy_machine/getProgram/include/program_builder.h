#ifndef PROGRAM_BUILDER_H
#define PROGRAM_BUILDER_H

#include "../../architectureLayer/machine_state.h"

int append_instruction(Instrn *program, int program_size, Instrn instruction);
int append_label(LABEL *labels, int label_size, LABEL label);

#endif