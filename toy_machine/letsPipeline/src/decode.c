
#include "../include/decoder.h"

// takes the CPU struct from the fetcher and determines: if the IR instruction type is of type branch -> if the conditions are met, assign branched to true
// if the instruction register operation is of type BRANCH, assign the value of targetPC to the instruction registers label jump PC
Instrn decode(CPU *cpu, PC *predict) {

    cpu->IR.branched = false;

    switch(cpu->IR.op) {

        case BEQ:
            predict->target = cpu->IR.L.PC;

            if (cpu->R[cpu->IR.Rs1] == cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;
        
        case BNE:
            predict->target = cpu->IR.L.PC;

            if (cpu->R[cpu->IR.Rs1] != cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;

        case BLT:
            predict->target = cpu->IR.L.PC;

            if (cpu->R[cpu->IR.Rs1] < cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;

        case BGE:
            predict->target = cpu->IR.L.PC;

            if (cpu->R[cpu->IR.Rs1] >= cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;

        case JUMP:
            predict->target = cpu->IR.L.PC;

            cpu->IR.branched = true;
            break;

        default:
            break;
            
    }

    return cpu->IR;
}
// returns the instruction with its operations and operands from the CPU to be used in the execute ...
