#include <stdlib.h>
#include "myLib.h"
#include "text.h"
#include "enemy_llist.h"
#include "bullet_llist.h"
#include "main.h"
#include "levels.h"

//Strings that store text to be displayed on the screen
char healthStr[] = "Health:";
char scoreStr[] = "Score: 1234";

//Determines whether or not we display the level string on screen
char displayLevelString = 100;
char displayLevelComplete = 0;

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
	    pause(1);

	} //If the game is in the SHOP state
	else if(state == SHOP) {

            waitForVblank();
            displayShopScreen();

	    //Check for user input
	    checkShopButtons();

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

    //Gets a pointer to the head of our ship's bullets list
    struct bullet_llist* bullet_list = get_bullet_head();

    unsigned short clr = determineBulletColor();

    while(bullet_list != NULL) {

	//If we're erasing, erase the old location of the bullet
	//  otherwise, draw the new location of the bullet
	if(erase) drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
	else drawRect(bullet_list->val->row, bullet_list->val->col, bullet_list->val->height, bullet_list->val->width, clr);

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

/** determineBulletColor **************************************
 * Determines the color of the bullet to be shot, which is based
 *  on the current weapon level of the ship.
 *************************************************************/
unsigned short determineBulletColor() {

    if(ship.weapon_level == 1) return YELLOW;
    if(ship.weapon_level == 2) return BLUE;
    if(ship.weapon_level == 5) return GREEN;
    if(ship.weapon_level == 10) return MAGENTA;
    if(ship.weapon_level == 20) return RED;
    if(ship.weapon_level == 50) return BLACK;
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

    //Determine which type of enemy we're drawing, and select the appropriate picture.
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
    else if(enemy->type == BIGG) {

        for(i = 0; i < enemy->height; i++) {
            //If the enemy has more than 10 health, use the full health picture source array
            if(enemy->health > 10) {
                DMANow(3, &(big_enemy_full_health_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
                        enemy->width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
            }
            //If the enemy has 10 or less health, use the half health picture source array
            else {
                DMANow(3, &(big_enemy_half_health_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
                        enemy->width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
            }
        }
    }
    else if(enemy->type == TRIS) {

        for(i = 0; i < enemy->height; i++) {
            //If the enemy has more than 20 health, use the full health picture source array
            if(enemy->health > 20) {
                DMANow(3, &(tri_enemy_full_health_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
                        enemy->width |  DMA_DESTINATION_INCREMENT | DMA_SOURCE_INCREMENT | DMA_ON);
            }
            //If the enemy has 20 or less health, use the half health picture source array
            else {
                DMANow(3, &(tri_enemy_half_health_picture[i*enemy->width]), &VIDEO_BUFFER[OFFSET(enemy->row + i, enemy->col, SCREENWIDTH)],
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

    //If the level hasn't just been completed, add enemies 
    if(!displayLevelComplete) {
        addEnemies();
    }

    //Move the enemies and the bullets on the screen
    moveEnemies();
    moveBullets();

}

/** enemiesFire ****************************************************
 * Will randomly shoot a bullet from a random enemy.
 ******************************************************************/
void enemiesFire() {

    struct enemy_llist* enemy_list = get_enemy_head();

    while(enemy_list != NULL) {

        //Set up a chance variable based on what type of enemy we have.
        //  (1/chance) is the probability that a bullet will be shot.
        char chance;
        if(enemy_list->val->type == NORM) {
            chance = 100;
        }
        else if(enemy_list->val->type == BIGG) {
            chance = 50;
        }
        else if(enemy_list->val->type == TRIS) {
            chance = 150;
        }
        else if(enemy_list->val->type == BOSS) {
            chance = 10;
        }

	//Randomly generate a number and test to see if we want to shoot a bullet.
	if((rand()%(chance)) == 0) {

	    //If the random chance happens, fire a bullet from the enemy.
	    e_add_to_bullet_list(enemy_list->val->row+10, enemy_list->val->col+10, 0, FALSE);

            //If we have a tri enemy type, then shoot two additional bullets with 2 and -2 horizontal velocity.
            if(enemy_list->val->type == TRIS) {
                e_add_to_bullet_list(enemy_list->val->row+10, enemy_list->val->col+10, 2, FALSE);
                e_add_to_bullet_list(enemy_list->val->row+10, enemy_list->val->col+10, -2, FALSE);
            }

	}

	enemy_list = enemy_list->next;
    }

}

/** checkCollisions ****************************************************
 * Checks to see if there are any collisions between enemies and our
 *  ship's bullets and between our ship and the enemies' bullets.
 ******************************************************************/
void checkCollisions() {

    //Gets a pointer to the head of the enemies' list
    struct enemy_llist* enemy_list = get_enemy_head();

    //Temporary variables used to keep track of enemy deletions.
    char del = 0;
    struct enemy_llist* del_enemy = NULL;

    while(enemy_list != NULL) {

	//Gets a pointer to the head of the bullets list
	struct bullet_llist* bullet_list = get_bullet_head();

        //del is set when an enemy is killed.  When that happens we don't
        //  want to keep iterating through bullets, so we break out of the loop.
	while(bullet_list != NULL && !del) {

	    //Checks to see if a collision happened between the bullet and enemy.
	    if(collision(enemy_list->val, bullet_list->val)) {

		//Erase and delete the bullet from our bullet list.
		drawRect(bullet_list->old_val->row, bullet_list->old_val->col, bullet_list->old_val->height, bullet_list->old_val->width, BGCOLOR);
		delete_from_bullet_list(bullet_list->val);

		//Decrement the enemies health.
                enemy_list->val->health -= ship.weapon_level;

		//  If the enemy has 0 health after this, erase and delete it
		if(enemy_list->val->health <= 0) {

                    //If the enemy is a boss, then move forward a level and
                    //  set the counter that displays the level string.
                    if(enemy_list->val->type == BOSS) {
                        displayLevelComplete = 100;
                    }

                    //Erase the enemy.
		    drawRect(enemy_list->old_val->row, enemy_list->old_val->col, enemy_list->old_val->height, enemy_list->old_val->width, BGCOLOR);

                    //Set delete variable so we know this enemy needs to be removed from the list.
                    del = TRUE;

		    //Also increment the score
		    score++;

                    //And give the user some money.
                    if(enemy_list->val->type == NORM) money += 10;
                    else if(enemy_list->val->type == BIGG) money += 20;
                    else if(enemy_list->val->type == TRIS) money += 40;
                    else if(enemy_list->val->type == BOSS) money += 40*(getCurrentLevel());
		}
	    }

	    bullet_list = bullet_list->next;
	}

        //If we want to delete this enemy because it died, set our temp variable
        if(del) {
            del_enemy = enemy_list;
        }

        //Move through the list
	enemy_list = enemy_list->next;
        
        //Now go back and delete our dead enemy from the list.
        if(del) {
            delete_from_enemy_list(del_enemy->val);
            del = FALSE;
        }

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
	    }
	}

	bullet_list =bullet_list->next;
    }
}

/** eraseOldObjects ****************************************************
 * Erases the previous locations of game objects from the screen.
 ******************************************************************/
void eraseOldObjects() {

    //Clears the screen if the game was just unpaused.
    if(state_old == PAUSE || state_old == SHOP) {
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

    //Variable used for flashing colors of the level strings.
    static unsigned int clr = 0;

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

    //displayLevelString is a counter for how many loop times we want
    //  to display what level the game is in.
    if(displayLevelString) {

        //Change which color we're going to use
        clr++;

        //Draw the string
        drawString(80, 120 - 3*strlen(getLevelString()), getLevelString(), bull_colors[clr%6]);

        //Decrement the count.
        displayLevelString--;

        //If the count just reached zero, erase the level string message.
        if(displayLevelString == 0) {
            drawRect(80,0,10,240,BGCOLOR);
        }
    }

    if(displayLevelComplete) {

        //Change which color we're going to use.
        clr++;

        //Draw the string
        drawString(80, 120 - 3*strlen("LEVEL COMPLETE!"), "LEVEL COMPLETE!", bull_colors[clr%6]);

        //Decrement the count.
        displayLevelComplete--;

        //If the count just reached zero, move to the shop.
        if(displayLevelComplete == 0) {
            state = SHOP;
        }
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
    }
}

/** displayShopScreen ****************************************************
 * Draws the shop screen to the display.
 ******************************************************************/
void displayShopScreen() {

    char shopStr[26];

    //If we just entered the shop, display the shop to the screen.
    if(state_old != SHOP) {
        fillBackground(BLACK);

        sprintf(shopStr, "SHOP!");
        drawString(10, 120-6*3, shopStr, BLUE);

        sprintf(shopStr, "HEALTH UPGRADE.........50");
        drawString(30, 120-3*strlen(shopStr), shopStr, BLUE);

        createWeaponUpgradeString(shopStr);
        drawString(40, 120-3*strlen(shopStr), shopStr, BLUE);

        sprintf(shopStr, "PRESS START TO CONTINUE!");
        drawString(100, 120-3*strlen(shopStr), shopStr, BLUE);

        state_old = SHOP;
    }
    
    //Always update how much money the user has.
    drawRect(142, 100, 10, 32, BLACK);
    sprintf(shopStr, "Current Money: %d", money);
    drawString(142, 10, shopStr, BLUE);

    //Update the cursor's location.
    drawRect(30,30,20,6,BLACK);
    drawCursor();

}

/** createWeaponUpgradeString **********************************
 * Creates the appropriate weapon upgrade string for current
 *  purchase history.
 *************************************************************/
void createWeaponUpgradeString(char* str) {

    //Figure out what the cost of the next weapon upgrade is.
    int weaponCost = 200;
    if(ship.weapon_level == 2) weaponCost = 500;
    else if(ship.weapon_level == 5) weaponCost = 1000;
    else if(ship.weapon_level == 10) weaponCost = 2000;
    else if(ship.weapon_level == 20) weaponCost = 5000;
    else if(ship.weapon_level == 50) weaponCost = 9999;

    //Place initial string into array.
    sprintf(str, "WEAPON UPGRADE.....%06d", weaponCost);

    //Blank out the leading zeros with periods.
    char* temp = str;
    while(*temp != '.') temp++;
    while(*temp == '.') temp++;
    while(*temp == '0') {
        *temp = '.';
        temp++;
    }
}

/** checkShopButtons ****************************************************
 * Checks for user input during the shop screen.
 ******************************************************************/
void checkShopButtons() {

    //If the user tries to purchase something from the shop.
    if(BUTTON_PRESSED(BUTTON_A)) {
        purchaseItem(shop_cursor_pos);
    }

    //If the user pressed up, move the cursor up
    if(BUTTON_PRESSED(BUTTON_UP)) {
        if(shop_cursor_pos != 0) {
            shop_cursor_pos--;
        }
    }

    //If the user pressed down, move the cursor down
    if(BUTTON_PRESSED(BUTTON_DOWN)) {
        if(shop_cursor_pos != 1) {
            shop_cursor_pos++;
        }
    }

    //If the start button is pressed, resume the game.
    if(BUTTON_PRESSED(BUTTON_START)) {
	state = GAME;
        advanceLevel();
        displayLevelString = 100;
        fillBackground(BGCOLOR);
    }

    //If the select button is pressed, reinitialize the game.
    if(BUTTON_PRESSED(BUTTON_SELECT)) {
	init();
        state = GAME;
    }

}

/** purchaseItem **********************************************
 * The user is trying to purchase the item at the given cursor
 *  position.
 *************************************************************/
void purchaseItem(char cursor_pos) {

    //Erases any previous purchase messages from the screen
    drawRect(80,0,8,240,BLACK);

    //If the cursor is hovering over health...
    if(cursor_pos == 0) {
        purchaseHealth();
    }
    //If the cursor is trying to upgrade weapons...
    else if(cursor_pos == 1) {
        purchaseWeaponUpgrade();
    }
}

/** purchaseWeaponUpgrade ************************************
 * Attempts to purchase a weapon upgrade for the ship.
 *************************************************************/
void purchaseWeaponUpgrade() {

    //Determine the upgrade cost of the next weapon.
    int weaponCost = 200;
    if(ship.weapon_level == 2) weaponCost = 500;
    else if(ship.weapon_level == 5) weaponCost = 1000;
    else if(ship.weapon_level == 10) weaponCost = 2000;
    else if(ship.weapon_level == 20) weaponCost = 5000;
    else if(ship.weapon_level == 50) weaponCost = 9999;

    //If the user has enough money to purchase the weapon...
    if(money >= weaponCost) {

        //Upgrade the ship's weapon based on how much the user paid.
        if(weaponCost == 200) ship.weapon_level = 2;
        else if(weaponCost == 500) ship.weapon_level = 5;
        else if(weaponCost == 1000) ship.weapon_level = 10;
        else if(weaponCost == 2000) ship.weapon_level = 20;
        else if(weaponCost == 5000) ship.weapon_level = 50;
        else if(weaponCost == 9999) ship.weapon_level = 100;

        //Display a confirmation message to the user.
        drawString(80, 120-3*strlen("WEAPON UPGRADED!"), "WEAPON UPGRADED!", GREEN);

        //Take their money.
        money -= weaponCost;

        //Update the price of the next weapon upgrade in the shop.
        drawRect(40,0,10,240,BLACK);
        char tempStr[26];
        createWeaponUpgradeString(tempStr);
        drawString(40, 120-3*strlen(tempStr), tempStr, BLUE);
    }
    //If they don't have enough moeny, let them know.
    else {
        drawString(80, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
    }

}

/** purchaseHealth ********************************************
 * Attempts to purchase health for the ship.
 *************************************************************/
void purchaseHealth() {

    //If the user is already at maximum health, don't let them
    //  purchase more and inform them why.
    if(ship.health == MAX_HEALTH) {
        drawString(80, 120-3*strlen("SORRY, ALREADY AT MAX HEALTH"), "SORRY, ALREADY AT MAX HEALTH", RED);
    }
    else {

        //If the user has enough money to buy health.
        if(money >= 50) {

            //Let them know
            drawString(80, 120-3*strlen("HEALTH ADDED!"), "HEALTH ADDED!", GREEN);

            //Increase their health and decrease their money.
            ship.health++;
            money -= 50;
        }
        //If the user doesn't have enough money, let them know.
        else {
            drawString(80, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
        }
    }
}

/** drawCursor *****************************************************
 * Draws the cursor on the shop screen.
 *************************************************************/
void drawCursor() {

    //static variable to keep track of frames to produce "blinking" effect 
    static char frame_counter = 0;

    frame_counter++;

    if(frame_counter < 10) {
        drawRect(30 + shop_cursor_pos*10, 30, 6, 6, BLUE);
    }
    else if(frame_counter >= 20) {
        frame_counter = 0;
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
    ship.weapon_level = 1;
    
    money = 0;
    score = 0;

    //Reinitializes old variables for the ship.
    ship_old.row = ship.row;
    ship_old.col = ship.col;
    ship_old.height = ship.height;
    ship_old.width = ship.width;

    displayLevelString = 100;
}
