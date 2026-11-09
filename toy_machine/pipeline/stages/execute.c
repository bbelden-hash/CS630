
#include "../pipeline.h"
#include "../../architectureLayer/labels/label.h"

int execute(EX_MEM *ex_mem, ID_EX *id_ex, CPU *cpu, int *taken, MisPrediction *misprediction, OneBit *BHT_1BIT, TwoBit *BHT_2BIT) {

    if (id_ex->valid == 0) {
        // fprintf(stdout, "bubble: +1 older instruction ID/EX is empty\n");
        return 0;
    }

    if (id_ex->instr.op == ADD) {

        ex_mem->op = ADD;
        ex_mem->ALU_result = id_ex->Rs1_val + id_ex->Rs2_val;
        ex_mem->dest = id_ex->instr.Rd;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == SUB) {

        ex_mem->op = SUB;
        ex_mem->ALU_result = id_ex->Rs1_val - id_ex->Rs2_val;
        ex_mem->dest = id_ex->instr.Rd;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == LOAD_I) {

        ex_mem->op = LOAD_I;
        ex_mem->ALU_result = id_ex->instr.I;
        ex_mem->dest = id_ex->instr.Rd;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == MOVE) {

        ex_mem->op = MOVE;
        ex_mem->ALU_result = id_ex->Rs_val;
        ex_mem->dest = id_ex->instr.Rd;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == LOAD) {

        ex_mem->op = LOAD;

        int base = id_ex->Rs1_val;
        int offset = id_ex->instr.Imm;

        ex_mem->ALU_result = base + offset;
        ex_mem->dest = id_ex->instr.Rd;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == LOAD_B) {

        ex_mem->op = LOAD_B;

        int base = id_ex->Rs1_val;
        int offset = id_ex->instr.Imm;

        ex_mem->ALU_result = base + offset;
        ex_mem->dest = id_ex->instr.Rd;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == STORE) {

        ex_mem->op = STORE;

        int base = id_ex->Rs1_val;
        int offset = id_ex->instr.Imm;

        ex_mem->ALU_result = id_ex->Rs_val;
        ex_mem->dest = base + offset;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == STORE_B) {

        ex_mem->op = STORE_B;

        int base = id_ex->Rs1_val;
        int offset = id_ex->instr.Imm;

        ex_mem->ALU_result = id_ex->Rs_val;
        ex_mem->dest = base + offset;
        ex_mem->npc = id_ex->instr.npc;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == BEQ || id_ex->instr.op == BGE || id_ex->instr.op == BLT || id_ex->instr.op == BNE) {

        ex_mem->op = id_ex->instr.op;

        check_prediction(id_ex->cond, &id_ex->bp, misprediction);
        update_1BIT(BHT_1BIT, id_ex->instr.pc, id_ex->cond);
        update_2BIT(BHT_2BIT, id_ex->instr.pc, id_ex->cond);

        if (id_ex->cond == 1) {
            (*taken)++;

            ex_mem->ALU_result = id_ex->instr.npc;
            ex_mem->dest = -1;
            ex_mem->npc = id_ex->instr.npc + 1;
            ex_mem->valid = 1;

            cpu->pc = ex_mem->ALU_result;
            return 1;
        } else {
            ex_mem->ALU_result = -1;
            ex_mem->dest = -1;
            ex_mem->npc = id_ex->instr.npc;
            ex_mem->valid = 1;
        }
    }
    if (id_ex->instr.op == JUMP) {

        ex_mem->op = id_ex->instr.op;
        ex_mem->dest = -1;
        ex_mem->ALU_result = -1;
        ex_mem->npc = id_ex->instr.npc + 1;
        ex_mem->valid = 1;
    }
    if (id_ex->instr.op == HALT) {

        ex_mem->op = HALT;
        ex_mem->valid = 1;
    }

    return 2;
}