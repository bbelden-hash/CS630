#ifndef LABEL_H
#define LABEL_H

#include "../machine_state.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define MAX_LABELS 4096
#define MAX_LABEL_NAME 64

// unique integer label identifier
typedef unsigned int Label;

typedef struct {
    int position; // where the label is located
    int resolved; // if we have already determined its location?
} LabelEntry;

// name-to-ID map
typedef struct {
    char name[MAX_LABEL_NAME];
    Label id;
} NamedLabel;

// label table - relationship between label identifier and its final location
typedef struct {
    LabelEntry entries[MAX_LABELS];
    NamedLabel names[MAX_LABELS];

    unsigned int next_label;
} LocalLabelTable;

Label find_or_create_label(LocalLabelTable *table, const char *name);
int bind_label(LocalLabelTable *table, Label id, int position);

void resolve_branch(LocalLabelTable *table, Instrn *instruction, Label target);

#endif