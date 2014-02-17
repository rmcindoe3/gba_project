#ifndef ENEMY_LLIST_H_INCLUDED
#define ENEMY_LLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NUM_ENEMIES 8

//Used for boolean expressions
#define TRUE 0x01
#define FALSE 0x00

//Velocity vars
#define LEFT -1
#define RIGHT 1

#define NORM 0x01
#define BOSS 0x02

signed short curr_velocity;

//Struct that contains relevant information for ENEMY
typedef struct enemy {
    char row;
    char col;
    char height;
    char width;
    signed short velocity;
    char delay;
    char health;
    char type;
} ENEMY;

struct enemy_llist
{
    ENEMY* val;
    ENEMY* old_val;
    struct enemy_llist *next;
};

extern struct enemy_llist *enemy_head;
extern struct enemy_llist *enemy_curr;
extern int enemy_listSize;

struct enemy_llist* create_enemy_list(char type);
struct enemy_llist* add_to_enemy_list(char type, int add_to_end);
struct enemy_llist* search_in_enemy_list(ENEMY* val, struct enemy_llist **prev);
struct enemy_llist* get_enemy_head(void);
struct enemy_llist* get_enemy_tail(void);
ENEMY* get_enemy(int index);
void empty_enemy_list(void);
int delete_from_enemy_list(ENEMY* val);
int get_enemy_list_size(void);
void moveEnemies(void);
void initEnemy(ENEMY* enemy, char type);
void updateOldEnemy(struct enemy_llist* node);
char determineEnemySpawn(char enemyType, int add_enemy_cnt);

#endif
