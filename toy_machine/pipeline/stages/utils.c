
#include "../utils.h"

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

int data_hazard(IF_ID *id, ID_EX *ex, EX_MEM *mem) {

    if (!id->valid) {
        return 0;
    }

    int rs1 = -1;
    int rs2 = -1;

    // determine which registers the ID instruction reads
    switch (id->instr.op) {

        case ADD:
        case SUB:
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            rs1 = id->instr.Rs1;
            rs2 = id->instr.Rs2;
            break;

        case MOVE:
            rs1 = id->instr.Rs;
            break;

        case LOAD:
        case LOAD_B:
            rs1 = id->instr.Rb;
            break;

        case STORE:
        case STORE_B:
            rs1 = id->instr.Rb;
            rs2 = id->instr.Rs; 
            break;

        default:
            break;
    }

    // check for dependency with instruction in EX
    if (ex->valid) {

        if (ex->instr.op == LOAD_I ||
            ex->instr.op == ADD ||
            ex->instr.op == SUB ||
            ex->instr.op == MOVE ||
            ex->instr.op == LOAD ||
            ex->instr.op == LOAD_B) {

            if ((rs1 != -1 && ex->instr.Rd == rs1) ||
                (rs2 != -1 && ex->instr.Rd == rs2)) {
                return 1;
            }
        }
    }

    // check for dependency with instruction in MEM
    if (mem->valid &&
        (mem->op == LOAD_I ||
         mem->op == ADD ||
         mem->op == SUB ||
         mem->op == MOVE ||
         mem->op == LOAD ||
         mem->op == LOAD_B)) {

        if ((rs1 != -1 && mem->dest == rs1) ||
            (rs2 != -1 && mem->dest == rs2)) {
            return 1;
        }
    }

    return 0;
}