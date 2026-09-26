#include <stdint.h>
#include <stdio.h>

#include "../execute.h"

void execute(CPU *cpu, MEM *ram, Instrn instruction) {

    // if instruction = LOAD_I, increment the destination register value by the immediate value
    if (instruction.op == LOAD_I) {

        cpu->R[instruction.Rd] = instruction.i;
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
    // if the instruction = SUB, subtract the value sitting in R[Rs2] from the value sitting in R[Rs1] and place the solution in R[Rd]
    else if (instruction.op == SUB) {

        int64_t wrap =
            (int64_t)cpu->R[instruction.Rs1] -
            (int64_t)cpu->R[instruction.Rs2];
        
        // 32-bit wraparound --> INT32_MIN - 1 = INT32_MAX
        if (wrap < INT32_MIN) {

             wrap += 4294967296LL;
        }
        else if (wrap > INT32_MAX) {

            wrap -= 4294967296LL;
        }

        cpu->R[instruction.Rd] = (int32_t)wrap;
    }
    // if the instruction = MOVE, a plain register-to-register copy; place the value sitting in R[Rs] into register R[Rd], R[Rs] is left unchanged
    else if (instruction.op == MOVE) {

        cpu->R[instruction.Rd] = cpu->R[instruction.Rs];
    }
    // if the instruction = LOAD, take the memory address stored in R[Rb], add offset (imm) to the address in R[Rb], load the value stored in the 4 consecutive bytes in memory starting from the address R[Rb] + offset into R[Rd]
    else if (instruction.op == LOAD) {

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        if (address < 0 || address > (1024 - 3)) {
            fprintf(stderr, "error: invalid calculated starting address in 'LOAD', must be in range [0, 1024]\n");
            exit(-1);
        }

        if (address % 4 == 0) { 

            int32_t load =
                ram->M[address] // we want this byte to stay in the rightmost 8 bits of 'load'
                | (ram->M[address + 1] << 8) // want this byte to move 8 bits left
                | (ram->M[address + 2] << 16) // want this byte to move 16 bits left
                | (ram->M[address + 3] << 24); // want this byte to move 24 bits left

            cpu->R[instruction.Rd] = load;
        } 
        else {
            fprintf(stderr, "error: 'LOAD' requires a starting address in 'MEM' that is a multiple of 4\n");
            exit(-1);
        }
    }
    else if (instruction.op == HALT) {

        cpu->halted = 1;
    }
}