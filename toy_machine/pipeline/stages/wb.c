
#include "../pipeline.h"

int writeback(MEM_WB *mem_wb, CPU *cpu, int *instrs) {

    if (mem_wb->valid == 1) {
        (*instrs)++;
    }

    /* debug
    printf("WB #%d: op=%d\n", *instrs, mem_wb->op);
    */

    if (mem_wb->valid == 0) {
        // fprintf(stdout, "bubble: +1 older istruction in EX/MEM empty\n");
        return 0;
    }

    if (mem_wb->op == HALT) {
        return -1;
    }

    if (mem_wb->dest >= 0 && mem_wb->dest < NUM_REGISTERS) {
        cpu->R[mem_wb->dest] = mem_wb->reg_write;
    }

    return 1;
}