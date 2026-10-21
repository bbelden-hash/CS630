
#include "../include/executioner.h"

void executeBRANCH(CPU *cpu, Instrn instruction) {

    // opcode == 'BEQ'; if R[Rs1] == R[Rs2], move to an instruction specified by 'L' (R[L]) by changing the PC to L
    if (instruction.op == BEQ) {

        if (instruction.branched) {
            cpu->PC = instruction.L.PC;
        }
    }
    // opcode == 'BNE'; if R[Rs1] != R[Rs2], move to the instruction pointed to by intsurction.L.PC ...
    else if (instruction.op == BNE) {

        if (instruction.branched) {
            cpu->PC = instruction.L.PC;
        }
    }
    // if R[Rs1] < R[Rs2], then ...
    else if (instruction.op == BLT) {

        if (instruction.branched) {
            cpu->PC = instruction.L.PC;
        }
    }
    // if R[Rs1] >= R[Rs2], then ...
    else if (instruction.op == BGE) {

        if (instruction.branched) {
            cpu->PC = instruction.L.PC;
        }
    }
    // PC == L, ...
    else if (instruction.op == JUMP) {

        if (instruction.branched) {
            cpu->PC = instruction.L.PC;
        }
    }

}
