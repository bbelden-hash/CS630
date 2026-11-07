# Test program

LOAD_I R1, 3
LOAD_I R2, 4
LOAD_I R3, 0
BEQ R1, R2, else
LOAD_I R3, 1
JUMP done
else:
LOAD_I R3, 2
done:
BNE R1, R2, end
LOAD_I R4, 9
end:
HALT