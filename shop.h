#ifndef SHOP_H
#define	SHOP_H

#define MAX_DMG 5
#define MAX_FIRE_RATE 3
#define MAX_SHOT_COUNT 2

#define SHOP_MESSAGE_ROW 75

typedef struct wpn_upgrade {
    char upgrade_value;
    short upgrade_cost;
} UPGRADE;

//These are the upgrade paths for the different ship attributes.
extern UPGRADE weapon_damage_path[MAX_DMG];
extern UPGRADE fire_rate_path[MAX_FIRE_RATE];
extern UPGRADE shot_count_path[MAX_SHOT_COUNT];

extern unsigned char weapon_damage_index;
extern unsigned char fire_rate_index;
extern unsigned char shot_count_index;

//Keeps track of how much money the user has.
extern unsigned short money;

//Keeps track of where the cursor is in the shop.
extern char shop_cursor_pos;


void assignUpgradePaths(void);

void purchaseItem(void);

void createWeaponUpgradeString(char* str);
void createShotCountUpgradeString(char* str);
void createFireRateUpgradeString(char* str);

void purchaseWeaponDamageUpgrade(void);
void purchaseShotCountUpgrade(void);
void purchaseFireRateUpgrade(void);

void purchaseHealth(void);

#endif
