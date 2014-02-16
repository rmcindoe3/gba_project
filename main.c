#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "enemy_llist.h"
#include "main.h"


int main() {
    REG_DISPCTL = 1027; //sets up the display in mode 3 with the settings we want
    fillBackground(BGCOLOR); //clears the screen in case anything else is present

    init();

    //Main loop
    while(1) {

	oldButtons = buttons;
	buttons = BUTTONS;

	if(state == GAME) {

	    checkGameButtons();
	    moveGameObjects();
	    checkCollisions();

	    waitForVblank();

	    eraseOldObjects();
	    drawNewObjects();
	    drawGameText();

	    updateOldVariables();

	    //Pauses for a bit before moving to the next frame
	    pause(2);

	} else if(state == PAUSE) {

	    if(state_old == GAME) {
		displayPauseScreen();
		state_old = PAUSE;
	    }

	    checkPauseButtons();

	}
    }
}

/** drawBullets ******************************************
 * Given an array of bullet OBJECTs, it will erase them
 *  from the screen.
 *********************************************************/
void drawBullets(BULLET* bull, int erase) {
    static int clr = 0;
    if(!erase) clr++;
    int i = 0;
    for(i = 0; i < MAX_BULLETS; i++) {
	if(bull[i].velocity != 0) {
	    if(erase) drawRect(bull[i].row, bull[i].col, bull[i].height, bull[i].width, BGCOLOR);
	    else drawRect(bull[i].row, bull[i].col, bull[i].height, bull[i].width, bull_colors[clr%6]);
	}
    }
}

/** drawEnemies ********************************************
 *
 *********************************************************/
void drawEnemies(int erase) {

    struct enemy_llist* temp = get_enemy_head();

    while(temp != NULL) {

	if(erase) {
	    drawRect(temp->old_val->row, temp->old_val->col, temp->old_val->height, temp->old_val->width, BGCOLOR);
	}
	else {
	    drawEnemy(temp->val);
	}

	temp = temp->next;
    }
}

/** drawShip ********************************************
 * Draws a ship at (row, col).  ERASE is true when you want this method to 
 * erase the ship at the given location instead of draw a new one. 
 *********************************************************/
void drawShip(SHIP* ship, int erase) {
    if (!erase) {
	int r;
	for(r = 0; r < 20; r++) {
	    DMANow(3, &(ship_picture[r*20]), &VIDEO_BUFFER[OFFSET(ship->row + r, ship->col, SCREENWIDTH)],
		    20 |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
	}
    } else {
	drawRect(ship->row, ship->col, 20, 20, BGCOLOR);
    }
}

/** drawEnemy ********************************************
 *
 *********************************************************/
void drawEnemy(ENEMY* enemy) {
    int i = 0;
    for(i = 0; i < 10; i++) {
	DMANow(3, &(enemy_picture[i*20]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
		20 |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
    }
}

/** moveBullets ******************************************
 * Takes in an array of bullet OBJECTs and moves them.
 *********************************************************/
void moveBullets(BULLET* obj) {
    int i = 0;
    for(i = 0; i < MAX_BULLETS; i++) {
	BULLET* temp = (&(obj[i]));
	if(temp->velocity != 0) {
	    temp->row += temp->velocity;
	    if(temp->row <= 3) {
		temp->velocity = 0;
		num_bullets--;
	    }
	}
    }
}


/** collision ********************************************
 * Takes in two OBJECT's and sees if those two objects are
 *  overlapping.
 *********************************************************/
char collision(ENEMY* enemy, BULLET* bullet) {
    if((((bullet->col + bullet->width) > enemy->col)) && (bullet->col < (enemy->col + enemy->width))) 
	if((((bullet->row + bullet->height) > enemy->row)) && (bullet->row < (enemy->row + enemy->height)))
	    return 1;
    return 0;
}

void checkGameButtons() {

    /**** THIS SECTION GENERATES THE BULLETS THAT ARE SHOT WITH A ****/
    if(BUTTON_PRESSED(BUTTON_A)) {
	if(num_bullets < MAX_BULLETS) {
	    num_bullets++;
	    BULLET* temp = bullets;
	    while(temp->velocity != 0) temp++;
	    temp->row = ship.row - 5;
	    temp->col = ship.col + 9;
	    temp->velocity = -3;
	}
    }

    if(BUTTON_PRESSED(BUTTON_START)) {
	state = PAUSE;
    }

    if(BUTTON_PRESSED(BUTTON_SELECT)) {
	init();
	fillBackground(BGCOLOR);
	score = 0;
	num_active_enemies = 0;
	num_bullets = 0;
    }

}

void moveGameObjects() {

    /**** THIS SECTION MOVES ALL THE PIECES  ****/
    if(BUTTON_HELD(BUTTON_LEFT)) {
	if(ship.col) {
	    ship.col -= 5;
	}
    }
    if(BUTTON_HELD(BUTTON_RIGHT)) {
	if(ship.col < 220) {
	    ship.col += 5;
	}
    }

    moveEnemies();
    moveBullets(bullets);

}

void checkCollisions() {

    int i;
    struct enemy_llist* temp = get_enemy_head();

    while(temp != NULL) {

	for(i = 0; i < MAX_BULLETS; i++) {
	    if(bullets[i].velocity != 0) {
		if(collision(temp->val, &(bullets[i]))) {
		    drawRect(temp->old_val->row, temp->old_val->col, temp->old_val->height, temp->old_val->width, BGCOLOR);

		    delete_from_enemy_list(temp->val);

		    bullets[i].velocity = 0;
		    num_bullets--;

		    score++;
		    score_change = 1;
		}
	    }
	}
	temp = temp->next;
    }
    /**** THIS SECTION DETECTS ANY COLLISIONS BETWEEN MOVING OBJECTS ****/
    /*for (i = 0; i < NUM_ENEMIES; i++) {
	if(collisionShip(&ship, &(enemies[i]))) {
	    drawRect(enemies[i].row, enemies[i].col, enemies[i].height, enemies[i].width, BGCOLOR);
	    enemies[i].velocity = 0;
	    enemies[i].row = 0;
	    num_active_enemies--;
	    score = 0;
	    score_change = 1;
	}
	for(j = 0; j < MAX_BULLETS; j++) {
	    if(bullets[j].velocity != 0 && enemies[i].velocity) {
		if(collisionBullet(&(enemies[i]), &(bullets[j]))) {
		    drawRect(enemies[i].row, enemies[i].col, enemies[i].height, enemies[i].width, BGCOLOR);
		    enemies[i].velocity = 0;
		    enemies[i].row = 0;
		    num_active_enemies--;

		    bullets[j].velocity = 0;
		    num_bullets--;

		    score++;
		    score_change = 1;
		}
	    }
	}
    }*/
}

void eraseOldObjects() {

    //Clears the screen if the game was just unpaused.
    if(state_old == PAUSE) {
	fillBackground(BGCOLOR);
	state_old = GAME;
    }

    /**** THIS SECTION ERASES OLD STUFF ****/
    drawEnemies(TRUE);
    drawBullets(bullets_old, TRUE);
    drawShip(&ship_old, TRUE);
}

void drawNewObjects() {
    /**** THIS SECTION DRAWS THE NEW LOCATIONS OF EVERYTHING ****/
    drawShip(&ship, FALSE);
    drawBullets(bullets, FALSE);
    drawEnemies(FALSE);
}

void drawGameText() {
    /**** THIS SECTION DRAWS THE TEXT TO THE SCREEN ****/

    if(score_change) {
	score_change = 0;
	drawRect(150, 160, 8, 6*12, BGCOLOR);
    }

    sprintf(scoreStr, "SCORE: %04d", score);
    drawString(150, 160, scoreStr, BLUE);
}


void updateOldVariables() {
    int i;
    //**** THIS SECTION UPDATES THE OLD VARIABLES ****/
    for(i = 0; i < MAX_BULLETS; i++) {
	bullets_old[i].row = bullets[i].row;
	bullets_old[i].col = bullets[i].col;
	bullets_old[i].height = bullets[i].height;
	bullets_old[i].width = bullets[i].width;
	bullets_old[i].velocity = bullets[i].velocity;
    }

    struct enemy_llist* temp = get_enemy_head();

    while(temp != NULL) {
	updateOldEnemy(temp);
	temp = temp->next;
    }

    ship_old.row = ship.row;
    ship_old.col = ship.col;
    ship_old.height = ship_old.height;
    ship_old.width = ship_old.width;
}

void displayPauseScreen() {
    fillBackground(BLACK);
    char pauseStr[] = "PAUSED";
    sprintf(scoreStr, "SCORE: %04d", score);
    drawString(86, 120-33, scoreStr, BLUE);
    drawString(76, 102, pauseStr, BLUE);
}

void checkPauseButtons() {
    if(BUTTON_PRESSED(BUTTON_START)) {
	state = GAME;
    }
    if(BUTTON_PRESSED(BUTTON_SELECT)) {
	init();
	fillBackground(BLACK);
	char pauseStr[] = "PAUSED";
	drawString(76, 102, pauseStr, BLUE);
	score = 0;
	num_active_enemies = 0;
	num_bullets = 0;
    }
}

/** init **************************************************
 * Initializes all of our objects with new variable locaitons
 *********************************************************/
void init() {

    int i = 0;

    empty_enemy_list();
    create_enemy_list(NULL);

    ship.row = 120;
    ship.col = 80;
    ship.height = 20;
    ship.width = 20;

    ship_old.row = ship.row;
    ship_old.col = ship.col;
    ship_old.height = ship.height;
    ship_old.width = ship.width;

    for(i = 0; i < MAX_BULLETS; i++) {
	bullets[i].height = 5;
	bullets[i].width = 3;
	bullets[i].velocity = 0;

	bullets_old[i].height = bullets[i].height;
	bullets_old[i].width = bullets[i].width;
	bullets_old[i].velocity = bullets[i].velocity;
    }
}
