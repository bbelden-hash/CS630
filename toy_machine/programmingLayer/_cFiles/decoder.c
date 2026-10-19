
#include "../decoder.h"

// takes the CPU struct from the fetcher and determines: if the IR instruction type is of type branch -> if the conditions are met, assign branched to true
Instrn decode(CPU *cpu) {

    cpu->IR.branched = false;

    switch(cpu->IR.op) {

        case BEQ:
            if (cpu->R[cpu->IR.Rs1] == cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;
        
        case BNE:
            if (cpu->R[cpu->IR.Rs1] != cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;

        case BLT:
            if (cpu->R[cpu->IR.Rs1] < cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;

        case BGE:
            if (cpu->R[cpu->IR.Rs1] >= cpu->R[cpu->IR.Rs2]) {
                cpu->IR.branched = true;
            }
            break;

        case JUMP:
            cpu->IR.branched = true;
            break;

        default:
            break;
            
    }

    return cpu->IR;

}
// returns the instruction with its operations and operands from the CPU to be used in the execute ...