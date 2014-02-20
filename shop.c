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

UPGRADE weapon_damage_path[MAX_DMG];
UPGRADE fire_rate_path[MAX_FIRE_RATE];
UPGRADE shot_count_path[MAX_SHOT_COUNT];

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
    fire_rate_path[0].upgrade_value = 6;

    fire_rate_path[1].upgrade_cost = 2000;
    fire_rate_path[1].upgrade_value = 1;

    shot_count_path[0].upgrade_cost = 2000;
    shot_count_path[0].upgrade_value = 2;

    shot_count_path[1].upgrade_cost = 9999;
    shot_count_path[1].upgrade_value = 3;
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

/** createShotCountUpgradeString ******************************
 *
 *************************************************************/
void createShotCountUpgradeString(char* str) {

    //If the weapon is not already at max damage
    if(shot_count_index != MAX_SHOT_COUNT) {

        //Figure out what the cost of the next weapon upgrade is.
        int cost = shot_count_path[shot_count_index].upgrade_cost;

        //Place initial string into array.
        sprintf(str, "SHOT COUNT.........%06d", cost);

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
        sprintf(str, "SHOT COUNT..........MAXED");
    }
}

/** createFireRateUpgradeString *******************************
 *
 *************************************************************/
void createFireRateUpgradeString(char* str) {

    //If the weapon is not already at max damage
    if(fire_rate_index != MAX_FIRE_RATE) {

        //Figure out what the cost of the next weapon upgrade is.
        int cost = fire_rate_path[fire_rate_index].upgrade_cost;

        //Place initial string into array.
        sprintf(str, "FIRE RATE..........%06d", cost);

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
        sprintf(str, "FIRE RATE...........MAXED");
    }
}

/** purchaseItem **********************************************
 * The user is trying to purchase the item at the given cursor
 *  position.
 *************************************************************/
void purchaseItem() {

    //Erases any previous purchase messages from the screen
    drawRect(SHOP_MESSAGE_ROW,0,8,240,BLACK);

    //If the cursor is hovering over health...
    if(shop_cursor_pos == 0) {
        purchaseHealth();
    }
    //If the cursor is trying to upgrade weapons...
    else if(shop_cursor_pos == 1) {
        if(weapon_damage_index != MAX_DMG) {
            purchaseWeaponDamageUpgrade();
        }
    }
    //If the cursor is trying to upgrade shot count...
    else if(shop_cursor_pos == 2) {
        if(shot_count_index != MAX_SHOT_COUNT) {
            purchaseShotCountUpgrade();
        }
    }
    //If the cursor is trying to upgrade fire rate...
    else if(shop_cursor_pos == 3) {
        if(fire_rate_index != MAX_FIRE_RATE) {
            purchaseFireRateUpgrade();
        }
    }
}

/** purchaseWeaponDamageUpgrade *******************************
 * Attempts to purchase a weapon upgrade for the ship.
 *************************************************************/
void purchaseWeaponDamageUpgrade() {

    //Determine the upgrade cost of the next weapon.
    int cost = weapon_damage_path[weapon_damage_index].upgrade_cost;

    //If the user has enough money to purchase the weapon...
    if(money >= cost) {

        //Upgrade the ship's weapon based on how much the user paid.
        ship.weapon_damage = weapon_damage_path[weapon_damage_index].upgrade_value;

        //Display a confirmation message to the user.
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("WEAPON UPGRADED!"), "WEAPON UPGRADED!", GREEN);

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
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
    }

}

/** purchaseShotCountUpgrade ***********************************
 * 
 *************************************************************/
void purchaseShotCountUpgrade() {

    //Determine the upgrade cost of the next weapon.
    int cost = shot_count_path[shot_count_index].upgrade_cost;

    //If the user has enough money to purchase the weapon...
    if(money >= cost) {

        //Upgrade the ship's weapon based on how much the user paid.
        ship.shot_count = shot_count_path[shot_count_index].upgrade_value;

        //Display a confirmation message to the user.
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("SHOT COUNT UPGRADED!"), "SHOT COUNT UPGRADED!", GREEN);

        //Take their money.
        money -= cost;

        //Advance the weapon damage path index.
        shot_count_index++;

        //Update the price of the next weapon upgrade in the shop.
        drawRect(50,0,10,240,BLACK);
        char tempStr[26];
        createShotCountUpgradeString(tempStr);
        drawString(50, 120-3*strlen(tempStr), tempStr, BLUE);
    }
    //If they don't have enough moeny, let them know.
    else {
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
    }
}

/** purchaseFireRateUpgrade ************************************
 *
 *************************************************************/
void purchaseFireRateUpgrade() {

    //Determine the upgrade cost of the next weapon.
    int cost = fire_rate_path[fire_rate_index].upgrade_cost;

    //If the user has enough money to purchase the weapon...
    if(money >= cost) {

        //Upgrade the ship's weapon based on how much the user paid.
        ship.fire_rate = fire_rate_path[fire_rate_index].upgrade_value;

        //Display a confirmation message to the user.
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("FIRE RATE UPGRADED!"), "FIRE RATE UPGRADED!", GREEN);

        //Take their money.
        money -= cost;

        //Advance the fire rate path index.
        fire_rate_index++;

        //Update the price of the next fire rate upgrade in the shop.
        drawRect(60,0,10,240,BLACK);
        char tempStr[26];
        createFireRateUpgradeString(tempStr);
        drawString(60, 120-3*strlen(tempStr), tempStr, BLUE);
    }
    //If they don't have enough moeny, let them know.
    else {
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
    }
}

/** purchaseHealth ********************************************
 * Attempts to purchase health for the ship.
 *************************************************************/
void purchaseHealth() {

    //If the user is already at maximum health, don't let them
    //  purchase more and inform them why.
    if(ship.health == MAX_HEALTH) {
        drawString(SHOP_MESSAGE_ROW, 120-3*strlen("SORRY, ALREADY AT MAX HEALTH"), "SORRY, ALREADY AT MAX HEALTH", RED);
    }
    else {

        //If the user has enough money to buy health.
        if(money >= 50) {

            //Let them know
            drawString(SHOP_MESSAGE_ROW, 120-3*strlen("HEALTH ADDED!"), "HEALTH ADDED!", GREEN);

            //Increase their health and decrease their money.
            ship.health++;
            money -= 50;
        }
        //If the user doesn't have enough money, let them know.
        else {
            drawString(SHOP_MESSAGE_ROW, 120-3*strlen("SORRY, NOT ENOUGH MONEY!"), "SORRY, NOT ENOUGH MONEY!", RED);
        }
    }
}
