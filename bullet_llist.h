#ifndef LLIST_H_INCLUDED
#define LLIST_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HOMING 100

//Struct that contains relevant information for BULLET
typedef struct bullet {
    char row;
    char col;
    char height;
    char width;
    signed short vert_velocity;
    signed short horz_velocity;
    char horz_step;
} BULLET;

struct bullet_llist
{
    BULLET* val;
    BULLET* old_val;
    struct bullet_llist *next;
};

extern struct bullet_llist *bullet_head;
extern struct bullet_llist *bullet_curr;
extern int bullet_listSize;

extern struct bullet_llist *e_bullet_head;
extern struct bullet_llist *e_bullet_curr;
extern int e_bullet_listSize;

struct bullet_llist* create_bullet_list(int row, int col, signed short horz_velocity);
struct bullet_llist* add_to_bullet_list(int row, int col, signed short horz_velocity, int add_to_end);
struct bullet_llist* search_in_bullet_list(BULLET* val, struct bullet_llist **prev);
struct bullet_llist* get_bullet_head(void);
struct bullet_llist* get_bullet_tail(void);
BULLET* get_bullet(int index);
void empty_bullet_list(void);
int delete_from_bullet_list(BULLET* val);
int get_bullet_list_size(void);

struct bullet_llist* e_create_bullet_list(int row, int col, signed short horz_velocity);
struct bullet_llist* e_add_to_bullet_list(int row, int col, signed short horz_velocity, int add_to_end);
struct bullet_llist* e_search_in_bullet_list(BULLET* val, struct bullet_llist **prev);
struct bullet_llist* e_get_bullet_head(void);
struct bullet_llist* e_get_bullet_tail(void);
BULLET* e_get_bullet(int index);
void e_empty_bullet_list(void);
int e_delete_from_bullet_list(BULLET* val);
int e_get_bullet_list_size(void);

void moveBullets(void);
void initBullet(BULLET* bullet, int row, int col, signed short horz_velocity);
void e_initBullet(BULLET* bullet, int row, int col, signed short horz_velocity);
void updateOldBullet(struct bullet_llist* node);

#endif
