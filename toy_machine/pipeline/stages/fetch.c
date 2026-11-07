
#include "../pipeline.h"

void fetch(IF_ID *if_id, Instrn *IR, int *pc) {

    if_id->instr = *IR;
    if_id->valid = 1;
    (*pc)++;
    
}