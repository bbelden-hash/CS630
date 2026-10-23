
#include "../include/executioner.h"

int executeBRANCH(CPU *cpu, Instrn instruction) {

    int taken = 0;

    // opcode == 'BEQ'; if R[Rs1] == R[Rs2], move to an instruction specified by 'L' (R[L]) by changing the PC to L
    if (instruction.op == BEQ) {

        if (instruction.branched) {
            taken = 1;

            cpu->PC = instruction.L.PC;
        }
    }
    // opcode == 'BNE'; if R[Rs1] != R[Rs2], move to the instruction pointed to by instruction.L.PC ...
    else if (instruction.op == BNE) {

        if (instruction.branched) {
            taken = 1;

            cpu->PC = instruction.L.PC;
        }
    }
    // if R[Rs1] < R[Rs2], then ...
    else if (instruction.op == BLT) {

        if (instruction.branched) {
            taken = 1;

            cpu->PC = instruction.L.PC;
        }
    }
    // if R[Rs1] >= R[Rs2], then ...
    else if (instruction.op == BGE) {

        if (instruction.branched) {
            taken = 1;

            cpu->PC = instruction.L.PC;
        }
    }

    return taken;
}
