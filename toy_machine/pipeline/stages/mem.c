
#include "../pipeline.h"
#include "../utils.h"

void mem(MEM_WB *mem_wb, EX_MEM *ex_mem, MEM *ram) {

    if (ex_mem->valid == 0) {
        // fprintf(stdout, "bubble: +1 older instruction in EX/MEM empty\n");
        return;
    }

    mem_wb->op = ex_mem->op;

    if (ex_mem->op == LOAD) {
        int address = ex_mem->ALU_result;

        int address0 = wrapAddress(address);
        int address1 = wrapAddress(address + 1);
        int address2 = wrapAddress(address + 2);
        int address3 = wrapAddress(address + 3);

        int32_t load =
            ((uint32_t)ram->M[address0]) // we want this byte to stay in the rightmost 8 bits of 'load'
            | ((uint32_t)ram->M[address1] << 8) // want this byte to move 8 bits left
            | ((uint32_t)ram->M[address2] << 16) // want this byte to move 16 bits left
            | ((uint32_t)ram->M[address3] << 24); // want this byte to move 24 bits left

        mem_wb->reg_write = load;
        mem_wb->dest = ex_mem->dest;
        mem_wb->valid = 1;
    }
    else if (ex_mem->op == LOAD_B) {
        int address = ex_mem->ALU_result;

        address = wrapAddress(address);

        uint8_t value = ram->M[address];
        mem_wb->reg_write = value;
        mem_wb->dest = ex_mem->dest;
        mem_wb->valid = 1;
    }
    else if (ex_mem->op == STORE) {
        int address = ex_mem->dest;

        int address0 = wrapAddress(address);
        int address1 = wrapAddress(address + 1);
        int address2 = wrapAddress(address + 2);
        int address3 = wrapAddress(address + 3);

        uint32_t storage = ex_mem->ALU_result;

        /*
            move the bits in 'storage' to the right by n positions (storage >> n)
            hexadecimal, each hexadecimal digit represents 4 bits -> F = 1111, FF = 1111 1111, ...
            0xFF = 1111 1111 -> 11111111 (8 bits, 1 byte)
            & = bitwise AND, compares the bits one at a time -> 1 & 1 = 1, 1 & 0 = 0, 0 & 1 = 0, 0 & 0 = 0
                
                storage = 0x12345678
                0xFF    = 0x000000FF

                12 34 56 78
            &   00 00 00 FF
            ---------------
            
            bytes w/ 00 under them -> completely wiped out
            byte w/ FF under it -> output, "keep all 8 bits of this byte segment"
        */
        uint8_t byte0 = (storage >> 0) & 0xFF;
        uint8_t byte1 = (storage >> 8) & 0xFF;
        uint8_t byte2 = (storage >> 16) & 0xFF;
        uint8_t byte3 = (storage >> 24) & 0xFF;

        ram->M[address0] = byte0;
        ram->M[address1] = byte1;
        ram->M[address2] = byte2;
        ram->M[address3] = byte3;
        
        mem_wb->valid = 1;
        mem_wb->mem_data = storage;
        mem_wb->dest = -1;
    }
    else if (ex_mem->op == STORE_B) {
        uint32_t storage = ex_mem->ALU_result;
        uint8_t value = (storage >> 0) & 0xFF;

        int address = ex_mem->dest;

        address = wrapAddress(address);

        ram->M[address] = value;

        mem_wb->valid = 1;
        mem_wb->mem_data = storage;
        mem_wb->dest = -1;
    }
    else {
        mem_wb->reg_write = ex_mem->ALU_result;
        mem_wb->dest = ex_mem->dest;
        mem_wb->valid = 1;
    }

}