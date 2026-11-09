#ifndef BRANCH_PREDICTORS_H
#define BRANCH_PREDICTORS_H

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
void initialize_BHT(OneBit *BHT_1BIT, TwoBit *BHT_2BIT);
int value_1BIT(OneBit *BHT_1BIT, int p);
int value_2BIT(TwoBit *BHT_2BIT, int p);
void check_prediction(int cond, BranchPredictors *branch_predictor, MisPrediction *misprediction);
void update_1BIT(OneBit *BHT_1BIT, int p, int outcome);
void update_2BIT(TwoBit *BHT_2BIT, int p, int outcome);

#endif