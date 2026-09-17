#include <stdint.h>

#include "../execute.h"

void execute(CPU *cpu, Instrn instruction) {

    // if instruction = LOAD_I, increment the destination register value by the immediate value
    if (instruction.op == LOAD_I) {

        cpu->R[instruction.Rd] = instruction.imm;
    }
    // if the instruction = ADD, add the value sitting in R[Rs1] and the value sitting in R[Rs2] and increment to the solution in R[Rd]
    else if (instruction.op == ADD) {

        int64_t wrap =
            (int64_t)cpu->R[instruction.Rs1] +
            (int64_t)cpu->R[instruction.Rs2];

        // 32-bit wraparound --> INT32_MAX + 1 = INT32_MIN
        if (wrap > INT32_MAX) {

            wrap -= 4294967296LL;
        }
        else if (wrap < INT32_MIN) {

            wrap += 4294967296LL;
        }

        cpu->R[instruction.Rd] = (int32_t)wrap;
    }
    else if (instruction.op == HALT) {

        cpu->halted = 1;
    }
}