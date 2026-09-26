#include "machine_state.h"

// initializes all registers R0-R7, the program counter (PC), and HALTED to 0 from the CPU struct
void initialize_machine(CPU *cpu) {

    *cpu = (CPU){0};
}

// initializes all locations in the 'ram' array to 0 from the MEM struct
void initialize_MEM(MEM *ram) {

    *ram = (MEM){0};
}

// cpu is a pointer variable that stores a memory address to a 'CPU' struct
// ram is a pointer variable that stores a memory address to a 'MEM' struct

