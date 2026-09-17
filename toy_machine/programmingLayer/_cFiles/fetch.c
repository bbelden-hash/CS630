
#include "../fetch.h"

// Instrn *program is pointing to the first element in the instruction array

void fetch(CPU *cpu, Instrn *program) {

    cpu->IR = program[cpu->PC];
    cpu->PC++;

}

/*
    1) find the index in the program array that corresponds to the current PC no.
    2) assign this instruction in program to the CPU IR
    3) increment the program counter by one
    4) ...

    IR is the pass between fetch and decode
*/