
#include "label.h"

Label find_or_create_label(LocalLabelTable *table, const char *name) {

    for (unsigned int i = 0; i < table->next_label; i++) {
        if (strcmp(table->names[i].name, name) == 0) {
            return table->names[i].id;
        }
    }

    assert(table->next_label < MAX_LABELS);
    
    Label id = table->next_label++;
    strncpy(table->names[id].name, name, MAX_LABEL_NAME - 1);
    table->names[id].id = id;
    table->entries[id].position = -1;
    table->entries[id].resolved = 0;

    return id;
}

int bind_label(LocalLabelTable *table, Label id, int position) {
    if (table == NULL || id >= table->next_label) {
        return -1;
    }

    if (table->entries[id].resolved) {
        return -1;
    }

    table->entries[id].position = position;
    table->entries[id].resolved = 1;

    return 0;
}

void resolve_branch(LocalLabelTable *table, Instrn *instruction, Label target) {

    /* debug
    printf("target label: %s\n", table->names[target].name);
    printf("resolving target ID: %u\n", target);
    printf("num of labels: %u\n", table->next_label);
    printf("resolved: %d\n", table->entries[target].resolved);
    */

    assert(table->entries[target].resolved);

    instruction->npc = table->entries[target].position;
}