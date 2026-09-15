#include <stdio.h>
#include <stdlib.h>

/*
    build a simple machine that completes fetch-decode-execute loop.
    fetch-decode-execute cycle:
        continuous loop that a computer's CPU uses to read and run every program instruction from boot-up to shutdown
    
        fetch: CPU gets the next instruction from the computer's memory (RAM)
               using an address held in a register called the program counter.
        decode: the control unit translates the binary instruction into signals
                that tell the rest of the CPU hardware what circuits or operations to turn on.
        execute: ALU or other parts of the processor perform the actual action,
                 like adding numbers, moving data, or jumping to a new instruction address.
*/



