
// change to correct path of 'machine_state.h' pending on where the header file lives
#include "/workspaces/CS630/toy_machine/architectureLayer/machine_state.h"

void fetch(CPU *cpu) {

    cpu->IR = cpu->R[cpu->PC];
    cpu->PC++;

}

/*
    1) look at the PC
    2) send the PC's address to memory -- cpu->R[cpu->PC]
    3) memory returns the instruction to be decoded
    4) put instruction in instruction register
    5) advance PC by one
*/