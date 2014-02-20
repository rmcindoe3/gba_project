#ifndef LEVELS_H_INCLUDED
#define LEVELS_H_INCLUDED

#define NUM_LEVELS 10
#define STARTING_LEVEL 0

//The different types of enemies in the game.
#define NORM 0x00
#define BOSS 0x01
#define BIGG 0x02
#define TRIS 0x03
#define DBLS 0x04

//These are all documented in levels.c
extern unsigned char level_index;
extern unsigned char curr_level;
extern char* levels[NUM_LEVELS];
extern char level_one[21];
extern char level_two[21];
extern char level_three[21];
extern char level_four[21];
extern char level_five[21];
extern char level_six[21];
extern char level_seven[21];
extern char level_eight[21];
extern char level_nine[21];
extern char level_ten[21];

//These are all documented in levels.c
void reset_level(void);
char getNextEnemyType(void);
char pullNextEnemy(void);
unsigned char getCurrentLevel(void);
void advanceLevel(void);
char* getLevelString(void);



#endif
