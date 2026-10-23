#ifndef EXECUTIONER_H
#define EXECUTIONER_H

#include "../../architectureLayer/machine_state.h"

void executeALU(CPU *cpu, Instrn instruction);
int executeBRANCH(CPU *cpu, Instrn instruction);
void executeDATAMOVEMENT(CPU *cpu, Instrn instruction);
void executeHALT(CPU *cpu, Instrn instruction);
void executeLOADSTORE(CPU *cpu, MEM *ram, Instrn instruction);
void executeJUMP(CPU *cpu, Instrn instruction);

#endif