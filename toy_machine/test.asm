# Test program

LOAD_I R0, 0 
LOAD_I R1, 1 
LOAD_I R2, 11
LOAD_I R3, 1
loop:
ADD R0, R0, R1
ADD R1, R1, R3
BLT R1, R2, loop
HALT