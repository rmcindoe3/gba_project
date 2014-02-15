

#ifndef MAIN_H
#define	MAIN_H

#define NUM_SPIKES 12
#define MAX_BULLETS 5

//Used for the type char of OJBECTs
#define SPIKE 0x00
#define SHIP 0x01

//Used for boolean expressions
#define TRUE 0x01
#define FALSE 0x00

//Game states
#define GAME 0x01
#define PAUSE 0x02

u16 state = GAME;
u16 state_old = GAME;

u16 num_falling = 0;
u16 num_bullets = 0;
u16 autopilot = FALSE;
u16 score = 0;

OBJECT spikes[NUM_SPIKES];
OBJECT spikes_old[NUM_SPIKES];
OBJECT ship;
OBJECT ship_old;
OBJECT bullets[MAX_BULLETS];
OBJECT bullets_old[MAX_BULLETS];

char scoreStr[20];			//char array to store string that will display score

//Variables used to minimize re-drawing text portions of the screen
u16 auto_pilot_change = 0;
u16 score_change = 0;

void drawBullets(OBJECT* bullets, int erase);
void drawSpikes(OBJECT* spikes, int erase);
void drawShip(OBJECT* ship, int ERASE);
void drawSpike(OBJECT* spike);

void moveShip(OBJECT* ship, OBJECT* spikes);
void moveBullets(OBJECT* obj);
void moveSpikes(OBJECT* obj);

char collision(OBJECT* ship, OBJECT* spike);

void init();
void checkGameButtons(void);
void checkCollisions();
void moveGameObjects();
void eraseOldObjects();
void drawNewObjects();
void drawGameText();
void updateOldVariables();

#endif	/* MAIN_H */

