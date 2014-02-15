#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "main.h"

unsigned int buttons;
unsigned int oldButtons;

int main() {
    int i = 0; //loop variable
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
void drawEnemies(ENEMY* enemies, int erase) {
    int i = 0;
    if(erase) {
	for(i = 0; i < NUM_ENEMIES; i++) {
	    if(enemies[i].velocity && !enemies[i].delay) {
		drawHorzLine(enemies[i].row, enemies[i].col, 20, BGCOLOR);
	    }
	}
    }
    else {
	for(i = 0; i < 10; i++) {
	    if(enemies[i].velocity && !enemies[i].delay) {
		drawEnemy(&(enemies[i]));
	    }
	}
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

/** moveShip *********************************************
 * Autopilot algorithm.  Takes in our ship OBJECT and the
 *  array of enemy OBJECTs.  Basically determines if any 
 *  columns neighboring the ship have a enemy that is at
 *  a higher height on the screen than the ship's current
 *  column.  If it does, then move the ship to that column
 *********************************************************/
void moveShip(SHIP* ship, ENEMY* enemies) {
    int curr_lane = ship->col / 20;

    //If a neighboring lane has a enemy with a higher location, switch to that lane
    if(curr_lane == 0) {
	if((&(enemies[curr_lane]))->row%140 > (&(enemies[curr_lane+1]))->row%140) {
	    ship->col += 20;
	}
    } else if(curr_lane == 11) {
	if((&(enemies[curr_lane]))->row%140 > (&(enemies[curr_lane-1]))->row%140) {
	    ship->col -= 20;
	}
    } else {
	if((&(enemies[curr_lane]))->row%140 > (&(enemies[curr_lane+1]))->row%140) {
	    ship->col += 20;
	} else if((&(enemies[curr_lane]))->row%140 > (&(enemies[curr_lane-1]))->row%140) {
	    ship->col -= 20;
	}
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
	    if(temp->row <= 10) {
		temp->velocity = 0;
		num_bullets--;
	    }
	}
    }
}

/** moveEnemies *******************************************
 * Takes in our array of enemy OBJECTs and moves all of
 *  them. 
 *********************************************************/
void moveEnemies(ENEMY* obj) {
    int i = 0;

    //If there are less than 8 enemies currently falling, then drop one...
    if(num_active_enemies < 8) {
	//This rand() is to make sure the enemies don't all fall one after each other
	if(!(rand()%20)) {
	    int test = 1;			//loop conditional - stores whether or not we've found a enemy that's not already falling
	    int ind = 0;			//stores the index of the enemy we're testing
	    while(test) {
		ind = rand()%12;							//randomly chose a lane to drop enemy in
		if(!((&(obj[ind]))->velocity)) {		//If the object isn't moving, start moving it and get out of the loop
		    (&(obj[ind]))->velocity = 1;
		    test = 0;
		}
	    }
	    num_active_enemies++;
	}
    }
    for(i = 0; i < NUM_ENEMIES; i++) {
	ENEMY* temp = (&(obj[i]));
	if(temp->velocity) {				//If the enemy has a velocity...
	    if(temp->row == 149) {		//If the enemy is one step away from the bottom we need to initialize the reset delay
		temp->delay = 50;
		temp->row += temp->velocity;
	    } else if(temp->row == 150) {	//If the enemy is already at the bottom
		if(temp->delay) {				//Wait the delay's length
		    temp->delay--;
		} else {							//Reset the enemy to the top
		    drawEnemies(temp, TRUE);
		    temp->row = 0;
		    temp->velocity = 0;
		    num_active_enemies--;
		}
	    } else temp->row += temp->velocity;		//For enemies not near the bottom...
	}
    }
}

/** collision ********************************************
 * Takes in two OBJECT's and sees if those two objects are
 *  overlapping.
 *********************************************************/
char collisionShip(SHIP* ship, ENEMY* enemy) {
    if((((enemy->col + enemy->width) > ship->col)) && (enemy->col < (ship->col + ship->width))) 
	if((((enemy->row + enemy->height) > ship->row)) && (enemy->row < (ship->row + ship->height)))
	    return 1;
    return 0;
}

char collisionBullet(ENEMY* ship, BULLET* spike) {
    if((((spike->col + spike->width) > ship->col)) && (spike->col < (ship->col + ship->width))) 
	if((((spike->row + spike->height) > ship->row)) && (spike->row < (ship->row + ship->height)))
	    return 1;
    return 0;
}

void checkGameButtons() {
    /**** THIS SECTION TOGGLES AUTO-PILOT MODE ****/
    if(BUTTON_PRESSED(BUTTON_B)) {
	autopilot ^= 1;
	auto_pilot_change = 1;
	ship.col -= ship.col%20;
    }

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
	init(enemies, enemies_old, bullets, bullets_old, &ship, &ship_old);
	fillBackground(BGCOLOR);
	score = 0;
	num_active_enemies = 0;
	num_bullets = 0;
    }

}

void moveGameObjects() {

    /**** THIS SECTION MOVES ALL THE PIECES  ****/
    if(!autopilot) {
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
    } else moveShip(&ship, enemies);

    moveEnemies(enemies);
    moveBullets(bullets);

}

void checkCollisions() {

    int i, j;
    /**** THIS SECTION DETECTS ANY COLLISIONS BETWEEN MOVING OBJECTS ****/
    for (i = 0; i < NUM_ENEMIES; i++) {
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
    }
}

void eraseOldObjects() {

    //Clears the screen if the game was just unpaused.
    if(state_old == PAUSE) {
	state_old = GAME;
	fillBackground(BGCOLOR);
    }

    /**** THIS SECTION ERASES OLD STUFF ****/
    drawEnemies(enemies_old, TRUE);
    drawBullets(bullets_old, TRUE);
    drawShip(&ship_old, TRUE);
}

void drawNewObjects() {
    /**** THIS SECTION DRAWS THE NEW LOCATIONS OF EVERYTHING ****/
    drawHorzLine(10, 0, 240, BLUE);
    drawShip(&ship, FALSE);
    drawBullets(bullets, FALSE);
    drawEnemies(enemies, FALSE);
}

void drawGameText() {
    /**** THIS SECTION DRAWS THE TEXT TO THE SCREEN ****/
    if(auto_pilot_change) {
	auto_pilot_change = 0;
	drawRect(150, 3+12*6, 8, 6*4, BGCOLOR);
    }

    if(autopilot)	drawString(150, 3, "Auto-Pilot: ON!", GREEN);
    else drawString(150, 3, "Auto-Pilot: OFF", BLUE);

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

    for (i = 0; i < NUM_ENEMIES; i++) {
	enemies_old[i].col = enemies[i].col; 
	enemies_old[i].row = enemies[i].row; 
	enemies_old[i].height = enemies[i].height;
	enemies_old[i].width = enemies[i].width;
	enemies_old[i].velocity = enemies[i].velocity;
	enemies_old[i].delay = enemies[i].delay;
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
	init(enemies, enemies_old, bullets, bullets_old, &ship, &ship_old);
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

    for (i = 0; i < NUM_ENEMIES; i++) {
	enemies[i].col = 20*i;
	enemies[i].row = 0;
	enemies[i].height = 10;
	enemies[i].width = 20;
	enemies[i].velocity = 0;
	enemies[i].delay = 0;

	enemies_old[i].col = enemies[i].col;
	enemies_old[i].row = enemies[i].row;
	enemies_old[i].height = enemies[i].height;
	enemies_old[i].width = enemies[i].width;
	enemies_old[i].velocity = enemies[i].velocity;
	enemies_old[i].delay = enemies[i].delay;
    }

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
