#ifndef UTILS_H
#define UTILS_H

#include "../architectureLayer/machine_state.h"

int wrapAddress(int address);
int data_hazard(IF_ID *id, ID_EX *ex, EX_MEM *mem);

#endif