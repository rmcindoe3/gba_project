#ifndef SHOP_H
#define	SHOP_H

typedef struct wpn_dmg_upgrade {
    char weapon_damage;
    short upgrade_cost;
} WPN_DMG_UPGRADE;

//Keeps track of how much money the user has.
extern unsigned short money;

//Keeps track of where the cursor is in the shop.
extern char shop_cursor_pos;

void upgradeWeapon(void);
int determineWeaponCost(void);
void createWeaponUpgradeString(char* str);
void purchaseItem(char cursor_pos);
void purchaseWeaponUpgrade(void);
void purchaseHealth(void);

#endif
