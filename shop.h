#ifndef SHOP_H
#define	SHOP_H

#define MAX_DMG 5
#define MAX_FIRE_RATE 3
#define MAX_SHOT_COUNT 3

typedef struct wpn_upgrade {
    char upgrade_value;
    short upgrade_cost;
} UPGRADE;

//These are the upgrade paths for the different ship attributes.
extern UPGRADE weapon_damage_path[5];
extern UPGRADE fire_rate_path[3];
extern UPGRADE shot_count_path[3];

//Keeps track of how much money the user has.
extern unsigned short money;

//Keeps track of where the cursor is in the shop.
extern char shop_cursor_pos;

void createWeaponUpgradeString(char* str);
void purchaseItem(void);
void purchaseWeaponUpgrade(void);
void purchaseHealth(void);
void assignUpgradePaths(void);

#endif
