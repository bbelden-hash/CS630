#ifndef PIPELINE_H
#define PIPELINE_H

#include <stdio.h>

#include "../architectureLayer/machine_state.h"

void fetch(IF_ID *if_id, Instrn *IR, int *pc);
int decode(ID_EX *id_ex, IF_ID *if_id, CPU *cpu);
int execute(EX_MEM *ex_mem, ID_EX *id_ex, CPU *cpu);
void mem(MEM_WB *mem_wb, EX_MEM *ex_mem, MEM *ram);
int writeback(MEM_WB *mem_wb, CPU *cpu, int *instrs);

#endif