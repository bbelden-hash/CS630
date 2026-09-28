
#include "../../execute.h"

void executeDATAMOVEMENT(CPU *cpu, Instrn instruction) {

     // if instruction = LOAD_I, increment the destination register value by the immediate value
    if (instruction.op == LOAD_I) {

        cpu->R[instruction.Rd] = instruction.i;
    }
    // if the instruction = MOVE, a plain register-to-register copy; place the value sitting in R[Rs] into register R[Rd], R[Rs] is left unchanged
    else if (instruction.op == MOVE) {

        cpu->R[instruction.Rd] = cpu->R[instruction.Rs];
    }

}