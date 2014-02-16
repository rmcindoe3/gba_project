#ifndef LLIST_H_INCLUDED
#define LLIST_H_INCLUDED

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

signed short curr_velocity;

//Struct that contains relevant information for ENEMY
typedef struct enemy {
    char row;
    char col;
    char height;
    char width;
    signed short velocity;
    char delay;
} ENEMY;

struct llist
{
    ENEMY* val;
    ENEMY* old_val;
    struct llist *next;
};

extern struct llist *head;
extern struct llist *curr;
extern int listSize;

struct llist* create_list();
struct llist* add_to_list(int add_to_end);
struct llist* search_in_list(ENEMY* val, struct llist **prev);
struct llist* get_head(void);
struct llist* get_tail(void);
ENEMY* get(int index);
void empty_list(void);
int delete_from_list(ENEMY* val);
int get_size(void);
void moveEnemies(void);
void initEnemy(ENEMY* enemy);
void updateOldEnemy(struct llist* node);

#endif
