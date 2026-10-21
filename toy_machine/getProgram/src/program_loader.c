
#include "../include/program_loader.h"
#include "../include/instruction_parser.h"
#include "../include/program_builder.h"

#include <stdio.h>
#include <string.h>

int load_program(const char *filename, Instrn *program, LABEL *labels, int *program_size, int *label_size) {
    
    if (filename == NULL || program == NULL || labels == NULL || program_size == NULL || label_size == NULL) {

        fprintf(stderr, "error: invalid loader arguments\n");
        return -1;
    }

    *program_size = 0;
    *label_size = 0;

    FILE *file = fopen(filename, "r");

    if (file == NULL) {

        perror(filename);
        return -1;
    }

    char line[256];
    int line_number = 0;
    int status = 0;

    while (fgets(line, sizeof(line), file) != NULL) {

        ++line_number;

        if (strchr(line, '\n') == NULL && !feof(file)) {

            fprintf(stderr, "%s:%d: error: line too long\n", filename, line_number);
            status = -1;
            break;
        }

        line[strcspn(line, "\r\n")] = '\0';

        Instrn instruction = {0};
        LABEL label = {0};

        // takes one line of assembly, figures out what it means, and tells loader what it found
        ParseResult result = parse_line(line, &instruction, &label);

        if (result == PARSE_IGNORED) {

            continue;
        }

        if (result == PARSE_ERROR) {

            fprintf(stderr, "%s:%d: error: invalid assembly syntax\n", filename, line_number);
            status = -1;
            break;
        }

        if (result == PARSE_LABEL) {
            label.PC = *program_size;
            if (append_label(labels, *label_size, label) != 0) {

                fprintf(stderr, "%s:%d: error: cannot add label\n", filename, line_number);
                status = -1;
                break;
            }
            (*label_size)++;

        } else if (result == PARSE_INSTRUCTION) {
            if (append_instruction(program, *program_size, instruction) != 0) {

                fprintf(stderr, "%s:%d: error: cannot add instruction\n", filename, line_number);
                status = -1;
                break;
            }
            (*program_size)++;

        }
    }

    if (ferror(file)) {

        perror("error: reading assembly file");
        status = -1;
    }

    if (fclose(file) != 0) {

        perror("error: closing assembly file");
        status = -1;
    }

    if (status != 0) {

        *program_size = 0;
        *label_size = 0;
        return -1;
    }

    return 0;
}