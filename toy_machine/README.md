MY TINY CPU!
LEGIT LITTLE PROCESSOR SIMULATOR!

Building my toy_machine in layers:

    1. Machine state
    2. Program storage
    3. Read/parse .asm file
    4. Fetch
    5. Decode
    6. Execute
    7. FDE loop
    8. Print final registers
    9. Compile/text
    10. Handle edge cases

architectureLayer

1) Machine State: 'machine_state.h'
    R0 - R7 registers, program counter, instruction register, available instructions, operands associated with instruction

2) Initial State: 'initial_state.c', 'initial_state.h'
    initialize R0 - R7 registers, program counter, and 'halted' to 0

programmingLayer

3) Program Housing: 'program.c'
    pulls and parses the .asm file through a while loop, increments a program_size variable as each instruction from the .asm is placed in the storage array of type struct Instrn

4) Fetcher: 'fetch.c'
    pulls the correct information out of the program array holding all instructions from the .asm file and gives that information to the cpu.IR, increments the PC++

5) Decoder: 'decoder.c'
    returns the information from the fetcher sends to the 'execute'

6) Executer: 'execute.c'
    performs calculations and updates registers in accordance from the instruction sent by the decoder

To Compile: make clean
            make

To Run: ./FDEloop <input.asm>

All Done! This was too fun :|





