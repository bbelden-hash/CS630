
#include "../fetch.h"

// 'program' is the PC index in the program array parsed from the .asm file ...

void fetch(CPU *cpu, Instrn program) {

    cpu->IR = program;

}

/*
    1) find the index in the program array that corresponds to the current PC no.
    2) assign this instruction in program to the CPU IR
    4) ...

    IR is the pass between fetch and decode
*/