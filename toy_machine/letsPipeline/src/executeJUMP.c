
#include "../include/executioner.h"

void executeJUMP(CPU *cpu, Instrn instruction) {

    cpu->PC = instruction.L.PC;
    
}