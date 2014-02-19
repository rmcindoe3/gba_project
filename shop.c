#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "shop.h"
#include "myLib.h"
#include "main.h"
#include "text.h"

unsigned short money = 0;

//Keeps track of where the cursor is in the shop.
char shop_cursor_pos = 0;

/** upgradeWeapon ********************************************
 * Upgrades the current weapon to the next level!
 *************************************************************/
void upgradeWeapon() {

    if(ship.weapon_level == 1) ship.weapon_level = 2;
    else if(ship.weapon_level == 2) ship.weapon_level = 5;
    else if(ship.weapon_level == 5) ship.weapon_level = 6;
    else if(ship.weapon_level == 6) ship.weapon_level = 10;
    else if(ship.weapon_level == 10) ship.weapon_level = 11;
    else if(ship.weapon_level == 11) ship.weapon_level = 20;
}

/** determineWeaponCost **************************************
 * Determines and returns the cost of the next weapon upgrade
 *************************************************************/
int determineWeaponCost() {

    //Determine the upgrade cost of the next weapon.
    int weaponCost = 200;
    if(ship.weapon_level == 2) weaponCost = 500;
    else if(ship.weapon_level == 5) weaponCost = 1000;
    else if(ship.weapon_level == 6) weaponCost = 2000;
    else if(ship.weapon_level == 10) weaponCost = 5000;
    else if(ship.weapon_level == 11) weaponCost = 9999;

    return weaponCost;
}

/** createWeaponUpgradeString **********************************
 * Creates the appropriate weapon upgrade string for current
 *  purchase history.
 *************************************************************/
void createWeaponUpgradeString(char* str) {

    //Figure out what the cost of the next weapon upgrade is.
    int weaponCost = determineWeaponCost();

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

/** purchaseItem **********************************************
 * The user is trying to purchase the item at the given cursor
 *  position.
 *************************************************************/
void purchaseItem(char cursor_pos) {

    //Erases any previous purchase messages from the screen
    drawRect(65,0,8,240,BLACK);

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
    int weaponCost = determineWeaponCost();

    //If the user has enough money to purchase the weapon...
    if(money >= weaponCost) {

        //Upgrade the ship's weapon based on how much the user paid.
        upgradeWeapon();

        //Display a confirmation message to the user.
        drawString(65, 120-3*strlen("WEAPON UPGRADED!"), "WEAPON UPGRADED!", GREEN);

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
        drawString(65, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
    }

}

/** purchaseHealth ********************************************
 * Attempts to purchase health for the ship.
 *************************************************************/
void purchaseHealth() {

    //If the user is already at maximum health, don't let them
    //  purchase more and inform them why.
    if(ship.health == MAX_HEALTH) {
        drawString(65, 120-3*strlen("SORRY, ALREADY AT MAX HEALTH"), "SORRY, ALREADY AT MAX HEALTH", RED);
    }
    else {

        //If the user has enough money to buy health.
        if(money >= 50) {

            //Let them know
            drawString(65, 120-3*strlen("HEALTH ADDED!"), "HEALTH ADDED!", GREEN);

            //Increase their health and decrease their money.
            ship.health++;
            money -= 50;
        }
        //If the user doesn't have enough money, let them know.
        else {
            drawString(65, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
        }
    }
}
