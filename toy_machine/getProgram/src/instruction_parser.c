
#include "../include/instruction_parser.h"

#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 256
#define MAX_TOKENS 5
#define REGISTER_COUNT 8

// static -> functions can only be used in this file!

static char *trim_whitespace(char *text) {
    // removes whitespace from the beginning and end of a string
    // returns a pointer to a character in memory

    char *end; // pointer to find the end of 'useful' text ...

    while (isspace((unsigned char)*text)) { // changes where the text pointer points to -> ensures pointer is pointing to first character in memory from 'text'

        text++;
    }

    if (*text == '\0') { // checks if current text pointer is pointing to a null terminator in memory -> if so return

        return text;
    }

    end = text + strlen(text) - 1; // moves pointer from beginning of 'text' to the null terminator (end), then moves it back one character to the last actual character in 'text'

    while (end > text && isspace((unsigned char)*end)) {
        // end > text: ensures pointer doesn't move too far back past the first character in 'text' 
        // ensures end is pointing to the last character in memory from 'text'

        end--;
    }

    end[1] = '\0'; // put a null terminator one character after the character 'end' points to
    return text;
}

static int valid_label_name(const char *name) {
    // checks whether a label name is valid according to rules ...
    // a valid label is a string with a letter or underscore, followed by letters, digits or underscores, and is case-sensitive

    size_t i;

    if (name == NULL || name[0] == '\0') { // first character a null terminator or nothing ...

        return 0;
    }

    if (!(isalpha((unsigned char)name[0]) || name[0] == '_')) { // first character is not a letter or underscore ...

        return 0;
    }

    for (i = 1; name[i] != '\0'; i++) {
        if (!(isalnum((unsigned char)name[i]) || name[i] == '_')) { // the remaining characters after the first is not a letter, digit, or underscore ...

            return 0;
        }
    }

    return 1;
}

static int parse_register(const char *token, int *reg) {
    // checks whether a string represents a valid register

    char *end;
    long value; // numeric value extracted from the register name 

    if (token == NULL || reg == NULL) {

        return -1;
    }

    if (token[0] != 'R' && token[0] != 'r') { // register starts with R or r

        return -1;
    }

    if (token[1] == '\0') { // register has a number

        return -1;
    }

    errno = 0;
    value = strtol(token + 1, &end, 10); // &end -> address storing a pointer to the first 'invalid' character, 10 -> use decimal

    if (errno != 0 || *end != '\0') { // no error, end is not a null terminator

        return -1;
    }

    if (value < 0 || value >= REGISTER_COUNT) { // is the register valid R[0] - R[7]

        return -1;
    }

    *reg = (int)value;
    return 0;
}

static int parse_integer(const char *token, int *value) {

    char *end;
    long parsed;

    if (token == NULL || value == NULL || token[0] == '\0') {

        return -1;
    }

    errno = 0;
    parsed = strtol(token, &end, 0); // strtol -> converts a string into a long int

    if (errno == ERANGE || *end != '\0' || end == token) {

        return -1;
    }

    if (parsed < INT_MIN || parsed > INT_MAX) {

        return -1;
    }

    *value = (int)parsed;
    return 0;
}

static int tokenize(char *text, char *tokens[], int max_tokens) {
    // takes line of assembly, splits it into individual tokens, stores pointers to those tokens in an array
    // ex: test = "ADD R1, R2, R3" -> tokens[0] = "ADD", tokens[1] = "R1", tokens[2] = "R2", tokens[3] = "R3", count = 4

    int count = 0;
    char *p = text;
    int expect_token = 1; // flag -> parser expects a token 

    while (*p != '\0') { // keep looping while the character in the instruction 'p' points to is not the null terminator
        while (isspace((unsigned char)*p)) { // if current character in instruction is whitespace skip

            p++;
        }

        if (*p == '\0') { // if the character is a null terminator ...

            break;
        }

        if (*p == ',') { // if current character is a comma ...
            if (expect_token) {
                // if expect_token = 1, function was expecting a token but got a comma -> treat as error

                return -1;
            }

            // if comma is valid
            expect_token = 1; // another token is expected
            p++; // move to next character
            continue; // next iteration of main 'while' loop
        }

        if (!expect_token) {

            expect_token = 1;
        }

        if (count >= max_tokens) { 

            return -1;
        }

        tokens[count] = p; // store current location pointer p is pointing to in tokens[]
        count += 1; // increment to next index in tokens[]

        while (*p != '\0' && !isspace((unsigned char)*p) && *p != ',') { // find end of token ...
            // advances p through instruction until a null terminator, whitespace, or a comma

            p++;
        }

        if (*p == ',') { // upon arrival at a comma in the text

            *p = '\0'; // change the comma character to a null terminator
            p++; // advance p past the null terminator
            expect_token = 1; // always an operand after a comma ...
        } else {
            if (*p != '\0') { // token ended w/ whitespace

                *p = '\0';
                p++;
            }

            expect_token = 0;
        }
    }

    if (count > 0 && expect_token) {

        return -1;
    }
    // expect_token = 1 -> ready to begin collecting token
    // expect_token = 0 -> just consumed a token and have not recognized the start of another

    return count;
}

static int set_branch_label(Instrn *instruction, const char *name) {
    // takes a label from a branch instruction, checks it is valid, places into proper instruction category L, stores it for resolving_labels

    size_t length; // num of characters in label name

    if (!valid_label_name(name)) {

        return -1;
    }

    length = strlen(name);

    if (length >= sizeof(instruction->L.L)) {

        return -1;
    }

    strcpy(instruction->L.L, name);
    instruction->L.PC = -1;

    return 0;
}

ParseResult parse_line(const char *line, Instrn *instruction, LABEL *label) {

    char buffer[MAX_LINE_LENGTH];
    char *text;
    char *comment;
    char *tokens[MAX_TOKENS];
    char *colon;
    int count;

    if (line == NULL || instruction == NULL || label == NULL) {

        return PARSE_ERROR;
    }

    if (strlen(line) >= sizeof(buffer)) {

        return PARSE_ERROR;
    }

    strcpy(buffer, line);

    buffer[strcspn(buffer, "\r\n")] = '\0';

    // comment is a pointer that points to an instance of a '#' in the buffer array of the current line -> indicating a comment
    // ex: buffer[] = "ADD R1, R2, R3 # add two registers"; buffer[15] = # -> buffer[15] = '\0', buffer[] = "ADD R1, R2, R3"
    // NOTE: each character in the array gets a 'box' in memory ...
    comment = strchr(buffer, '#');
    if (comment != NULL) {

        *comment = '\0';
    }

    text = trim_whitespace(buffer);

    if (*text == '\0') { // checks to see if 'text' is an empty string ...

        return PARSE_IGNORED;
    }

    colon = strchr(text, ':'); // returns a pointer from the text string pointing to a ':', or NULL if 'text' contains no colon

    if (colon != NULL) {

        size_t length;

        if (colon[1] != '\0' && !isspace((unsigned char)colon[1])) { // accesses character one position after the ':' in text 
            // if there is characters after the colon -> reject

            return PARSE_ERROR;
        }

        // ensures label is all characters up to colon, eliminating the colon
        colon[1] = '\0';
        *colon = '\0'; 

        text = trim_whitespace(text);

        if (!valid_label_name(text)) {

            return PARSE_ERROR;
        }

        length = strlen(text);

        if (length >= sizeof(label->L)) { // if potential label 'text' is too long to fit in the LABEL struct ...

            return PARSE_ERROR;
        }

        strcpy(label->L, text);
        label->PC = -1; // recognizes label name, loader needs to determine which instruction index it refers to

        return PARSE_LABEL;
    }

    count = tokenize(text, tokens, MAX_TOKENS);

    if (count < 1) {

        return PARSE_ERROR;
    }

    memset(instruction, 0, sizeof(*instruction)); // initialize instruction to 0
    instruction->L.PC = -1; // for BRANCH -> instruction label set to -1

    // LOAD_I -> immediate value
    if (strcmp(tokens[0], "LOAD_I") == 0) {
        if (count != 3 || parse_register(tokens[1], &instruction->Rd) != 0 || parse_integer(tokens[2], &instruction->i) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        instruction->op = LOAD_I;
        instruction->type = DATA_MOVEMENT;
    }

    // ADD, SUB
    else if (strcmp(tokens[0], "ADD") == 0 || strcmp(tokens[0], "SUB") == 0) {
        if (count != 4 || parse_register(tokens[1], &instruction->Rd) != 0 || parse_register(tokens[2], &instruction->Rs1) != 0 || parse_register(tokens[3], &instruction->Rs2) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        instruction->op = (strcmp(tokens[0], "ADD") == 0) ? ADD : SUB; // if tokens[0] == ADD -> op, else SUB -> op
        instruction->type = ALU;
    }

    // MOVE
    else if (strcmp(tokens[0], "MOVE") == 0) {
        if (count != 3 || parse_register(tokens[1], &instruction->Rd) != 0 || parse_register(tokens[2], &instruction->Rs) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        instruction->op = MOVE;
        instruction->type = DATA_MOVEMENT;
    }

    // LOAD, LOAD_B
    else if (strcmp(tokens[0], "LOAD") == 0 || strcmp(tokens[0], "LOAD_B") == 0) {
        if (count != 4 || parse_register(tokens[1], &instruction->Rd) != 0 || parse_register(tokens[2], &instruction->Rb) != 0 || parse_integer(tokens[3], &instruction->imm) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        instruction->op = (strcmp(tokens[0], "LOAD") == 0) ? LOAD : LOAD_B;
        instruction->type = LOAD_STORE;
    }

    // STORE, STORE_B
    else if (strcmp(tokens[0], "STORE") == 0 || strcmp(tokens[0], "STORE_B") == 0) {
        if (count != 4 || parse_register(tokens[1], &instruction->Rs) != 0 || parse_register(tokens[2], &instruction->Rb) != 0 || parse_integer(tokens[3], &instruction->imm) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        instruction->op = (strcmp(tokens[0], "STORE") == 0) ? STORE : STORE_B;
        instruction->type = LOAD_STORE;
    }

    // BEQ, BNE, BLT, BGE
    else if (strcmp(tokens[0], "BEQ") == 0 || strcmp(tokens[0], "BNE") == 0 || strcmp(tokens[0], "BLT") == 0 || strcmp(tokens[0], "BGE") == 0) {
        if (count != 4 || parse_register(tokens[1], &instruction->Rs1) != 0 || parse_register(tokens[2], &instruction->Rs2) != 0 || set_branch_label(instruction, tokens[3]) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        if (strcmp(tokens[0], "BEQ") == 0) {
            instruction->op = BEQ;

        } else if (strcmp(tokens[0], "BNE") == 0) {
            instruction->op = BNE;

        } else if (strcmp(tokens[0], "BLT") == 0) {
            instruction->op = BLT;

        } else {
            instruction->op = BGE;
        }

        instruction->type = BRANCH;
    }

    // JUMP
    else if (strcmp(tokens[0], "JUMP") == 0) {
        if (count != 2 || set_branch_label(instruction, tokens[1]) != 0) { // assign necessary operands, operations

            return PARSE_ERROR;
        }

        instruction->op = JUMP;
        instruction->type = BRANCH;
    }

    // HALT
    else if (strcmp(tokens[0], "HALT") == 0) { // assign necessary operands, operations
        if (count != 1) {

            return PARSE_ERROR;
        }

        instruction->op = HALT;
        instruction->type = HALTED;
    }

    // unknown opCode
    else {

        return PARSE_ERROR;
    }

    return PARSE_INSTRUCTION;
}
