#ifndef EXECUTE_H
#define EXECUTE_H

#include "../architectureLayer/machine_state.h"

// execute can change the registers because cpu is a pointer to the struct in memory
void execute(CPU *cpu, Instrn instruction);

#endif