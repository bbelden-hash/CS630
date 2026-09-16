#include "machine_state.h"

// initializes all registers R0-R7, the program counter (PC), and HALTED to 0 from the CPU struct
void initialize_machine(CPU *cpu) {

    *cpu = (CPU){0};
}

// cpu is a pointer variable that stores a memory address to a 'CPU' struct

