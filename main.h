#ifndef MAIN_H
#define	MAIN_H

#include "enemy_llist.h"
#include "bullet_llist.h"

//Game states
#define GAME 0x01
#define PAUSE 0x02
#define SHOP 0x03

//The maximum health a player can have
#define MAX_HEALTH 10

//The background color of our game.
#define BGCOLOR CYAN

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
void drawBullet(BULLET* bullet);
unsigned short determineBulletColor(void);

void drawEnemies(int erase);
void drawEnemy(ENEMY* enemy);

void drawShip(int erase);

void checkCollisions(void);
char collision(ENEMY* enemy, BULLET* bullet);
char shipCollision(BULLET* bullet);

void init(void);

void checkGameButtons(void);
void checkPauseButtons(void);
void checkShopButtons(void);

void enemiesFire(void);
void moveGameObjects(void);

void eraseOldObjects(void);
void drawNewObjects(void);
void drawGameText(void);

void updateOldVariables(void);

void displayPauseScreen(void);
void displayShopScreen(void);

void drawCursor(void);

#endif	/* MAIN_H */

