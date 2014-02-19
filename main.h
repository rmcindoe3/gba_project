#ifndef MAIN_H
#define	MAIN_H

#include "enemy_llist.h"
#include "bullet_llist.h"

//Game states
#define GAME 0x01
#define PAUSE 0x02
#define SHOP 0x03

#define MAX_HEALTH 10

/*** THE FOLLOWING NEEDS TO BE MOVED TO IT'S OWN FILE ***/
//Struct that contains the relevant information for our SHIP
typedef struct ship {
    char row;
    char col;
    char height;
    char width;
    char health;
    char weapon_level;
} SHIP;

//Our ship structs that represent the user.
SHIP ship;
SHIP ship_old;

/*** UP TO HERE ***/

//Variables that store what state the game is in.
extern unsigned short state;
extern unsigned short state_old;

//Keeps track of how many points the user has.
extern unsigned short score;

//Keeps track of bullets shot and bullets hit for hit rate calculation.
unsigned short bullets_shot;
unsigned short bullets_hit;

//Keeps track of user input.
unsigned int buttons;
unsigned int oldButtons;

/*** Full explanations of these functions ***
 *** can be found in the main.c file.     ***/
void drawBullets(int erase);
unsigned short determineBulletColor(void);
void drawEnemies(int erase);
void drawShip(int erase);
void drawEnemy(ENEMY* enemy);

char collision(ENEMY* enemy, BULLET* bullet);
char shipCollision(BULLET* bullet);

void init(void);

void checkGameButtons(void);
void moveGameObjects(void);
void enemiesFire(void);

void checkCollisions(void);
void eraseOldObjects(void);
void drawNewObjects(void);
void drawGameText(void);
void updateOldVariables(void);

void displayPauseScreen(void);
void checkPauseButtons(void);

void checkShopButtons(void);
void displayShopScreen(void);
void drawCursor(void);

#endif	/* MAIN_H */

