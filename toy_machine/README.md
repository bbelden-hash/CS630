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
    - R0 - R7 registers, program counter, instruction register, available instructions, operands associated with instruction

2) Initial State: 'initial_state.c', 'initial_state.h'
    - initialize R0 - R7 registers, program counter, and 'halted' to 0

programmingLayer

3) Program Housing: 'program.c'
    - pulls and parses the .asm file through a while loop, increments a program_size variable as each instruction from the .asm is placed in the storage array of type struct Instrn

4) Fetcher: 'fetch.c'
    - pulls the correct information out of the program array holding all instructions from the .asm file and gives that information to the cpu.IR, increments the PC++

5) Decoder: 'decoder.c'
    - returns the information from the fetcher sends to the 'execute'

6) Executer: 'execute.c'
    - performs calculations and updates registers in accordance from the instruction sent by the decoder

-> To Compile: 
    1) make clean
    2) make

-> To Run: 
    1) ./FDEloop <input.asm>

For Docker:
    1) cd hw0_docker
    2) docker build -t toy-machine .
    3) docker run -it --rm -v "$(cd .. && pwd):/toy_machine" toy-machine
    4) cd /toy_machine
    5) ls
    6) make clean
    7) make
    8) find /toy_machine -name "hw0_check.sh"
    9) cat hw0_docker/hw0_check.sh --> make sure executables match up
    10) chmod +x hw0_docker/hw0_check.sh --> permission denied
    11) bash hw0_docker/hw0_check.sh ./FDEloop
    12) to leave --> exit

My hw0_check.sh Output ->

PASS  input-1      Halt directly, check the init state
PASS  input-2      LOAD_I: place an immediate into a register
PASS  input-3      ADD: Rd <- Rs1 + Rs2
PASS  input-4      compute 30 + 10, then 2 + result
PASS  input-5      every register written, printed R0..R7 in order
PASS  input-6      comments and blank lines are ignored
PASS  input-7      dead code: only the first HALT is reached
PASS  input-9      R0 is a general-purpose register here (no hardcoded zero)
PASS  input-10     operands are read before the write: Rd may also be a source
PASS  input-14     INT32_MAX + 1 wraps to INT32_MIN (two's complement)

10/10 public sample cases passed
(this is a sample self-check, not the full graded suite)


All Done! This was too fun :|





