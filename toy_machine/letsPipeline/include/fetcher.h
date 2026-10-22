#ifndef FETCHER_H
#define FETCHER_H

#include "../../architectureLayer/machine_state.h"
#include "../../thePredictors/include/predictors.h"

void fetch(CPU *cpu, Instrn currFetch, PC *predict);

#endif