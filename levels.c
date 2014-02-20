#include "levels.h"

//Keeps track of where we are in the level.
unsigned char level_index = 0;

//Sets the current level to the starting level.
unsigned char curr_level = STARTING_LEVEL;

//Strings used when displaying what level we're on in the game.
char level_one_string[15] = "LEVEL ONE";
char level_two_string[15] = "LEVEL TWO";
char level_three_string[15] = "LEVEL THREE";
char level_four_string[15] = "LEVEL FOUR";
char level_five_string[15] = "LEVEL FIVE";
char level_six_string[15] = "LEVEL SIX";
char level_seven_string[15] = "LEVEL SEVEN";
char level_eight_string[15] = "LEVEL EIGHT";
char level_nine_string[15] = "LEVEL NINE";
char level_ten_string[15] = "LEVEL TEN";

//Array of our levels.
char* levels[NUM_LEVELS] = {
    level_one,
    level_two,
    level_three,
    level_four,
    level_five,
    level_six,
    level_seven,
    level_eight,
    level_nine,
    level_ten
};

//Level one enemy order.
char level_one[21] = {
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM,
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM,
    BOSS
};

//Level two enemy order.
char level_two[21] = {
    NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM, NORM,
    BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG,
    BOSS
};

//Level three enemy order.
char level_three[21] = {
    BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG,
    BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG,
    BOSS
};

//Level four enemy order.
char level_four[21] = {
    BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG, BIGG,
    DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, 
    BOSS
};

//Level five enemy order.
char level_five[21] = {
    DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, 
    DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, 
    BOSS
};

//Level six enemy order.
char level_six[21] = {
    DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, DBLS, 
    TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS,
    BOSS
};

//Level seven enemy order.
char level_seven[21] = {
    TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS,
    TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS,
    BOSS
};

//Level eight enemy order.
char level_eight[21] = {
    TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS, TRIS,
    HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME,
    BOSS
};

//Level nine enemy order.
char level_nine[21] = {
    HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME,
    HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME, HOME,
    BOSS
};

//Level ten enemy order.
char level_ten[41] = {
    NORM, BIGG, DBLS, TRIS, HOME, NORM, BIGG, DBLS, TRIS, HOME, 
    NORM, BIGG, DBLS, TRIS, HOME, NORM, BIGG, DBLS, TRIS, HOME, 
    NORM, BIGG, DBLS, TRIS, HOME, NORM, BIGG, DBLS, TRIS, HOME, 
    NORM, BIGG, DBLS, TRIS, HOME, NORM, BIGG, DBLS, TRIS, HOME, 
    BOSS
};


/** getNextEnemyType *****************************************************
 * Returns the next enemy type for our current level.
 *************************************************************/
char getNextEnemyType(void) {
    return levels[curr_level][level_index];
}

/** pullNextEnemy *****************************************************
 * Returns the next enemy type for our current level and 
 *  then advances forward in the level.
 *************************************************************/
char pullNextEnemy(void) {

    //If the next enemy is a boss, don't increment the counter
    //  for the level index because the level is complete.
    if(levels[curr_level][level_index] == BOSS) {
        return levels[curr_level][level_index];
    }
    else {
        return levels[curr_level][level_index++];
    }
}

/** getCurrentLevel *****************************************************
 * Returns which level we are currently on.
 *************************************************************/
unsigned char getCurrentLevel(void) {
    return curr_level;
}

/** advanceLevel *****************************************************
 * Moves the game foward a level.
 *************************************************************/
void advanceLevel(void) {

    //Increments our current level.
    curr_level++;

    //If we've gone past the final level, start over.
    curr_level %= NUM_LEVELS;

    //Make sure we reset the level index back to the beginning.
    level_index = 0;
}

/** getLevelString *****************************************************
 * Returns a string that shows which level we're currently on.
 *************************************************************/
char* getLevelString(void) {
    if(curr_level == 0) return level_one_string;
    if(curr_level == 1) return level_two_string;
    if(curr_level == 2) return level_three_string;
    if(curr_level == 3) return level_four_string;
    if(curr_level == 4) return level_five_string;
    if(curr_level == 5) return level_six_string;
    if(curr_level == 6) return level_seven_string;
    if(curr_level == 7) return level_eight_string;
    if(curr_level == 8) return level_nine_string;
    if(curr_level == 9) return level_ten_string;
}

/** reset_level *****************************************************
 * Resets the game when the user loses or presses select.
 *************************************************************/
void reset_level() {
    level_index = 0;
    curr_level = STARTING_LEVEL;
}


