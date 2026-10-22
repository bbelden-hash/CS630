# Test program

LOAD_I R0, 0
LOAD_I R1, 0
LOAD_I R3, 4
LOAD_I R4, 1
outer:
LOAD_I R2, 0
inner:
BEQ R1, R2, skip
ADD R0, R0, R4
skip:
ADD R2, R2, R4
BLT R2, R3, inner
ADD R1, R1, R4 
BLT R1, R3, outer
HALT