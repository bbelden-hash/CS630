#include <stdio.h>
#include <stdlib.h>

#include "../../execute.h"

void executeLOADSTORE(CPU *cpu, MEM *ram, Instrn instruction) {

     // if the instruction = LOAD, take the memory address stored in R[Rb], add offset (imm) to the address in R[Rb], load the value stored in the 4 consecutive bytes in memory starting from the address R[Rb] + offset into R[Rd]
    if (instruction.op == LOAD) {

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
    else if (instruction.op == STORE) {

        


    }

}