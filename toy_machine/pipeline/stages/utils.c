
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

static int writes_register(Opcode op) {
    switch(op) {
        case LOAD_I:
        case ADD:
        case SUB:
        case MOVE:
        case LOAD:
        case LOAD_B:
            return 1;
        
        default:
            return 0;
    }
}

static int uses_register(Instrn *instr, int reg) {
    switch (instr->op) {
        case ADD:
        case SUB:
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            return instr->Rs1 == reg || instr->Rs2 == reg;
        
        case MOVE:
            return instr->Rs == reg;
        
        case LOAD:
        case LOAD_B:
            return instr->Rb == reg;

        case STORE:
        case STORE_B:
            return instr->Rb == reg || instr->Rs == reg;
        
        default:
            return 0;
    }
}

int data_hazard(IF_ID *if_id, ID_EX *id_ex, EX_MEM *ex_mem) {

    if (!if_id->valid) {
        return 0;
    }

    Instrn *consumer = &if_id->instr;

    int is_branch = (consumer->op == BEQ || consumer->op == BNE || consumer->op == BLT || consumer->op == BGE);

    // producer in EX
    if (id_ex->valid && writes_register(id_ex->instr.op)) {
        int dest = id_ex->instr.Rd;

        if (uses_register(consumer, dest)) {
            if (is_branch) {
                return 1;
            }

            if (id_ex->instr.op == LOAD || id_ex->instr.op == LOAD_B) {
                return 1;
            }
        }
    }

    // a branch must also wait while a load is in MEM.
    if (is_branch && ex_mem->valid && (ex_mem->op == LOAD || ex_mem->op == LOAD_B)) {
        if (uses_register(consumer, ex_mem->dest)) {
            return 1;
        }
    }

    return 0;
}

void read_operands(ID_EX *id_ex, CPU *cpu) {

    Instrn *instr = &id_ex->instr;

    switch (instr->op) {

        case ADD:
        case SUB:
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            id_ex->Rs1_val = cpu->R[instr->Rs1];
            id_ex->Rs2_val = cpu->R[instr->Rs2];
            break;

        case MOVE:
            id_ex->Rs_val = cpu->R[instr->Rs];
            break;

        case LOAD:
        case LOAD_B:
            id_ex->Rs1_val = cpu->R[instr->Rb];
            break;

        case STORE:
        case STORE_B:
            id_ex->Rs1_val = cpu->R[instr->Rb];
            id_ex->Rs_val = cpu->R[instr->Rs];
            break;

        case LOAD_I:
        case JUMP:
        case HALT:
            break;
    }
}

int forward_values(EX_MEM *newer_ex_mem, EX_MEM *older_ex_mem, MEM_WB *mem_wb, int reg, int32_t *value) {

    // newest producer: instruction currently in EX
    if (newer_ex_mem->valid) {
        switch (newer_ex_mem->op) {
            case LOAD_I:
            case ADD:
            case SUB:
            case MOVE:
                if (newer_ex_mem->dest == reg) {
                    *value = newer_ex_mem->ALU_result;
                    return 1;
                }
                break;

            default:
                break;
        }
    }

    // older producer: instruction currently in MEM
    if (older_ex_mem->valid) {
        switch (older_ex_mem->op) {
            case LOAD_I:
            case ADD:
            case SUB:
            case MOVE:
                if (older_ex_mem->dest == reg) {
                    *value = older_ex_mem->ALU_result;
                    return 1;
                }
                break;

            default:
                break;
        }
    }

    // completed memory operation: result is in MEM/WB
    if (mem_wb->valid && mem_wb->dest == reg && reg >= 0 && reg < NUM_REGISTERS) {

        switch (mem_wb->op) {
            case LOAD_I:
            case ADD:
            case SUB:
            case MOVE:
            case LOAD:
            case LOAD_B:
                *value = mem_wb->reg_write;
                return 1;

            default:
                break;
        }
    }

    return 0;
}

void forward_operands(ID_EX *id_ex, IF_ID *if_id, EX_MEM *newer_ex_mem, EX_MEM *older_ex_mem, MEM_WB *mem_wb) {

    Instrn *instr = &if_id->instr;
    int32_t value;

    switch (instr->op) {
        case ADD:
        case SUB:
        case BEQ:
        case BNE:
        case BLT:
        case BGE:
            if (forward_values(newer_ex_mem, older_ex_mem, mem_wb, instr->Rs1, &value)) {
                id_ex->Rs1_val = value;
            }    
            if (forward_values(newer_ex_mem, older_ex_mem, mem_wb, instr->Rs2, &value)) {
                id_ex->Rs2_val = value;
            }
            break;

        case MOVE:
            if (forward_values(newer_ex_mem, older_ex_mem, mem_wb, instr->Rs, &value)) {
                id_ex->Rs_val = value;
            }
            break;

        case LOAD:
        case LOAD_B:
            if (forward_values(newer_ex_mem, older_ex_mem, mem_wb, instr->Rb, &value)) {
                id_ex->Rs1_val = value;
            }
            break;

        case STORE:
        case STORE_B:
            if (forward_values(newer_ex_mem, older_ex_mem, mem_wb, instr->Rs, &value)) {
                id_ex->Rs_val = value;
            }
            if (forward_values(newer_ex_mem, older_ex_mem, mem_wb, instr->Rb, &value)) {
                id_ex->Rs1_val = value;
            }
            break;

        case LOAD_I:
        case JUMP:
        case HALT:
            break;
    }
}