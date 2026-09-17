
#include "../decoder.h"


// decode receives the instruction register from fetch and returns this register
Instrn decode(Instrn IR) {

    return IR;
}

/*
    because we have chosen to store instructions (IR) as an Instrn struct,
    this is all that needs to be done! --> return IR

    separating parsing from decoding:
        decode --> takes the Instrn object and makes it available to execute
*/