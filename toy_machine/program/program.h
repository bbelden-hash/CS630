#ifndef PROGRAM_H
#define PROGRAM_H

#include "../architectureLayer/machine_state.h"
#include "../architectureLayer/labels/label.h"

int load_program(const char *filename, Instrn *program, LocalLabelTable *labels, int *program_size, int *label_size);

// tools to correctly parse instructions from the inputted .asm file
typedef enum {
    PARSE_ERROR = -1, // line = invalid syntax
    PARSE_IGNORED, // line = blank or comment
    PARSE_INSTRUCTION, // line = instruction
    PARSE_LABEL // line = label
} ParseResult;

ParseResult parse_line(const char *line, Instrn *instruction, Label *label, LocalLabelTable *table);
//

#endif


