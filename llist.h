#ifndef LLIST_H_INCLUDED
#define LLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//Struct that contains relevant information for ENEMY
typedef struct enemy {
    char row;
    char col;
    char height;
    char width;
    char velocity;
    char delay;
} ENEMY;

struct llist
{
    ENEMY* val;
    struct llist *next;
};

extern struct llist *head;
extern struct llist *curr;
extern int listSize;

struct llist* create_list(ENEMY* val);
struct llist* add_to_list(ENEMY* val, int add_to_end);
struct llist* search_in_list(ENEMY* val, struct llist **prev);
struct llist* get_head(void);
ENEMY* get(int index);
void empty_list(void);
int delete_from_list(ENEMY* val);
int get_size(void);

#endif
