#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "enemy_llist.h"
#include "bullet_llist.h"
#include "main.h"

char bullets_to_be_erased = 0;

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
void drawBullets(int erase) {

    static int clr = 0;
    if(!erase) clr++;

    struct bullet_llist* bullet_list = get_bullet_head();

    while(bullet_list != NULL) {

	if(erase) drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
	else drawRect(bullet_list->val->row, bullet_list->val->col, bullet_list->val->height, bullet_list->val->width, bull_colors[clr%6]);

	bullet_list = bullet_list->next;
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
	if(get_bullet_list_size() < NUM_BULLETS) {
	    add_to_bullet_list(ship.row, ship.col + ship.width/2, FALSE);
	}
    }

    if(BUTTON_PRESSED(BUTTON_START)) {
	state = PAUSE;
    }

    if(BUTTON_PRESSED(BUTTON_SELECT)) {
	init();
	fillBackground(BGCOLOR);
	score = 0;
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
    moveBullets();

}

void checkCollisions() {

    /**** THIS SECTION DETECTS ANY COLLISIONS BETWEEN MOVING OBJECTS ****/
    struct enemy_llist* enemy_list = get_enemy_head();

    while(enemy_list != NULL) {

	struct bullet_llist* bullet_list = get_bullet_head();

	while(bullet_list != NULL) {

	    if(collision(enemy_list->val, bullet_list->val)) {
		drawRect(enemy_list->old_val->row, enemy_list->old_val->col, enemy_list->old_val->height, enemy_list->old_val->width, BGCOLOR);
		drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);

		delete_from_enemy_list(enemy_list->val);
		delete_from_bullet_list(bullet_list->val);

		score++;
		score_change = 1;
	    }

	    bullet_list = bullet_list->next;
	}

	enemy_list = enemy_list->next;
    }
}

void eraseOldObjects() {

    if(bullets_to_be_erased != get_bullet_list_size()) {
	drawRect(6,0,11,240,BGCOLOR);
	bullets_to_be_erased = get_bullet_list_size();
    }

    //Clears the screen if the game was just unpaused.
    if(state_old == PAUSE) {
	fillBackground(BGCOLOR);
	state_old = GAME;
    }

    /**** THIS SECTION ERASES OLD STUFF ****/
    drawEnemies(TRUE);
    drawBullets(TRUE);
    drawShip(&ship_old, TRUE);
}

void drawNewObjects() {
    /**** THIS SECTION DRAWS THE NEW LOCATIONS OF EVERYTHING ****/
    drawShip(&ship, FALSE);
    drawBullets(FALSE);
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

    struct enemy_llist* enemy_list = get_enemy_head();

    while(enemy_list != NULL) {
	updateOldEnemy(enemy_list);
	enemy_list = enemy_list->next;
    }

    struct bullet_llist* bullet_list = get_bullet_head();

    while(bullet_list != NULL) {
	updateOldBullet(bullet_list);
	bullet_list = bullet_list->next;
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
    }
}

/** init **************************************************
 * Initializes all of our objects with new variable locaitons
 *********************************************************/
void init() {

    empty_enemy_list();
    create_enemy_list();

    empty_bullet_list();

    ship.row = 120;
    ship.col = 80;
    ship.height = 20;
    ship.width = 20;

    ship_old.row = ship.row;
    ship_old.col = ship.col;
    ship_old.height = ship.height;
    ship_old.width = ship.width;

}
