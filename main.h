

#ifndef MAIN_H
#define	MAIN_H

//Game states
#define GAME 0x01
#define PAUSE 0x02
#define SHOP 0x03

#define MAX_HEALTH 10

//Struct that contains the relevant information for our SHIP
typedef struct ship {
    char row;
    char col;
    char height;
    char width;
    char health;
    char weapon_level;
} SHIP;

//Variables that store what state the game is in.
u16 state = GAME;
u16 state_old = GAME;

//Keeps track of how many points the user has.
unsigned short score = 0;
unsigned short money = 0;
unsigned short bullets_shot;
unsigned short bullets_hit;
char shop_cursor_pos = 0;

//Keeps track of user input.
unsigned int buttons;
unsigned int oldButtons;

//Our ship structs that represent the user.
SHIP ship;
SHIP ship_old;

//Strings used to display text information on the screen.
extern char scoreStr[20];
extern char healthStr[20];

/*** Full explanations of these functions ***
 *** can be found in the main.c file.     ***/
void drawBullets(int erase);
unsigned short determineBulletColor(void);
void drawEnemies(int erase);
void drawShip(SHIP* ship, int erase);
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
void createWeaponUpgradeString(char* str);
void purchaseItem(char cursor_pos);
void purchaseWeaponUpgrade(void);
void purchaseHealth(void);
void drawCursor(void);
unsigned short determineBulletColor(void);

void upgradeWeapon(void);
int determineWeaponCost(void);

#endif	/* MAIN_H */

