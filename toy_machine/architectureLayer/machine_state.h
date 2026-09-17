#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#include <stdint.h>

#define NUM_REGISTERS 8

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

// 8 general-purpose registers, 32-bit signed integers -> -2,147,483,648  to  2,147,483,647
typedef struct {

    int32_t R[NUM_REGISTERS]; // registers
    uint32_t PC; // program counter -> index of the next instruction

    Instrn IR; // instruction register -> program instruction sent to the decoder

    int halted; // if HALT has not happened --> keep running
} CPU;

#endif


