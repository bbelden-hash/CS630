#ifndef UTILS_H
#define UTILS_H

#include "../architectureLayer/machine_state.h"

int wrapAddress(int address);
int data_hazard(IF_ID *if_id, ID_EX *id_ex, EX_MEM *ex_mem);
void read_operands(ID_EX *id_ex, CPU *cpu);
int forward_values(EX_MEM *newer_ex_mem, EX_MEM *older_ex_mem, MEM_WB *mem_wb, int reg, int32_t *value);
void forward_operands(ID_EX *id_ex, IF_ID *if_id, EX_MEM *newer_ex_mem, EX_MEM *older_ex_mem, MEM_WB *mem_wb);

#endif