

#ifndef MAIN_H
#define	MAIN_H

#define NUM_ENEMIES 8
#define MAX_BULLETS 5

#define BGCOLOR CYAN

//Used for boolean expressions
#define TRUE 0x01
#define FALSE 0x00

//Game states
#define GAME 0x01
#define PAUSE 0x02

//Velocity vars
#define LEFT 0x01
#define RIGHT 0x02

//Struct that contains the relevant information for an SHIP
typedef struct ship {
    char row;
    char col;
    char height;
    char width;
} SHIP;

//Struct that contains relevant information for BULLETs
typedef struct bullet {
    char row;
    char col;
    char height;
    char width;
    char velocity;
} BULLET;

u16 state = GAME;
u16 state_old = GAME;

u16 num_active_enemies = 0;
u16 num_bullets = 0;
u16 autopilot = FALSE;
u16 score = 0;

unsigned int buttons;
unsigned int oldButtons;

char curr_enemy_velocity = LEFT;

SHIP ship;
ENEMY enemies[NUM_ENEMIES];
BULLET bullets[MAX_BULLETS];

SHIP ship_old;
ENEMY enemies_old[NUM_ENEMIES];
BULLET bullets_old[MAX_BULLETS];

char scoreStr[20];			//char array to store string that will display score

//Variables used to minimize re-drawing text portions of the screen
u16 auto_pilot_change = 0;
u16 score_change = 0;

void drawBullets(BULLET* bullets, int erase);
void drawEnemies(ENEMY* enemies, int erase);
void drawShip(SHIP* ship, int erase);
void drawEnemy(ENEMY* enemy);

void moveShip(SHIP* ship, ENEMY* enemies);
void moveBullets(BULLET* obj);
void moveEnemies(ENEMY* obj);

char collisionShip(SHIP* ship, ENEMY* spike);
char collisionBullet(ENEMY* ship, BULLET* spike);

void init();

void checkGameButtons(void);
void moveGameObjects(void);

void checkCollisions(void);
void eraseOldObjects(void);
void drawNewObjects(void);
void drawGameText(void);
void updateOldVariables(void);

void displayPauseScreen(void);
void checkPauseButtons(void);


#endif	/* MAIN_H */

