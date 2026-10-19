#ifndef DECODER_H
#define DECODER_H

#include "../architectureLayer/machine_state.h"

Instrn decode(CPU *cpu);

int resolve_labels(Instrn *program, int program_size, LABEL *labels, int label_size);

#endif