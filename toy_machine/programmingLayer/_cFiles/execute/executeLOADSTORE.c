#include <stdio.h>
#include <stdlib.h>

#include "../../execute.h"

// little-endian style -> lowest memory address holds the least-significant byte (rightmost 8 bits)
// decided to implement wraparound behavior so -> LOAD R3, -4(R0) will give you the 4 byte int stored in M[1021], M[1022], M[1023], M[0] if R0 is 1, ...

// converts any memory address into a permitted address [0, 1023]
int wrapAddress(int address) {

    /*
        example: NUM_MEM_LOCATIONS = 1024
        address = 1025

        1025 % 1024 = 1
        1 + 1024 = 1025
        1025 % 1024 = 1
        M[1025] = M[1]

        address + 1 = 1026
        1026 % 1024 = 2
        2 + 1024 = 1026
        1026 % 1024 = 2
        M[1026] = M[2]

        ...
    */
    return ((address % NUM_MEM_LOCATIONS)
            + NUM_MEM_LOCATIONS)
            % NUM_MEM_LOCATIONS;
}

void executeLOADSTORE(CPU *cpu, MEM *ram, Instrn instruction) {

     // if the instruction = LOAD, take the memory address stored in R[Rb], add offset (imm) to the address in R[Rb], load the value stored in the 4 consecutive bytes in memory starting from the address R[Rb] + offset into R[Rd]
    if (instruction.op == LOAD) {

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        int address0 = wrapAddress(address);
        int address1 = wrapAddress(address + 1);
        int address2 = wrapAddress(address + 2);
        int address3 = wrapAddress(address + 3);

        int32_t load =
            ((uint32_t)ram->M[address0]) // we want this byte to stay in the rightmost 8 bits of 'load'
            | ((uint32_t)ram->M[address1] << 8) // want this byte to move 8 bits left
            | ((uint32_t)ram->M[address2] << 16) // want this byte to move 16 bits left
            | ((uint32_t)ram->M[address3] << 24); // want this byte to move 24 bits left

        cpu->R[instruction.Rd] = load;
    }
    // if the instruction = STORE, store the value to R[Rs] ... store the value within 4 consecutive memory bins starting at the address stored in R[Rb] + the offset (imm)
    else if (instruction.op == STORE) {

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        int address0 = wrapAddress(address);
        int address1 = wrapAddress(address + 1);
        int address2 = wrapAddress(address + 2);
        int address3 = wrapAddress(address + 3);

        uint32_t storage = cpu->R[instruction.Rs];

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

        ram->M[address0] = byte0;
        ram->M[address1] = byte1;
        ram->M[address2] = byte2;
        ram->M[address3] = byte3;
    }
    // if the instruction = LOAD_B, take the 8-bit value sitting in memory at R[Rb] + offset (imm) and load into the low 8 bits of R[Rd]
    else if (instruction.op == LOAD_B) {

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        address = wrapAddress(address);

        uint8_t value = ram->M[address];
        cpu->R[instruction.Rd] = value;
    }
    // if the instruction = STORE_B, take the low 8 bits of R[Rs] and store that value to ram->M[R[Rb] + imm]
    else if (instruction.op == STORE_B) {

        uint32_t storage = cpu->R[instruction.Rs];
        uint8_t value = (storage >> 0) & 0xFF;

        int base = cpu->R[instruction.Rb];
        int address = base + instruction.imm;

        address = wrapAddress(address);

        ram->M[address] = value;
    }
}