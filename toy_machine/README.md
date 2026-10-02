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
    - struct CPU:
        * 8 general-purpose registers
        * program counter - address of next instruction to run
        * instruction register - program instruction
        * halted
    - struct MEM:
        * a flat array of 1024 bytes, addressed 0-1023
    - struct Opcode:
        * LOAD_I
        * ADD
        * SUB
        * MOVE
        * LOAD
        * STORE
        * LOAD_B
        * STORE_B
        * HALT
    - struct InstrnType:
        * ALU
        * LOAD_STORE
        * BRANCH
        * DATA_MOVEMENT
        * HALTED
    - struct Instrn
        * op - the operation to be performed on the operands (LOAD, ADD, SUB, etc.)
        * Rb - the base register for load, store operations; starting memory address
        * Rd - the destination register, the operand (register) in which the execution performed will be stored
        * Rs - the source register for a data movement instruction type
        * Rs1 - a source register (1) for ALU instruction type
        * Rs2 - a source register (2) for ALU instruction type
        * i - immediate value assigned to a register through 'LOAD_I'
        * imm - the displacement offset incremented or decremented to the base register for load, store operations
        * type - the genre of instruction

2) Initial State: 'initial_state.c', 'initial_state.h'
    - files are responsible for initializing all objects in 'CPU' struct and 'MEM' struct from 'machine_state.h' to zero

programmingLayer

3) Program Housing: 'program.c'
    - int load_program(const char *filename, Instrn *program)
        * function takes a pointer to an .asm file (argv[1]) and an array with each index being of type struct Instrn
        * .asm file read line by line, blank lines are ignored and lines beginning with '#' are ignored
        * each line in .asm file is an instruction consisting of operands and an operation
        * each line in .asm file is parsed and saved as an index in the 'program' array
        * file is closed

4) Fetcher: 'fetch.c'
    - void fetch(CPU *cpu, Instrn *program)
        * function takes a pointer to a struct 'CPU' from 'machine_state.h' in memory and a pointer to a struct 'Instrn' from 'machine_state.h' in memory
        * both pointers point to the address of its very first member in each struct
        * finds the index in the 'program' array corresponding to the current PC number
        * assigns this index of type 'Instrn' to one of struct 'CPU's objects - the instruction register (IR)
        * IR is the pass between fetch and decode
        * increments the PC by one

5) Decoder: 'decoder.c'
    - Instrn decode(Instrn IR)
        * function takes the instruction register assigned to cpu->IR in the fetch stage as a parameter
        * decode receives this object and returns it

6) Executer: dir 'execute'
    - void executeALU(CPU *cpu, Instrn instruction)
        * function takes a pointer to a struct 'CPU' from 'machine_state.h' in memory and a struct 'Instrn' from 'machine_state.h' called instruction
        * the instruction type required to initiate this function is an instruction that utilizes the ALU requiring computation
        * operations could be 'ADD', 'SUB', 'AND', 'OR', 'XOR'
    - void executeBRANCH(CPU *cpu, Instrn instruction)
        * function takes a pointer to a struct 'CPU' from 'machine_state.h' in memory and a struct 'Instrn' from 'machine_state.h' called instruction
        * TBD: will take 'branching' instructions
    - void executeDATAMOVEMENT(CPU *cpu, Instrn instruction)
        * function takes a pointer to a struct 'CPU' from 'machine_state.h' in memory and a struct 'Instrn' from 'machine_state.h' called instruction
        * instructions with data movement, instructions are processor commands that copy data between registers or load an immediate value
        * consist of operations like 'MOVE' or 'LOAD_I'
    - executeLOADSTORE.c
        * int wrapAddress(int address)
            * function takes the base memory address for operations like 'STORE' or 'LOAD'
            * confirms all memory addresses are valid and consecutive using a wrap around technique if required
        * void executeLOADSTORE(CPU *cpu, MEM *ram, Instrn instruction)
            * function takes a pointer to a struct 'CPU' from 'machine_state.h' in memory, a struct 'Instrn' from 'machine_state.h' called instruction, and a pointer to a struct 'MEM' from 'machine_state.h' in memory
            * responsible for load and store operations
            * utilizes the memory array for storage and loading from
            * utilizes the cpu to acquire base addresses and offsets to find proper locations in 'MEM'


-> To Compile: 
    1) make clean
    2) make

-> To Run: 
    1) ./FDEloop <input.asm>

For Docker:
- [ ] `cd docker`
- [ ] `docker build -t toy-machine .`
- [ ] `docker run -it --rm -v "$(cd .. && pwd):/toy_machine" toy-machine`
- [ ] `cd /toy_machine`
- [ ] `ls`
- [ ] `make clean`
- [ ] `make`
- [ ] `find /toy_machine -name "hw_check.sh"`
- [ ] `cat docker/hw_check.sh` &mdash; *make sure executables match up*
- [ ] `chmod +x hw_docker/hw_check.sh` &mdash; *permission denied*
- [ ] `bash docker/hw_check.sh ./FDEloop`
- [ ] To leave &mdash; `exit`

My hw_check.sh most recent output ->

    PASS  input-1      SUB: Rd <- Rs1 - Rs2, positive and negative results
    PASS  input-2      MOVE: copy a register, source is left unchanged
    PASS  input-4      handout example 1: store two words, load them back, SUB, MOVE
    PASS  input-5      handout example 2: four STORE_B bytes read back as one little-endian word
    PASS  input-6      memory starts zeroed: loads from untouched addresses give 0
    PASS  input-7      STORE a word, then LOAD_B each byte: little-endian byte order
    PASS  input-9      LOAD_B zero-extends: a stored 0xFF byte loads as 255, not -1
    PASS  input-10     STORE_B writes only the low 8 bits of the register
    PASS  input-11     overwrite a word: a later LOAD sees the newer STORE
    PASS  input-13     SUB wraps around (two's complement): MIN - 1 and 0 - MIN

10/10 public sample cases passed


All Done! Still fun :|





