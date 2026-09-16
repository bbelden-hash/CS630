 build a simple machine that completes fetch-decode-execute loop.

    fetch-decode-execute cycle:
        continuous loop that a computer's CPU uses to read and run every program instruction from boot-up to shutdown
    
        fetch: CPU gets the next instruction from the computer's memory (RAM)
               using an address held in a register called the program counter.
        decode: the control unit translates the binary instruction into signals
                that tell the rest of the CPU hardware what circuits or operations to turn on.
        execute: ALU or other parts of the processor perform the actual action,
                 like adding numbers, moving data, or jumping to a new instruction address.

Simple Instruction Set:

For operands:
    Rd means the destination register, with d as the index. 
    Rsi is for the source register, with i as the index.

LOAD_I --> Syntax: LOAD_I Rd, i, Semantics: Rd <-- imm, where i is a signed decimal integer literal.
ADD --> Syntax: ADD Rd, Rs1, Rs2, Semantics: Rd <-- Rs1 + Rs2. Operands are red before the write, so ADD R0, R0, R1 is well-defined.
HALT --> Syntax: HALT, Semantics: Stop execution immediately. Any instructions after HALT are never reached.

representation that can answer:
    "What operation is this?"
    "Which register is the destination?"
    "Which registers are the sources?"
    "What immediate integer was provided?"

LOAD_I needs to store two pieces of information: LOAD_I R3, 7 = R3 <- 7
    destination register (R3)
    immediate value (7)
ADD needs to store three pieces of information: ADD R0, R2, R6 = R0 <- R2 + R6
    destination register (R0)
    source1 register (R2)
    source2 register (R6)
HALT needs to store, "This is a HALT instruction":
    a STOP

Architecture Layer --> define what exists, machine state
Programming Layer --> establish the initial state and perform fetch, decode, execute



