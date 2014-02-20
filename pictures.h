#ifndef PICTURES_H
#define	PICTURES_H

#include "myLib.h"

//defined colors that help when assigning pictures
#define RED COLOR(31, 0, 0)
#define GRE COLOR(0, 31, 0)
#define BLU COLOR(0, 0, 31)
#define MAG COLOR(31, 0, 31)
#define CYA COLOR(0, 31, 31)
#define YEL COLOR(31, 31, 0)
#define BLA 0
#define WHI COLOR(31, 31, 31)

//Byte arrays that represent our object's picture.
extern const unsigned short ship_picture[20*20];
extern const unsigned short boss_picture[40*20];
extern const unsigned short enemy_full_health_picture[20*10];
extern const unsigned short enemy_half_health_picture[20*10];
extern const unsigned short big_enemy_full_health_picture[20*10];
extern const unsigned short big_enemy_half_health_picture[20*10];
extern const unsigned short tri_enemy_full_health_picture[20*10];
extern const unsigned short tri_enemy_half_health_picture[20*10];
extern const unsigned short dbl_enemy_full_health_picture[20*10];
extern const unsigned short dbl_enemy_half_health_picture[20*10];
extern const unsigned short health_bar[8*8];
extern const unsigned short bull_colors[6];

#endif
