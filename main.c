#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "enemy_llist.h"
#include "bullet_llist.h"
#include "main.h"

//Two variables that erase bullets that fly off the screen instead of 
//  hit something.
char bullets_to_be_erased = 0;

//Determines the rate at which enemies fire
char difficulty = 1;

//Strings that store text to be displayed on the screen
char healthStr[] = "Health:";
char scoreStr[] = "Score: 1234";

//Main function of program.
int main() {

    REG_DISPCTL = 1027; //sets up the display in mode 3 with the settings we want
    fillBackground(BGCOLOR); //clears the screen in case anything else is present

    //initializes the game pieces
    init();

    //Main loop
    while(1) {

	//Updates our button bit flag arrays
	oldButtons = buttons;
	buttons = BUTTONS;

	//If the game state is in GAME
	if(state == GAME) {

	    //Checks inputs, moves pieces, enemies fire bullets, and then
	    //	check for collisions between the pieces
	    checkGameButtons();
	    moveGameObjects();
	    enemiesFire();
	    checkCollisions();

	    //Waits for the gameboy display's vertical blank
	    waitForVblank();

	    //Erases old objects from the screen, then draws the new locations
	    eraseOldObjects();
	    drawNewObjects();

	    //Draws the text content to the screen
	    drawGameText();

	    //Updates our "old" variables to be erased next loop cycle
	    updateOldVariables();

	    //Pauses for a bit before moving to the next frame
	    pause(2);

	} //If the game is in the PAUSE state
	else if(state == PAUSE) {

	    //If the game just switched to pause, show the pause screen
	    if(state_old == GAME) {
		displayPauseScreen();
		state_old = PAUSE;
	    }

	    //Check for user input
	    checkPauseButtons();

	}
    }
}

/** drawBullets ****************************************************
 * Draws all current bullet's to the screen.
 *  - int erase - TRUE if you want to erase the bullets
 *		  FALSE if you want to draw the bullets
 ******************************************************************/
void drawBullets(int erase) {

    //Static variable for this function for determining the 
    //	color of a bullet to draw.  Indexes through bull_colors color array.
    static int clr = 0;

    //If we're drawing bullets, increment to the next bullet color
    if(!erase) clr++;

    //Gets a pointer to the head of our ship's bullets list
    struct bullet_llist* bullet_list = get_bullet_head();

    while(bullet_list != NULL) {

	//If we're erasing, erase the old location of the bullet
	//  otherwise, draw the new location of the bullet
	if(erase) drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
	else drawRect(bullet_list->val->row, bullet_list->val->col, bullet_list->val->height, bullet_list->val->width, bull_colors[clr%6]);

	bullet_list = bullet_list->next;
    }

    //Gets a pointer to the head of the enemies' bullets list
    bullet_list = e_get_bullet_head();

    while(bullet_list != NULL) {

	//If we're erasing, erase the old location of the bullet
	//  otherwise, draw the new location of the bullet
	if(erase) drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
	else drawRect(bullet_list->val->row, bullet_list->val->col, bullet_list->val->height, bullet_list->val->width, RED);

	bullet_list = bullet_list->next;
    }
}

/** drawEnemies ********************************************
 * Draws all current enemies to the screen.
 *  - int erase - TRUE if you want to erase the enemies
 *		  FALSE if you want to draw the enemies
 *********************************************************/
void drawEnemies(int erase) {

    //Gets a pointer to the head of the enemies' list
    struct enemy_llist* temp = get_enemy_head();

    while(temp != NULL) {

	//If we're erasing, erase the old location of the enemy
	//  otherwise, draw the new location of the enemy
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
 * Draws the ship to the screen.
 *  - SHIP* ship - pointer to our ship's object.
 *  - int erase - TRUE if you want to erase the ship
 *		  FALSE if you want to draw the ship
 *********************************************************/
void drawShip(SHIP* ship, int erase) {

    //If we're not erasing, draw the ship in it's current location 
    //  otherwise, erase at the ship's location
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
 * Draws a single enemy to the screen according to the
 * struct's variables
 *  - ENEMY* enemy - a pointer to the enemy we want to draw
 *********************************************************/
void drawEnemy(ENEMY* enemy) {
    int i = 0;
    if(enemy->type == NORM) {
        for(i = 0; i < enemy->height; i++) {
            //If the enemy has more than 5 health, use the full health picture source array
            if(enemy->health > 5) {
                DMANow(3, &(enemy_full_health_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
                        enemy->width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
            }
            //If the enemy has 5 or less health, use the half health picture source array
            else {
                DMANow(3, &(enemy_half_health_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
                        enemy->width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
            }
        }
    }
    else if(enemy->type == BOSS) {
        for(i = 0; i < enemy->height; i++) {
            DMANow(3, &(boss_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
                    enemy->width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
        }
    }
}

/** drawHealth ********************************************
 * Draws the health bar picture to the screen at (row,col)
 *  - int row - the row to draw the picture at
 *  - int col - the column to draw the picture at
 *********************************************************/
void drawHealth(int row, int col) {
    int i = 0;
    for(i = 0; i < 8; i++) {
	DMANow(3, &(health_bar[i*8]), &VIDEO_BUFFER[OFFSET(row + i, col, SCREENWIDTH)],
		8 |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
    }
}

/** collision ********************************************
 * Checks to see if the given enemy is hit by the given
 * bullet.
 *  - ENEMY* enemy - the enemy we're checking collision on
 *  - BULLET* bullet - the bullet we're checking collision on
 *********************************************************/
char collision(ENEMY* enemy, BULLET* bullet) {
    if((((bullet->col + bullet->width) > enemy->col)) && (bullet->col < (enemy->col + enemy->width))) 
	if((((bullet->row + bullet->height) > enemy->row)) && (bullet->row < (enemy->row + enemy->height)))
	    return 1;
    return 0;
}

/** shipCollision ****************************************************
 * Checks to see if the given bullet has hit our ship.
 *  - BULLET* bullet - the bullet we want to check for collision
 ******************************************************************/
char shipCollision(BULLET* bullet) {
    if((((bullet->col + bullet->width) > ship.col)) && (bullet->col < (ship.col + ship.width))) 
	if((((bullet->row + bullet->height) > ship.row)) && (bullet->row < (ship.row + ship.height)))
	    return 1;
    return 0;
}

/** checkGameButtons ****************************************************
 * Checks to see if the user has given any input that the game
 * needs to respond to in this loop cycle.
 ******************************************************************/
void checkGameButtons() {

    //If button A is pressed and we don't have too many bullets
    //	shot out already, then shoot a bullet
    if(BUTTON_PRESSED(BUTTON_A)) {
	if(get_bullet_list_size() < NUM_BULLETS) {
	    add_to_bullet_list(ship.row, ship.col + ship.width/2, FALSE);
	}
    }

    //If the start button is pressed, pause the game
    if(BUTTON_PRESSED(BUTTON_START)) {
	state = PAUSE;
    }

    //If the select button is pressed, reset the game
    if(BUTTON_PRESSED(BUTTON_SELECT)) {
	init();
	fillBackground(BGCOLOR);
	score = 0;
    }
}

/** moveGameObjects ****************************************************
 * Moves the game objects that are currently in the field of play.
 ******************************************************************/
void moveGameObjects() {

    //If the user presses left, then move the ship left.
    if(BUTTON_HELD(BUTTON_LEFT)) {
	if(ship.col) {
	    ship.col -= 5;
	}
    }
    //If the user pressed right, then move the ship right.
    if(BUTTON_HELD(BUTTON_RIGHT)) {
	if(ship.col < 220) {
	    ship.col += 5;
	}
    }

    //Adds enemies to the screen, if necessary.
    addEnemies();

    //Move the enemies and the bullets on the screen
    moveEnemies();
    moveBullets();

}

/** enemiesFire ****************************************************
 * Will randomly shoot a bullet from a random enemy.
 ******************************************************************/
void enemiesFire() {
    //If the enemies haven't reached their max bullet count, and there 
    //  are enemies on the screen right now
    if(e_get_bullet_list_size() < 1000 && get_enemy_list_size()) {
	
	//Randomly generate a number based on difficulty and number
	//  of enemies currently on the screen
	if(rand()%(15 - difficulty * get_enemy_list_size()) == 0) {

	    //If the random chance happens, choose a random enemy
	    //	and fire a bullet from it.
	    ENEMY* temp = get_enemy(rand()%get_enemy_list_size());
	    e_add_to_bullet_list(temp->row+10, temp->col+10, FALSE);
	}
    }
}

/** checkCollisions ****************************************************
 * Checks to see if there are any collisions between enemies and our
 *  ship's bullets and between our ship and the enemies' bullets.
 ******************************************************************/
void checkCollisions() {

    //Gets a pointer to the head of the enemies' list
    struct enemy_llist* enemy_list = get_enemy_head();

    while(enemy_list != NULL) {

	//Gets a pointer to the head of the bullets list
	struct bullet_llist* bullet_list = get_bullet_head();

	while(bullet_list != NULL) {

	    //Checks to see if a collision happened between these two objects
	    if(collision(enemy_list->val, bullet_list->val)) {

		//Erase and delete the bullet from our bullet list.
		drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
		delete_from_bullet_list(bullet_list->val);

		//Decrement the enemies health.
		//  If the enemy has 0 health after this, erase and delete it
		if(--(enemy_list->val->health) == 0) {
		    drawRect(enemy_list->old_val->row, enemy_list->old_val->col, enemy_list->old_val->height, enemy_list->old_val->width, BGCOLOR);
		    delete_from_enemy_list(enemy_list->val);

		    //Also increment the score
		    score++;
		}
	    }

	    bullet_list = bullet_list->next;
	}

	enemy_list = enemy_list->next;
    }

    //Gets a pointer to the head of the enemies' bullet list
    struct bullet_llist* bullet_list = e_get_bullet_head();

    while(bullet_list != NULL) {

	//If there is a collision between the ship and this bullet
	if(shipCollision(bullet_list->val)) {

	    //Erase and delete the bullet from our bullet list.
	    drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
	    e_delete_from_bullet_list(bullet_list->val);

	    //Decrement the ship's health.
	    ship.health--;

	    //If the ship is out of health, reinitialize the game.
	    if(!ship.health) {
		init();
		fillBackground(BGCOLOR);
		score = 0;
	    }
	}

	bullet_list =bullet_list->next;
    }
}

/** eraseOldObjects ****************************************************
 * Erases the previous locations of game objects from the screen.
 ******************************************************************/
void eraseOldObjects() {

    //If there has been bullets leave the game, make sure to erase them.
    if(bullets_to_be_erased != get_bullet_list_size()) {
	drawRect(6,0,11,240,BGCOLOR);
	bullets_to_be_erased = get_bullet_list_size();
    }

    //Clears the screen if the game was just unpaused.
    if(state_old == PAUSE) {
	fillBackground(BGCOLOR);
	state_old = GAME;
    }

    //Erases old positions of enemies, bullets, and ship.
    drawEnemies(TRUE);
    drawBullets(TRUE);
    drawShip(&ship_old, TRUE);
}

/** drawNewObjects ****************************************************
 * Draws the new locations of objects onto the screen.
 ******************************************************************/
void drawNewObjects() {

    //Draw the new positions of the ship, bullets, and enemies.
    drawShip(&ship, FALSE);
    drawBullets(FALSE);
    drawEnemies(FALSE);
}

/** drawGameText ****************************************************
 * Draws the game text that will be displayed on the screen
 ******************************************************************/
void drawGameText() {

    int i = 0;

    //Clears the bottom bar of the screen.
    drawRect(145, 0, 15, 240, BGCOLOR);

    //Print the score to a string array and then display it.
    sprintf(scoreStr, "Score: %04d", score);
    drawString(150, 160, scoreStr, BLUE);

    //Draw the health string to the screen.
    drawString(150, 10, healthStr, BLUE);

    //Draw a health bar picture per health the user has.
    for(i = 0; i < ship.health; i++) {
	drawHealth(150, 55 + 10*i);
    }
}


/** updateOldVariables ****************************************************
 * Updates all "old" variables that will need to be erased next loop cycle.
 ******************************************************************/
void updateOldVariables() {

    //Gets a pointer to the head of the enemies' list
    struct enemy_llist* enemy_list = get_enemy_head();

    while(enemy_list != NULL) {
	//Updates every enemy's old location.
	updateOldEnemy(enemy_list);
	enemy_list = enemy_list->next;
    }

    //Gets a pointer to the head of the bullets list
    struct bullet_llist* bullet_list = get_bullet_head();

    while(bullet_list != NULL) {
	//Updates every bullet's old location.
	updateOldBullet(bullet_list);
	bullet_list = bullet_list->next;
    }

    //Gets a pointer to the head of the enemies' bullets list
    bullet_list = e_get_bullet_head();

    while(bullet_list != NULL) {
	//Updates every bullet's old location.
	updateOldBullet(bullet_list);
	bullet_list = bullet_list->next;
    }

    //Updates the ship's old variable.
    ship_old.row = ship.row;
    ship_old.col = ship.col;
    ship_old.height = ship_old.height;
    ship_old.width = ship_old.width;
}

/** displayPauseScreen ****************************************************
 * Draws the pause screen to the display.
 ******************************************************************/
void displayPauseScreen() {
    fillBackground(BLACK);
    char pauseStr[] = "PAUSED";
    sprintf(scoreStr, "SCORE: %04d", score);
    drawString(86, 120-33, scoreStr, BLUE);
    drawString(76, 102, pauseStr, BLUE);
}

/** checkPauseButtons ****************************************************
 * Checks for user input during the pause screen.
 ******************************************************************/
void checkPauseButtons() {

    //If the start button is pressed, resume the game.
    if(BUTTON_PRESSED(BUTTON_START)) {
	state = GAME;
    }

    //If the select button is pressed, reinitialize the game.
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

    //Empties the enemy list.
    empty_enemy_list();

    //Resets the level of the game.
    reset_level();

    //Empties our bullet lists.
    empty_bullet_list();
    e_empty_bullet_list();

    //Relocates the ship to the starting location.
    ship.row = 120;
    ship.col = 80;
    ship.height = 20;
    ship.width = 20;
    ship.health = 5;

    //Reinitializes old variables for the ship.
    ship_old.row = ship.row;
    ship_old.col = ship.col;
    ship_old.height = ship.height;
    ship_old.width = ship.width;

}
