/*
    - simulator runs four predictors at the same time
    - each predictor makes a guess (taken or not taken) for every conditional branch
    - predictions are taken before the branch executes
    - the guess is then compared with the actual outcome
*/

#include "../include/predictors.h"

// always predicts not taken
int NT(void) {
    // 0 -> not taken, 1 -> taken

    return 0;
}

// if the branch requires moving back up the array, predict taken
int BTFN(int branch, int target) {
    
    // target: location of label PC BRANCH uses to branch, jump to
    // branch: location of the actual BRANCH instruction, PC
    if (target <= branch) {

        return 1; // taken
    }

    return 0; // not taken
}

void initialize_BHT(BHT *tables) {
    // initializes all values in the 1BIT table to 0
    // initializes all values in the 2BIT table to 1

    for (int i = 0; i < 16; i++) {
        tables->BHT_1BIT[i].value = 0;
        tables->BHT_2BIT[i].value = 1;
    }
}

int calculate_1BIT_use(int branchPC, OneBit *table_1BIT) {

    int index = branchPC % 16;
    int guess = table_1BIT[index].value;

    return guess;
}

int calculate_2BIT_use(int branchPC, TwoBit *table_2BIT) {

    int index = branchPC % 16;
    int guess = table_2BIT[index].value;

    if (guess == 0 || guess == 1) {
        guess = 0;
    } else if (guess == 2 || guess == 3) {
        guess = 1;
    }

    return guess;
}

void update_1BIT_index(int value, int branchPC, OneBit *table_1BIT) {

    int index = branchPC % 16;
    table_1BIT[index].value = value; 
}

void update_2BIT_index(int taken, int branchPC, TwoBit *table_2BIT) {

    int index = branchPC % 16;
    int out = table_2BIT[index].value;

    if (taken == 1) {
        if (out < 3) {

            out++;
        }
    } else if (taken == 0) {
        if (out > 0) {

            out--;
        }
    }

    table_2BIT[index].value = out;
}

void check_prediction(int taken, BranchPredictors *predictor, MisPrediction *mis) {

    if (taken == 1) {

        mis->MISS_NT += 1;
    }
    if (taken == 1 && predictor->BTFN == 0) {

        mis->MISS_BTFN += 1;
    }
    if (taken == 0 && predictor->BTFN == 1) {

        mis->MISS_BTFN += 1;
    }
    if (taken == 0 && predictor->BHT_1BIT == 1) {

        mis->MISS_1BIT += 1;
    }
    if (taken == 1 && predictor->BHT_1BIT == 0) {

        mis->MISS_1BIT += 1;
    }
    if (taken == 1 && predictor->BHT_2BIT == 0) {

        mis->MISS_2BIT += 1;
    }
    if (taken == 0 && predictor->BHT_2BIT == 1) {

        mis->MISS_2BIT += 1;
    }

}





