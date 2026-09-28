
#include "../../execute.h"

void executeHALT(CPU *cpu, Instrn instruction) {

    if (instruction.op == HALT) {

        cpu->halted = 1;
    }
}