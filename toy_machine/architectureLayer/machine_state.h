#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <stdint.h>
#include <stdbool.h>

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
    BEQ, // 8
    BNE, // 9
    BLT, // 10
    BGE, // 11
    JUMP, // 12
    HALT // 14
} Opcode;

typedef enum {

    ALU,
    LOAD_STORE,
    BRANCH,
    JUMPING,
    DATA_MOVEMENT,
    HALTED
} InstrnType;

// labels - a line containing only a name followed by a colon, names the first instruction that comes after it in the file
typedef struct {

    char L[20]; // label name
    int PC; // instruction in .asm branch instruction needs to branch or jump to
} LABEL;

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
    LABEL L;
    bool branched; // for decode to determine if a branch is necessary ...
    InstrnType type; // instruction category
} Instrn;

// 8 general-purpose registers, 32-bit signed integers -> -2,147,483,648  to  2,147,483,647
typedef struct {

    int32_t R[NUM_REGISTERS]; // registers

    Instrn IR; // instruction register -> program instruction sent to the decoder
    int halted; // if HALT has not happened --> keep running
    int PC;
} CPU;

// memory: a flat array of 1024 bytes, addressed 0-1023, all bytes start at zero
typedef struct {

    unsigned char M[NUM_MEM_LOCATIONS]; 
} MEM;

#endif