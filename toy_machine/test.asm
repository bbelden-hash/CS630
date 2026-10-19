# Test program

# for i = 0 to 3:
# for j = 0 to 3:
# if i != j: count = count + 1
LOAD_I R0, 0 # PC 0 R0: count
LOAD_I R1, 0 # PC 1 R1: i
LOAD_I R3, 4 # PC 2 R3: bound
LOAD_I R4, 1 # PC 3 R4: constant 1
outer:
LOAD_I R2, 0 # PC 4 R2: j
inner:
BEQ R1, R2, skip # PC 5 forward branch: skip the ADD if i == j
ADD R0, R0, R4 # PC 6
skip:
ADD R2, R2, R4 # PC 7
BLT R2, R3, inner # PC 8 inner back edge
ADD R1, R1, R4 # PC 9
BLT R1, R3, outer # PC 10 outer back edge
HALT