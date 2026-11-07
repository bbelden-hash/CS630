#include <stdio.h>
#include <stdlib.h>

#define MAX_PROGRAM_SIZE 100000

// relative paths to header files for architecture
#include "architectureLayer/machine_state.h"
#include "architectureLayer/initial_state.h"
#include "architectureLayer/labels/label.h"

// relative path to header for program
#include "program/program.h"

// relative path to header for pipeline
#include "pipeline/pipeline.h"
#include "pipeline/utils.h"

int main(int argc, char *argv[]) {

    if (argc != 2) {
        fprintf(stderr, "error: ./<my_submission> <program.asm>\n");
        return EXIT_FAILURE;
    }

    CPU cpu;
    MEM ram;

    initialize_machine(&cpu);
    initialize_MEM(&ram);
    
    Instrn program[MAX_PROGRAM_SIZE];
    int program_size;

    LocalLabelTable label_table = {0};
    int label_size;

    if (load_program(argv[1], program, &label_table, &program_size, &label_size) != 0) {
        fprintf(stderr, "error: failed to load a valid program\n");
        return EXIT_FAILURE;
    }

    for (int i = 0; i < program_size; i++) {
        
        if (program[i].op == BEQ || program[i].op == BGE || program[i].op == BLT || program[i].op == BNE || program[i].op == JUMP) {
            resolve_branch(&label_table, &program[i], program[i].label);
        }
    }

    PipelineRegisters curr = {0};
    PipelineRegisters next = {0};

    int clock = 0;
    int data_stalls = 0;
    int control_stalls = 0;
    int instrs = 0;
    
    while(cpu.pc < program_size || curr.if_id.valid || curr.id_ex.valid || curr.ex_mem.valid || curr.mem_wb.valid) {
        next = (PipelineRegisters){0};

        if (writeback(&curr.mem_wb, &cpu, &instrs) == -1) {
            clock++;
            break;
        }

        int execute_status = execute(&next.ex_mem, &curr.id_ex, &cpu);
        
        if (execute_status == 1) {
            next.id_ex = (ID_EX){0};
            next.if_id = (IF_ID){0};

            control_stalls += 2;
        }
        else if (data_hazard(&curr.if_id, &curr.id_ex, &curr.ex_mem)) {
            // stall IF and ID
            next.if_id = curr.if_id;
            data_stalls++;
        }
        else {
            
            if (decode(&next.id_ex, &curr.if_id, &cpu) == -1) {
                next.if_id = (IF_ID){0};
                control_stalls++;
            }
            else if (cpu.pc >= 0 && cpu.pc < program_size) {
                Instrn IR = program[cpu.pc];
                fetch(&next.if_id, &IR, &cpu.pc);
            }
        }

        mem(&next.mem_wb, &curr.ex_mem, &cpu, &ram);
        
        curr = next;
        clock++;
    }

    for (int i = 0; i < NUM_REGISTERS; i++) {
        fprintf(stdout, "R%d=%d\n", i, cpu.R[i]);
    }

    if (clock != (instrs + 4 + data_stalls + control_stalls)) {
        fprintf(stderr, "error: upon completion of pipeline required math does not hold -> improper stall, cycle, or instr count\n");
        printf("instrs + 4 + data_stalls + ctrl_stalls = %d\n", instrs + 4 + data_stalls + control_stalls);
        printf("cycles = %u\n", clock);
        return -1;
    }

    printf("CYCLES=%u\n", clock);
    printf("INSTRS=%u\n", instrs);
    printf("DATA_STALLS=%u\n", data_stalls);
    printf("CTRL_STALLS=%u\n", control_stalls);

    return 0;
}