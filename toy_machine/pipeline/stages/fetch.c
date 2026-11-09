
#include "../pipeline.h"

int fetch(IF_ID *if_id, Instrn *IR, int *pc, OneBit *BHT_1BIT, TwoBit *BHT_2BIT) {

    if_id->instr = *IR;
    if_id->instr.pc = *pc;
    if_id->valid = 1;

    BranchPredictors branch_predictors = {0};
    if_id->bp = branch_predictors;

    if (IR->op == BEQ || IR->op == BLT || IR->op == BGE || IR->op == BNE) {
        if_id->bp.NT = NT();
        if_id->bp.BTFN = BTFN(IR->npc, *pc);
        if_id->bp.BHT_1BIT = value_1BIT(BHT_1BIT, *pc);
        if_id->bp.BHT_2BIT = value_2BIT(BHT_2BIT, *pc);

        (*pc)++;
        return 1;
    }

    (*pc)++;
    return 0;
}