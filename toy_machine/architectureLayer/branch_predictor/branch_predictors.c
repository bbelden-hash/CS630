
#include "branch_predictors.h"

// 0 -> not taken, 1 -> taken

// always predicts not taken
int NT(void) {
    return 0;
}

// init all values in 1BIT to 0, 2BIT to 1
void initialize_BHT(OneBit *BHT_1BIT, TwoBit *BHT_2BIT) {

    for (int i = 0; i < 16; i++) {
        BHT_1BIT[i].value = 0;
        BHT_2BIT[i].value = 1;
    }
}