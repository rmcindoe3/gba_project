#ifndef LEVELS_H_INCLUDED
#define LEVELS_H_INCLUDED

#define NUM_LEVELS 6
#define STARTING_LEVEL 0

#define NORM 0x01
#define BOSS 0x02
#define BIGG 0x03
#define TRIS 0x04

extern unsigned char level_index;
extern unsigned char curr_level;
extern char* levels[NUM_LEVELS];
extern char level_one[21];
extern char level_two[21];
extern char level_three[21];
extern char level_four[21];
extern char level_five[21];
extern char level_six[21];

void reset_level(void);
char getNextEnemyType(void);
char pullNextEnemy(void);
unsigned char getCurrentLevel(void);
void advanceLevel(void);
char* getLevelString(void);



#endif
