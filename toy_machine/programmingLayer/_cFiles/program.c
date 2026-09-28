#include <stdio.h>
#include <string.h>

/*
    how do we read the .asm input file?
    without the program loader/parser --> there is nothing for fetch() to fetch
*/

#include "../program.h"

// opens filename, reads each line in filename, parses each instruction, puts instructions into 'program[]'
// returns how many instructions were loaded
int load_program(const char *filename, Instrn *program) {

    // pointer to .asm file, line in .asm file, and how many lines in the .asm file
    FILE *file;
    char line[256];
    int program_size = 0;

    file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "error: no file to open and read in load_program function\n");
        return -1;
    }

    // blank lines -> ignore, lines beginning with # -> ignore, everything else -> fair game
    while (fgets(line, sizeof(line), file) != NULL) {

        line[strcspn(line, "\r\n")] = '\0';
        
        if (line[0] == '#') {
            continue;
        }
        if (line[0] == '\0') {
            continue;
        }

        int rb;
        int rd;
        int rs;
        int rs1;
        int rs2;
        int i;
        int imm;
        // & gives sscanf() the address where it should store the operands
        // did sscanf() pull 2 values, 3 values, or no values?
        if (sscanf(line, "LOAD_I R%d, %d", &rd, &i) == 2) {

            Instrn instruction = {0};

            instruction.op = LOAD_I;
            instruction.Rd = rd;
            instruction.i = i;
            instruction.type = DATA_MOVEMENT;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "ADD R%d, R%d, R%d", &rd, &rs1, &rs2) == 3) {

            Instrn instruction = {0};

            instruction.op = ADD;
            instruction.Rd = rd;
            instruction.Rs1 = rs1;
            instruction.Rs2 = rs2;
            instruction.type = ALU;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "SUB R%d, R%d, R%d", &rd, &rs1, &rs2) == 3) {

            Instrn instruction = {0};

            instruction.op = SUB;
            instruction.Rd = rd;
            instruction.Rs1 = rs1;
            instruction.Rs2 = rs2;
            instruction.type = ALU;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "MOVE R%d, R%d", &rd, &rs) == 2) {

            Instrn instruction = {0};

            instruction.op = MOVE;
            instruction.Rd = rd;
            instruction.Rs = rs;
            instruction.type = DATA_MOVEMENT;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "LOAD R%d, %d(R%d)", &rd, &imm, &rb) == 3) {

            Instrn instruction = {0};

            instruction.op = LOAD;
            instruction.Rb = rb;
            instruction.Rd = rd;
            instruction.imm = imm;
            instruction.type = LOAD_STORE;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "STORE R%d, %d(R%d)", &rs, &imm, &rb) == 3) {

            Instrn instruction = {0};

            instruction.op = STORE;
            instruction.Rb = rb;
            instruction.Rs = rs;
            instruction.imm = imm;
            instruction.type = LOAD_STORE;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "LOAD_B R%d, %d(R%d)", &rd, &imm, &rb) == 3) {

            Instrn instruction = {0};

            instruction.op = LOAD_B;
            instruction.Rb = rb;
            instruction.Rd = rd;
            instruction.imm = imm;
            instruction.type = LOAD_STORE;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "STORE_B R%d, %d(R%d)", &rs, &imm, &rb) == 3) {

            Instrn instruction = {0};

            instruction.op = STORE_B;
            instruction.Rb = rb;
            instruction.Rs = rs;
            instruction.imm = imm;
            instruction.type = LOAD_STORE;

            program[program_size] = instruction;
            program_size++;
        }
        else if (strcmp(line, "HALT") == 0) {

            Instrn instruction = {0};

            instruction.op = HALT;
            instruction.type = HALTED;

            program[program_size] = instruction;
            program_size++;

            break;
        }
        else {
            
            fprintf(stderr, "error: invalid instruction: %s\n", line);
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return program_size;
}