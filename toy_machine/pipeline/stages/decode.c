
#include "../pipeline.h"
#include "../utils.h"

int decode(ID_EX *id_ex, 
           IF_ID *if_id, 
           EX_MEM *newer_ex_mem, 
           EX_MEM *older_ex_mem,
           MEM_WB *mem_wb,
           CPU *cpu, 
           OneBit *BHT_1BIT, 
           TwoBit *BHT_2BIT, 
           int *squash,
           int *taken) {

    if (if_id->valid == 0) {
        // fprintf(stdout, "bubble: +1 older instruction IF/ID is empty\n");
        return 0;
    }

    id_ex->instr = if_id->instr;
    id_ex->bp = if_id->bp;

    read_operands(id_ex, cpu);
    forward_operands(id_ex, if_id, newer_ex_mem, older_ex_mem, mem_wb);

    if (id_ex->instr.op == BEQ) {
        id_ex->cond = (id_ex->Rs1_val == id_ex->Rs2_val);

        if ((id_ex->cond == 0 && (id_ex->bp.BHT_2BIT == 0 || id_ex->bp.BHT_2BIT == 1)) ||
            (id_ex->cond == 1 && (id_ex->bp.BHT_2BIT == 2 || id_ex->bp.BHT_2BIT == 3))) {
                (*squash) = 0;    
        } else {
            (*squash) = 1;
        }
    }
    else if (id_ex->instr.op == BNE) {
        id_ex->cond = (id_ex->Rs1_val != id_ex->Rs2_val);

        if ((id_ex->cond == 0 && (id_ex->bp.BHT_2BIT == 0 || id_ex->bp.BHT_2BIT == 1)) ||
            (id_ex->cond == 1 && (id_ex->bp.BHT_2BIT == 2 || id_ex->bp.BHT_2BIT == 3))) {
                (*squash) = 0;    
        } else {
            (*squash) = 1;
        }
    }
    else if (id_ex->instr.op == BLT) {
        id_ex->cond = (id_ex->Rs1_val < id_ex->Rs2_val);

        if ((id_ex->cond == 0 && (id_ex->bp.BHT_2BIT == 0 || id_ex->bp.BHT_2BIT == 1)) ||
            (id_ex->cond == 1 && (id_ex->bp.BHT_2BIT == 2 || id_ex->bp.BHT_2BIT == 3))) {
                (*squash) = 0;    
        } else {
            (*squash) = 1;
        }
    }
    else if (id_ex->instr.op == BGE) {
        id_ex->cond = (id_ex->Rs1_val >= id_ex->Rs2_val);

        if ((id_ex->cond == 0 && (id_ex->bp.BHT_2BIT == 0 || id_ex->bp.BHT_2BIT == 1)) ||
            (id_ex->cond == 1 && (id_ex->bp.BHT_2BIT == 2 || id_ex->bp.BHT_2BIT == 3))) {
                (*squash) = 0;    
        } else {
            (*squash) = 1;
        }
    }
    if (id_ex->instr.op == JUMP) {
        cpu->pc = id_ex->instr.npc;
        id_ex->valid = 1;
        return -1;
    }

    if (id_ex->cond == 1) {
        (*taken)++;
    }

    update_1BIT(BHT_1BIT, id_ex->instr.pc, id_ex->cond);
    update_2BIT(BHT_2BIT, id_ex->instr.pc, id_ex->cond);

    id_ex->valid = 1;
    return 1;
}


