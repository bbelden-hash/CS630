#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <stdint.h>

#define NUM_REGISTERS 8
#define NUM_MEM_LOCATIONS 1024

// enum: allows swapping out of confusing numbers in code for easy-to-read words

// instruction set:
typedef enum {

    LOAD_I, // 0
    ADD, // 1
    SUB, // 2
    MOVE, // 3
    LOAD, // 4
    STORE, // 5
    LOAD_B, // 6
    STORE_B, // 7
    HALT // 8
} Opcode;

typedef enum {

    ALU,
    LOAD_STORE,
    BRANCH,
    DATA_MOVEMENT,
    HALTED
} InstrnType;

// operands - instruction relationship:
typedef struct {

    Opcode op;
    int Rb; // base register holding a memory address
    int Rd;
    int Rs; // for MOVE and STORE_B
    int Rs1;
    int Rs2;
    int i; // immediate value
    int imm; // offset
    InstrnType type; // instruction category
} Instrn;

// 8 general-purpose registers, 32-bit signed integers -> -2,147,483,648  to  2,147,483,647
typedef struct {

    int32_t R[NUM_REGISTERS]; // registers
    uint32_t PC; // program counter -> index of the next instruction

    Instrn IR; // instruction register -> program instruction sent to the decoder

    int halted; // if HALT has not happened --> keep running
} CPU;

// memory: a flat array of 1024 bytes, addressed 0-1023, all bytes start at zero
typedef struct {

    unsigned char M[NUM_MEM_LOCATIONS]; 
} MEM;

#endif


