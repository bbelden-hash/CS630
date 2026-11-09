
#include "branch_predictors.h"

// 0 -> not taken, 1 -> taken

// always predicts not taken
int NT(void) {
    return 0;
}

// predicts taken if the branch target is at or before the branch, and not taken otherwise
int BTFN(int target, int branch) {
    if (target <= branch) {
        return 1;
    }

    return 0;
}

// init all values in 1BIT to 0, 2BIT to 1
void initialize_BHT(OneBit *BHT_1BIT, TwoBit *BHT_2BIT) {
    for (int i = 0; i < 16; i++) {
        BHT_1BIT[i].value = 0;
        BHT_2BIT[i].value = 1;
    }
}

int value_1BIT(OneBit *BHT_1BIT, int p) {
    int loc = p % 16;
    int pred = BHT_1BIT[loc].value;

    return pred;
}

int value_2BIT(TwoBit *BHT_2BIT, int p) {
    int loc = p % 16;
    int pred = BHT_2BIT[loc].value;

    if (pred >= 2) {
        return 1;
    } else {
        return 0;
    }
}

void check_prediction(int cond, BranchPredictors *branch_predictor, MisPrediction *misprediction) {

    if (cond == 1) {
        misprediction->MISS_NT++;
    }
    if (cond == 1 && branch_predictor->BTFN == 0) {
        misprediction->MISS_BTFN++;
    }
    if (cond == 0 && branch_predictor->BTFN == 1) {
        misprediction->MISS_BTFN++;
    }
    if (cond == 0 && branch_predictor->BHT_1BIT == 1) {
        misprediction->MISS_1BIT++;
    }
    if (cond == 1 && branch_predictor->BHT_1BIT == 0) {
        misprediction->MISS_1BIT++;
    }
    if (cond == 0 && branch_predictor->BHT_2BIT == 1) {
        misprediction->MISS_2BIT++;
    }
    if (cond == 1 && branch_predictor->BHT_2BIT == 0) {
        misprediction->MISS_2BIT++;
    }
}

void update_1BIT(OneBit *BHT_1BIT, int p, int outcome) {
    int entry = p % 16;

    BHT_1BIT[entry].value = outcome;
}

void update_2BIT(TwoBit *BHT_2BIT, int p, int outcome) {
    int entry = p % 16;

    if (outcome == 1) {
        if (BHT_2BIT[entry].value < 3) {
            BHT_2BIT[entry].value += 1;
        }
    } else if (outcome == 0) {
        if (BHT_2BIT[entry].value > 0) {
            BHT_2BIT[entry].value -= 1;

        }
    }
}

