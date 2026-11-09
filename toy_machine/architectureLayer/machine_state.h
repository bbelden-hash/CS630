#ifndef MACHINE_STATE_H
#define MACHINE_STATE_H

#define NUM_REGISTERS 8
#define NUM_MEM_LOCATIONS 1024

#include "branch_predictor/branch_predictors.h"

#include <stdint.h>

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
    HALT // 13
} Opcode;

typedef struct {
    Opcode op;

    int pc;
    int npc;
    int Rs1;
    int Rs2;
    int Rs; // for MOVE, STORE_B, STORE
    int Rb; // base register holding a memory address
    int Rd;
    int I; // immediate value
    int Imm; // offset
    int label;
} Instrn;

typedef struct {
    Instrn instr;
    BranchPredictors bp;

    int valid;
} IF_ID;

typedef struct {
    Instrn instr;
    BranchPredictors bp;
    int32_t Rs1_val; // value read from register Rs1
    int32_t Rs2_val; // value read from register Rs2
    int32_t Rs_val; // value read from register Rs (MOVE/STORE)

    int cond;
    int valid;
} ID_EX;

typedef struct {
    Opcode op;
    int npc;

    int32_t ALU_result;
    int32_t Rs_val;

    int dest;
    int valid;
} EX_MEM;

typedef struct {
    Opcode op;

    int32_t reg_write;
    int32_t mem_data;

    int dest;
    int valid;
} MEM_WB;

typedef struct {
    IF_ID if_id;
    ID_EX id_ex;
    EX_MEM ex_mem;
    MEM_WB mem_wb;
} PipelineRegisters;

// 8 general-purpose registers, 32-bit signed integers -> -2,147,483,648  to  2,147,483,647
typedef struct {
    int32_t R[NUM_REGISTERS];
    int pc;
    Instrn IR; 
} CPU;

// memory: a flat array of 1024 bytes, addressed 0-1023, all bytes start at zero
typedef struct {
    unsigned char M[NUM_MEM_LOCATIONS]; 
} MEM;

#endif

