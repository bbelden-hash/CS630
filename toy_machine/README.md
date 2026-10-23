MY TINY CPU!
LEGIT LITTLE PROCESSOR SIMULATOR!

FDEloop.c
- int main(int argc, char *argv[])
    * main function (driver), takes the 'terminal' as parameters to the function ...
    * argc is the number of objects typed to the terminal when properly running the program separated by a ' '
    * argv are the actual sequence of char inputs typed by the user to the terminal, each input is separated by a ' ' and each 'word' is placed into an array of chars
    * a struct 'CPU' object is created from 'machine_state.h' and every item in the struct is initialized to zero
    * a struct 'MEM' object is created from 'machine_state.h' and every index in the memory array from the struct is initialized to zero
    * a struct 'BHT' object is created from 'predictors.h', this contains two branch tables of size 16 representing the 1BIT and 2BIT predictors
    * a struct 'BranchPredictors' object is created from 'predictors.h', this changes values as the program runs, represents the current guess from each branch predictor
    * a struct 'MisPrediction' object is created from 'predictors.h', this contains all branch predictors total number of mispredicitions calculated throughout the program
    * a struct 'Record' object is created from 'predictors.h', this containes variables BRANCHES and TAKEN -> represents the total number of branches and the number within that total taken
    * an array of 'Instrn' struct objects is created, given the variable name 'program'
    * an array of 'LABEL' struct objects is created, given the variable name 'labels'
    * a variable called 'program_size' is created, this will represent the number of lines (instructions) parsed from the .asm file (argv[1])
    * a variable called 'label_size' is created, this will represent the number of label lines parsed from the .asm file (argv[1])
    * a variable called 'taken' is created, taken's value flunctuates -> 1 if a branch instruction is taken and 0 if a branch instruction is not taken
    * a function called 'load_program' is called using the .asm file (argv[1]), the 'program' array, the 'labels' array, and pointers to 'program_size' and 'label_size' as input parameters
    * 'load_program' will parse the input .asm file line by line, placing each line (struct 'Instrn' object) from argv[1] as an index in the 'program' array
    * upon return from 'load_program', the 'program' array will consist of every instruction the processor needs to complete from the .asm file
    * a function called 'resolve_labels' is called -> sends the full program array through another full cycle; this allows the assignment of the correct PC branching instructions need to branch to
    * if the 'cpu' is not 'HALTED' meaning a particular index in the 'program' arrays 'instruction.op' == 'HALT' or another way to interpret it is a line in the .asm file consists of 'HALT', then ...
    * iterate through the 'program' arrays 'Instrn' objects and perform the necessary steps for each instruction in the .asm file to complete successfully ...
    * call the fetch, decode, and corresponding instruction types (ALU, branch, data movement, load-store, etc.) execute function
    * if the current instruction type is BRANCH, then assign proper predicition values to all predictors and check correctness -> if incorrect, increment the corresponding predictors misprediction variable
    * upon an 'instruction.op' of 'HALT', exit the 'while' loop and print the values: currently sitting in all created registers in the 'CPU' struct from the 'machine_state.h' file, how many total branches were their within the program, how many of those branches were taken, and the total misprediction values from all four branch predictors


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
        * BEQ
        * BNE
        * BLT
        * BGE
        * JUMP
        * HALT
    - struct InstrnType:
        * ALU
        * LOAD_STORE
        * BRANCH
        * JUMPING
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
        * L.L - a label associated with a branch instruction
        * L.PC - the PC the branch instruction needs to branch to if taken
        * branched - a boolean -> true if branch condition holds true, false otherwise
        * type - the genre of instruction

2) Initial State: 'initial_state.c', 'initial_state.h'
    - files are responsible for initializing all objects in 'CPU' struct and 'MEM' struct from 'machine_state.h' to zero

getProgram

3) program_loader -> pulls the next line from the file and sends to the instruction parser file -> upon return from the instruction parser, file is sent to the program build file
4) instruction_parser -> splits the line into tokens, sends each token to check for validity -> if valid fills a Instrn struct object with the line
5) program_build -> puts the Instrn struct object filled with the line into the 'program' array as an index
6) resolve_labels -> iterates through the full 'program' array created, assigns branching instructions target PC to its corresponding labels PC

letsPipeline

7) fetch -> gets the IR, if the IR type is BRANCH -> assigns the branchPC to the current cpu PC (for BTFN), and increments the cpu PC by one
8) decode -> returns the IR to be used in main and determines if a branch is valid if the IR happens to be a branch instruction
9) execute(s) -> performs the necessary operations to successfully complete the instruction

thePredictors

10) NT -> always predicts not taken for a branch
11) BFTN -> predicts taken if the branch is at or before the branch (target PC <= branch PC), and not taken otherwise
12) 1BIT -> branch history table of 16 entries, each holding a single bit (the last outcome seen) ...
            the branch at PC p uses entry p mod 16 ...
            it predicts whatever that entry holds, and after the branch executes the entry is set to the actual outcome ...
            all entries start at not taken
13) 2BIT ->  separate 16-entry branch history table ...
             indexed the same way, where each entry is a counter from 0 to 3 ...
             predicts taken if the counter is >= 2 and not taken otherwise ...
             after branch executes, counter goes up by 1 if the branch was taken and down by 1 if it was not, staying between 0 and 3 ...
             all entries start at 1

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

    PASS  input-1      HW1 regression: no branches, all six statistics lines print 0
    PASS  input-2      handout example 1: sum 1..10 with one backward BLT
    PASS  input-3      handout example 2: nested loop with an if inside (count i != j)
    PASS  input-4      JUMP forward skips code and is not counted as a conditional branch
    PASS  input-5      BEQ and BNE, each once taken and once not taken (forward targets)
    PASS  input-6      BLT/BGE compare signed: -1 < 0 and -5 < -2
    PASS  input-7      forward-referenced labels, and two labels naming the same instruction
    PASS  input-8      while loop: top-test BGE exit plus bottom JUMP (JUMP is not counted)
    PASS  input-9      countdown loop closed by a backward BNE
    PASS  input-10     BGE with equal operands is taken; BLT with equal operands is not

10/10 public sample cases passed


All Done! Still fun :|





