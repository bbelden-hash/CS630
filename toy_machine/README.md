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
- [ ] `cd hw0_docker`
- [ ] `docker build -t toy-machine .`
- [ ] `docker run -it --rm -v "$(cd .. && pwd):/toy_machine" toy-machine`
- [ ] `cd /toy_machine`
- [ ] `ls`
- [ ] `make clean`
- [ ] `make`
- [ ] `find /toy_machine -name "hw0_check.sh"`
- [ ] `cat hw0_docker/hw0_check.sh` &mdash; *make sure executables match up*
- [ ] `chmod +x hw0_docker/hw0_check.sh` &mdash; *permission denied*
- [ ] `bash hw0_docker/hw0_check.sh ./FDEloop`
- [ ] To leave &mdash; `exit`

My hw0_check.sh Output ->

    I. PASS  input-1      Halt directly, check the init state
    II. PASS  input-2      LOAD_I: place an immediate into a register
    III. PASS  input-3      ADD: Rd <- Rs1 + Rs2
    IV. PASS  input-4      compute 30 + 10, then 2 + result
    V. PASS  input-5      every register written, printed R0..R7 in order
    VI. PASS  input-6      comments and blank lines are ignored
    VII. PASS  input-7      dead code: only the first HALT is reached
    VII. PASS  input-8     R0 is a general-purpose register here (no hardcoded zero)
    IX. PASS  input-9     operands are read before the write: Rd may also be a source
    X. PASS  input-10     INT32_MAX + 1 wraps to INT32_MIN (two's complement)

10/10 public sample cases passed
(this is a sample self-check, not the full graded suite)


All Done! This was too fun :|





