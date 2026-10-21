#ifndef INSTRUCTION_PARSER_H
#define INSTRUCTION_PARSER_H

#include "../../architectureLayer/machine_state.h"

typedef enum {

    PARSE_ERROR = -1, // line = invalid syntax
    PARSE_IGNORED, // line = blank or comment
    PARSE_INSTRUCTION, // line = instruction
    PARSE_LABEL // line = label
} ParseResult;

ParseResult parse_line(const char *line, Instrn *instruction, LABEL *label);

#endif