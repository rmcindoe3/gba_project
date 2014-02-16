

#ifndef MAIN_H
#define	MAIN_H

#define BGCOLOR CYAN

//Game states
#define GAME 0x01
#define PAUSE 0x02


//Struct that contains the relevant information for an SHIP
typedef struct ship {
    char row;
    char col;
    char height;
    char width;
} SHIP;

u16 state = GAME;
u16 state_old = GAME;

u16 num_active_enemies = 0;
u16 score = 0;

unsigned int buttons;
unsigned int oldButtons;

SHIP ship;

SHIP ship_old;

char scoreStr[20];			//char array to store string that will display score

//Variables used to minimize re-drawing text portions of the screen
u16 score_change = 0;

void drawBullets(int erase);
void drawEnemies(int erase);
void drawShip(SHIP* ship, int erase);
void drawEnemy(ENEMY* enemy);

char collision(ENEMY* enemy, BULLET* bullet);

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


#endif	/* MAIN_H */

