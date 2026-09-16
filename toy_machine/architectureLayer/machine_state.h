#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <stdint.h>


#define NUM_REGISTERS 8

// 8 general-purpose registers, 32-bit signed integers -> -2,147,483,648  to  2,147,483,647 (no overflow)
typedef struct {

    int32_t R[NUM_REGISTERS];
    uint32_t PC; // program counter -> where next instruction is located in memory
    uint32_t IR; // instruction register -> instruction held for the decoder
} CPU;

// enum: allows swapping out of confusing numbers in code for easy-to-read words

// instruction set:
typedef enum {

    LOAD_I, // 0
    ADD, // 1
    HALT // 2
} Opcode;

// operands - instruction relationship:
typedef struct {

    Opcode op;
    int Rd;
    int Rs1;
    int Rs2;
    int imm;
} Instrn;

#endif


