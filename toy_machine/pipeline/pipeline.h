#ifndef PIPELINE_H
#define PIPELINE_H

#include "../architectureLayer/machine_state.h"
#include "../architectureLayer/branch_predictor/branch_predictors.h"

int fetch(IF_ID *if_id, Instrn *IR, int *pc, OneBit *BHT_1BIT, TwoBit *BHT_2BIT);
int decode(ID_EX *id_ex, IF_ID *if_id, EX_MEM *newer_ex_mem, EX_MEM *older_ex_mem,MEM_WB *mem_wb,CPU *cpu, OneBit *BHT_1BIT, TwoBit *BHT_2BIT, int *squash, int *taken);
void execute(EX_MEM *ex_mem, ID_EX *id_ex, MisPrediction *misprediction);
void mem(MEM_WB *mem_wb, EX_MEM *ex_mem, MEM *ram);
int writeback(MEM_WB *mem_wb, CPU *cpu, int *instrs);

#endif