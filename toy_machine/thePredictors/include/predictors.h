#ifndef PREDICTORS_H
#define PREDICTORS_H

#include "../../architectureLayer/machine_state.h"

/*
    Static not-taken (NT): always predicts not taken
    Backward-taken, forward-not-taken (BFTN): predicts taken if the branch is at or before the branch (target PC <= branch PC), and not taken otherwise
    1-bit dynamic (1BIT): branch history table of 16 entries, each holding a single bit (the last outcome seen) ...
                          the branch at PC p uses entry p mod 16 ...
                          it predicts whatever that entry holds, and after the branch executes the entry is set to the actual outcome ...
                          all entries start at not taken
    2-bit saturating counter (2BIT): separate 16-entry branch history table ...
                                     indexed the same way, where each entry is a counter from 0 to 3 ...
                                     predicts taken if the counter is >= 2 and not taken otherwise ...
                                     after branch executes, counter goes up by 1 if the branch was taken and down by 1 if it was not, staying between 0 and 3 ...
                                     all entries start at 1
*/

typedef struct {

    unsigned int value : 1;
} OneBit;

typedef struct {

    unsigned int value : 2;
} TwoBit;

typedef struct {

    OneBit BHT_1BIT[16]; // each entry stores exactly 1 bit, each entry remembers just the last outcome of the branch ...
    TwoBit BHT_2BIT[16];
} BHT;

typedef struct {

    int branch; // branch PC 
    int target; // target PC
} PC;

typedef struct {

    int MISS_NT;
    int MISS_BTFN;
    int MISS_1BIT;
    int MISS_2BIT;
} MisPrediction;

typedef struct {

    int NT;
    int BTFN;
    int BHT_1BIT;
    int BHT_2BIT;
} BranchPredictors;

int NT(void);
int BFTN(int branch, int target);
void initialize_BHT(BHT *tables);
int calculate_1BIT_use(int branchPC, OneBit *table_1BIT);
int calculate_2BIT_use(int branchPC, TwoBit *table_2BIT);
void update_1BIT_index(int value, int branchPC, OneBit *table_1BIT);
void update_2BIT_index(int taken, int branchPC, TwoBit *table_2BIT);
void check_prediction(int taken, BranchPredictors *predictor, MisPrediction *mis);

#endif


/*
    NOTES:
        for 1BIT:
            every time the branch executes, replace the stored bit with the actual outcome
            actual taken -> store 1
            actual not taken -> store 0
            PC mod 16?
                PC -> address of an instruction
                mod 16 -> take remainder after dividing the address by 16
                remainder determines which of the 16 entries to use
                use the location of the branch instructions PC for the mod 16 not the location of where the jump goes to

        for 2BIT:
            0 -> strongly not taken, 1 -> weakly not taken, 2 -> weakly taken, 3 -> strongly taken
            0, 1 -> not taken, 2, 3 -> taken
            after branch executes; if it was taken -> increase the counter by 1, if it was not taken -> decrease the counter by 1
            if counter is at 3 and branch is taken again -> counter stays at 3
            if counter is at 0 and branch is not taken again -> counter stays at 0
            all entries start at 1
*/