
#include "../include/fetcher.h"

void fetch(CPU *cpu, Instrn currFetch) {

    cpu->IR = currFetch;
    cpu->PC += 1;
}