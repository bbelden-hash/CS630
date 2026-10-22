#ifndef DECODER_H
#define DECODER_H

#include "../../architectureLayer/machine_state.h"
#include "../../thePredictors/include/predictors.h"

Instrn decode(CPU *cpu, PC *predict);

#endif