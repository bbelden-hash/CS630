
#include "../pipeline.h"

int decode(ID_EX *id_ex, IF_ID *if_id, CPU *cpu) {

    if (if_id->valid == 0) {
        // fprintf(stdout, "bubble: +1 older instruction IF/ID is empty\n");
        return 0;
    }

    id_ex->instr = if_id->instr;
    id_ex->bp = if_id->bp;

    // initialize unused operands
    id_ex->Rs1_val = 0;
    id_ex->Rs2_val = 0;
    id_ex->Rs_val = 0;
    id_ex->cond = 0;

    switch (if_id->instr.op) {

        case ADD:
        case SUB:
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            id_ex->Rs1_val = cpu->R[if_id->instr.Rs1];
            id_ex->Rs2_val = cpu->R[if_id->instr.Rs2];
            break;

        case MOVE:
            id_ex->Rs_val = cpu->R[if_id->instr.Rs];
            break;

        case LOAD:
        case LOAD_B:
            id_ex->Rs1_val = cpu->R[if_id->instr.Rb];
            break;

        case STORE:
        case STORE_B:
            id_ex->Rs1_val = cpu->R[if_id->instr.Rb];
            id_ex->Rs_val = cpu->R[if_id->instr.Rs];
            break;

        default:
            break;
    }

    if (id_ex->instr.op == BEQ) {
        id_ex->cond = (id_ex->Rs1_val == id_ex->Rs2_val);
    }
    else if (id_ex->instr.op == BNE) {
        id_ex->cond = (id_ex->Rs1_val != id_ex->Rs2_val);
    }
    else if (id_ex->instr.op == BLT) {
        id_ex->cond = (id_ex->Rs1_val < id_ex->Rs2_val);
    }
    else if (id_ex->instr.op == BGE) {
        id_ex->cond = (id_ex->Rs1_val >= id_ex->Rs2_val);
    }

    if (id_ex->instr.op == JUMP) {
        cpu->pc = id_ex->instr.npc;
        id_ex->valid = 1;
        return -1;
    }

    id_ex->valid = 1;
    return 1;
}
