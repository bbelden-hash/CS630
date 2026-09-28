#include <stdio.h>
#include <stdlib.h>

#include "../../execute.h"

// little-endian style -> lowest memory address holds the least-significant byte (rightmost 8 bits)

void executeLOADSTORE(CPU *cpu, MEM *ram, Instrn instruction) {

     // if the instruction = LOAD, take the memory address stored in R[Rb], add offset (imm) to the address in R[Rb], load the value stored in the 4 consecutive bytes in memory starting from the address R[Rb] + offset into R[Rd]
    if (instruction.op == LOAD) {

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        if (address < 0 || address > (NUM_MEM_LOCATIONS - 4)) {
            fprintf(stderr, "error: invalid calculated starting address in 'LOAD', must be in range [0, 1023]\n");
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
    // if the instruction = STORE, take the value currently in R[Rs] and store the value within 4 consecutive memory bins starting at the address stored in R[Rb] + the offset (imm)
    else if (instruction.op == STORE) {

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        int storage = cpu->R[instruction.Rs];

        if (address < 0 || address > (NUM_MEM_LOCATIONS - 3)) {
            fprintf(stderr, "error: invalid calculated starting address in 'STORE', must be in range [0, 1023]\n");
            exit(-1);
        }

        /*
            move the bits in 'storage' to the right by n positions (storage >> n)
            hexadecimal, each hexadecimal digit represents 4 bits -> F = 1111, FF = 1111 1111, ...
            0xFF = 1111 1111 -> 11111111 (8 bits, 1 byte)
            & = bitwise AND, compares the bits one at a time -> 1 & 1 = 1, 1 & 0 = 0, 0 & 1 = 0, 0 & 0 = 0
                
                storage = 0x12345678
                0xFF    = 0x000000FF

                12 34 56 78
            &   00 00 00 FF
            ---------------
            
            bytes w/ 00 under them -> completely wiped out
            byte w/ FF under it -> output, "keep all 8 bits of this byte segment"
        */

        uint8_t byte0 = (storage >> 0) & 0xFF;
        uint8_t byte1 = (storage >> 8) & 0xFF;
        uint8_t byte2 = (storage >> 16) & 0xFF;
        uint8_t byte3 = (storage >> 24) & 0xFF;

        if (address % 4 == 0) { 

            











        


    }

}