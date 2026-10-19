# sum = 1 + 2 + ... + 10
LOAD_I R0, 0 # PC 0 R0: running sum
LOAD_I R1, 1 # PC 1 R1: i
LOAD_I R2, 11 # PC 2 R2: loop bound
LOAD_I R3, 1 # PC 3 R3: constant 1
loop:
ADD R0, R0, R1 # PC 4 sum <- sum + i
ADD R1, R1, R3 # PC 5 i <- i + 1
BLT R1, R2, loop # PC 6 backward branch: taken while i < 11
HALT