#ifndef EXECUTE_H
#define EXECUTE_H

#include "../architectureLayer/machine_state.h"

// execute can change the registers because cpu is a pointer to the CPU struct in memory

// instructions that require ALU use ... (ADD, SUB, AND, OR, XOR, ...)
void executeALU(CPU *cpu, Instrn instruction);

// instructions that use memory -> register, register -> memory ... (LOAD, STORE, LOAD_B, STORE_B, ...)
void executeLOADSTORE(CPU *cpu, MEM *ram, Instrn instruction);

// instructions considered a data movement/immediate instruction (LOAD_I, MOVE)
void executeDATAMOVEMENT(CPU *cpu, Instrn instruction);

// instructions requiring a comparision and (potentially) a branch, changing next execution location (BEQ, ...)
void executeBRANCH(CPU *cpu, Instrn instruction);

// HALT ...
void executeHALT(CPU *cpu, Instrn instruction);

#endif