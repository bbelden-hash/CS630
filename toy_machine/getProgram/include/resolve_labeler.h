#ifndef RESOLVE_LABELER_H
#define RESOLVE_LABELER_H

#include "../../architectureLayer/machine_state.h"

int resolve_labels(Instrn *program, int program_size, LABEL *labels, int label_size);

#endif