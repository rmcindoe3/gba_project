#include "levels.h"

unsigned char level_index = 0;

unsigned char curr_level = 1;

char level_one_string[15] = "LEVEL ONE";
char level_two_string[15] = "LEVEL TWO";
char level_three_string[15] = "LEVEL THREE";
char level_four_string[15] = "LEVEL FOUR";

char* levels[4] = {
    level_one,
    level_two,
    level_three,
    level_four
};

char level_one[21] = {
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    BOSS
};

char level_two[21] = {
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    BOSS
};

char level_three[21] = {
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    BOSS
};

char level_four[21] = {
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, 
    BOSS
};

char getNextEnemyType(void) {
    return levels[curr_level][level_index];
}

char pullNextEnemy(void) {
    return levels[curr_level][level_index++];
}

void advanceLevel(void) {
    curr_level++;
    curr_level %= NUM_LEVELS;
    level_index = 0;
}

char* getLevelString(void) {
    if(curr_level == 1) return level_one_string;
    if(curr_level == 2) return level_two_string;
    if(curr_level == 3) return level_three_string;
    if(curr_level == 4) return level_four_string;
}

void reset_level() {
    level_index = 0;
}


