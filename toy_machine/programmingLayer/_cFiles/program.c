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
        fprintf(stderr, "error: no file to open and read in load_program function");
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

        int rd;
        int rs1;
        int rs2;
        int imm;
        // & gives sscanf() the address where it should store the operands
        // did sscanf() pull 2 values, 3 values, or no values?
        if (sscanf(line, "LOAD_I R%d, %d", &rd, &imm) == 2) {

            Instrn instruction;

            instruction.op = LOAD_I;
            instruction.Rd = rd;
            instruction.Rs1 = 0;
            instruction.Rs2 = 0;
            instruction.imm = imm;

            program[program_size] = instruction;
            program_size++;
        }
        else if (sscanf(line, "ADD R%d, R%d, R%d", &rd, &rs1, &rs2) == 3) {

            Instrn instruction;

            instruction.op = ADD;
            instruction.Rd = rd;
            instruction.Rs1 = rs1;
            instruction.Rs2 = rs2;
            instruction.imm = 0;

            program[program_size] = instruction;
            program_size++;
        }
        else if (strcmp(line, "HALT") == 0) {

            Instrn instruction;

            instruction.op = HALT;
            instruction.Rd = 0;
            instruction.Rs1 = 0;
            instruction.Rs2 = 0;
            instruction.imm = 0;

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