

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


void drawBullets(OBJECT bullets[], int erase);
void moveSpikes(OBJECT* obj);
void moveShip(OBJECT* ship, OBJECT* spikes);
void moveBullets(OBJECT* obj);
void eraseSpikes(OBJECT spikes[]);
char collision(OBJECT* ship, OBJECT* spike);
void reinit(OBJECT* spikes, OBJECT* spikes_old, OBJECT* bullets, OBJECT* bullets_old, OBJECT* ship, OBJECT* ship_old);

#endif	/* MAIN_H */

