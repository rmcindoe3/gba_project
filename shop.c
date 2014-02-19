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

UPGRADE weapon_damage_path[5];
UPGRADE fire_rate_path[3];
UPGRADE shot_count_path[3];

unsigned char weapon_damage_index = 0;
unsigned char fire_rate_index = 0;
unsigned char shot_count_index = 0;

/** assignUpgradePaths ****************************************
 * Fills in the values of the upgrade paths so we can upgrade
 * our ships weapons easily.
 *************************************************************/
void assignUpgradePaths() {

    weapon_damage_path[0].upgrade_cost = 200;
    weapon_damage_path[0].upgrade_value = 2;

    weapon_damage_path[1].upgrade_cost = 500;
    weapon_damage_path[1].upgrade_value = 4;

    weapon_damage_path[2].upgrade_cost = 1000;
    weapon_damage_path[2].upgrade_value = 6;

    weapon_damage_path[3].upgrade_cost = 5000;
    weapon_damage_path[3].upgrade_value = 10;

    weapon_damage_path[4].upgrade_cost = 9999;
    weapon_damage_path[4].upgrade_value = 15;

    fire_rate_path[0].upgrade_cost = 500;
    fire_rate_path[0].upgrade_value = 10;

    fire_rate_path[1].upgrade_cost = 2000;
    fire_rate_path[1].upgrade_value = 5;

    fire_rate_path[2].upgrade_cost = 5000;
    fire_rate_path[2].upgrade_value = 1;

    shot_count_path[0].upgrade_cost = 2000;
    shot_count_path[0].upgrade_value = 10;

    shot_count_path[1].upgrade_cost = 5000;
    shot_count_path[1].upgrade_value = 5;

    shot_count_path[2].upgrade_cost = 9999;
    shot_count_path[2].upgrade_value = 1;

}

/** createWeaponUpgradeString **********************************
 * Creates the appropriate weapon damage upgrade string for
 *  current purchase history.
 *************************************************************/
void createWeaponUpgradeString(char* str) {

    //If the weapon is not already at max damage
    if(weapon_damage_index != MAX_DMG) {

        //Figure out what the cost of the next weapon upgrade is.
        int weaponCost = weapon_damage_path[weapon_damage_index].upgrade_cost;

        //Place initial string into array.
        sprintf(str, "WEAPON DAMAGE......%06d", weaponCost);

        //Blank out the leading zeros with periods.
        char* temp = str;
        while(*temp != '.') temp++;
        while(*temp == '.') temp++;
        while(*temp == '0') {
            *temp = '.';
            temp++;
        }
    }
    else {
        sprintf(str, "WEAPON DAMAGE.......MAXED");
    }
}

/** purchaseItem **********************************************
 * The user is trying to purchase the item at the given cursor
 *  position.
 *************************************************************/
void purchaseItem() {

    //Erases any previous purchase messages from the screen
    drawRect(65,0,8,240,BLACK);

    //If the cursor is hovering over health...
    if(shop_cursor_pos == 0) {
        purchaseHealth();
    }
    //If the cursor is trying to upgrade weapons...
    else if(shop_cursor_pos == 1) {
        if(weapon_damage_index != MAX_DMG) {
            purchaseWeaponUpgrade();
        }
    }
}

/** purchaseWeaponUpgrade ************************************
 * Attempts to purchase a weapon upgrade for the ship.
 *************************************************************/
void purchaseWeaponUpgrade() {

    //Determine the upgrade cost of the next weapon.
    int cost = weapon_damage_path[weapon_damage_index].upgrade_cost;

    //If the user has enough money to purchase the weapon...
    if(money >= cost) {

        //Upgrade the ship's weapon based on how much the user paid.
        ship.weapon_damage = weapon_damage_path[weapon_damage_index].upgrade_value;

        //Display a confirmation message to the user.
        drawString(65, 120-3*strlen("WEAPON UPGRADED!"), "WEAPON UPGRADED!", GREEN);

        //Take their money.
        money -= cost;

        //Advance the weapon damage path index.
        weapon_damage_index++;

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
