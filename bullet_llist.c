#include "bullet_llist.h"
#include "myLib.h"
#include "pictures.h"
#include "main.h"

struct bullet_llist *bullet_head = NULL;
struct bullet_llist *bullet_curr = NULL;
int bullet_listSize = 0;

struct bullet_llist *e_bullet_head = NULL;
struct bullet_llist *e_bullet_curr = NULL;
int e_bullet_listSize = 0;

struct bullet_llist* create_bullet_list(int row, int col, signed short horz_velocity)
{
    struct bullet_llist *ptr = (struct bullet_llist*)malloc(sizeof(struct bullet_llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->old_val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->next = NULL;

    initBullet(ptr->val, row, col, horz_velocity);
    updateOldBullet(ptr);

    bullet_listSize++;
    bullet_head = ptr;
    bullet_curr = ptr;
    return ptr;
}

void empty_bullet_list(void) {

    if(NULL == bullet_head) return;

    struct bullet_llist* ptr = bullet_head;
    while(ptr->next != NULL)
    {
	delete_from_bullet_list(ptr->next->val);
    }
    delete_from_bullet_list(ptr->val);
    return;
}

int get_bullet_list_size(void) {
    return bullet_listSize;
}

BULLET* get_bullet(int index) {
    if(index < 0 || index >= bullet_listSize) {
	return NULL;
    }
    int i;
    struct bullet_llist* ptr = bullet_head;
    for(i = 0; i < index; i++) {
	ptr = ptr->next;
    }
    return ptr->val;
}

struct bullet_llist* get_bullet_head(void) {
    return bullet_head;
}

struct bullet_llist* get_bullet_tail(void) {
    return bullet_curr;
}

struct bullet_llist* add_to_bullet_list(int row, int col, signed short horz_velocity, int add_to_end)
{
    if(NULL == bullet_head)
    {
	return (create_bullet_list(row, col, horz_velocity));
    }

    struct bullet_llist *ptr = (struct bullet_llist*)malloc(sizeof(struct bullet_llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->old_val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->next = NULL;

    initBullet(ptr->val, row, col, horz_velocity);
    updateOldBullet(ptr);

    if(add_to_end)
    {
	bullet_curr->next = ptr;
	bullet_curr = ptr;
    }
    else
    {
	ptr->next = bullet_head;
	bullet_head = ptr;
    }
    bullet_listSize++;
    return ptr;
}

struct bullet_llist* search_in_bullet_list(BULLET* val, struct bullet_llist **prev)
{
    struct bullet_llist *ptr = bullet_head;
    struct bullet_llist *tmp = NULL;
    bool found = false;


    while(ptr != NULL)
    {
	if(ptr->val == val)
	{
	    found = true;
	    break;
	}
	else
	{
	    tmp = ptr;
	    ptr = ptr->next;
	}
    }

    if(true == found)
    {
	if(prev)
	    *prev = tmp;
	return ptr;
    }
    else
    {
	return NULL;
    }
}

int delete_from_bullet_list(BULLET* val)
{
    struct bullet_llist *prev = NULL;
    struct bullet_llist *del = NULL;

    del = search_in_bullet_list(val,&prev);
    if(del == NULL)
    {
	return -1;
    }
    else
    {
	if(prev != NULL)
	    prev->next = del->next;

	if(del == bullet_curr)
	{
	    bullet_curr = prev;
	}
	if(del == bullet_head)
	{
	    bullet_head = del->next;
	}
    }

    free(del->val);
    free(del->old_val);
    free(del);
    del = NULL;
    bullet_listSize--;

    return 0;
}

/**** FROM NOW ON ITS ENEMY BULLET LIST INFO ****/

struct bullet_llist* e_create_bullet_list(int row, int col, signed short horz_velocity)
{
    struct bullet_llist *ptr = (struct bullet_llist*)malloc(sizeof(struct bullet_llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->old_val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->next = NULL;

    e_initBullet(ptr->val, row, col, horz_velocity);
    updateOldBullet(ptr);

    e_bullet_listSize++;
    e_bullet_head = ptr;
    e_bullet_curr = ptr;
    return ptr;
}

void e_empty_bullet_list(void) {

    if(NULL == e_bullet_head) return;

    struct bullet_llist* ptr = e_bullet_head;
    while(ptr->next != NULL)
    {
	e_delete_from_bullet_list(ptr->next->val);
    }
    e_delete_from_bullet_list(ptr->val);
    return;
}

int e_get_bullet_list_size(void) {
    return e_bullet_listSize;
}

BULLET* e_get_bullet(int index) {
    if(index < 0 || index >= e_bullet_listSize) {
	return NULL;
    }
    int i;
    struct bullet_llist* ptr = e_bullet_head;
    for(i = 0; i < index; i++) {
	ptr = ptr->next;
    }
    return ptr->val;
}

struct bullet_llist* e_get_bullet_head(void) {
    return e_bullet_head;
}

struct bullet_llist* e_get_bullet_tail(void) {
    return e_bullet_curr;
}

struct bullet_llist* e_add_to_bullet_list(int row, int col, signed short horz_velocity, int add_to_end)
{
    if(NULL == e_bullet_head)
    {
	return (e_create_bullet_list(row, col, horz_velocity));
    }

    struct bullet_llist *ptr = (struct bullet_llist*)malloc(sizeof(struct bullet_llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->old_val = (BULLET*)malloc(sizeof(BULLET)); 
    ptr->next = NULL;

    e_initBullet(ptr->val, row, col, horz_velocity);
    updateOldBullet(ptr);

    if(add_to_end)
    {
	e_bullet_curr->next = ptr;
	e_bullet_curr = ptr;
    }
    else
    {
	ptr->next = e_bullet_head;
	e_bullet_head = ptr;
    }
    e_bullet_listSize++;
    return ptr;
}

struct bullet_llist* e_search_in_bullet_list(BULLET* val, struct bullet_llist **prev)
{
    struct bullet_llist *ptr = e_bullet_head;
    struct bullet_llist *tmp = NULL;
    bool found = false;


    while(ptr != NULL)
    {
	if(ptr->val == val)
	{
	    found = true;
	    break;
	}
	else
	{
	    tmp = ptr;
	    ptr = ptr->next;
	}
    }

    if(true == found)
    {
	if(prev)
	    *prev = tmp;
	return ptr;
    }
    else
    {
	return NULL;
    }
}

int e_delete_from_bullet_list(BULLET* val)
{
    struct bullet_llist *prev = NULL;
    struct bullet_llist *del = NULL;

    del = e_search_in_bullet_list(val,&prev);
    if(del == NULL)
    {
	return -1;
    }
    else
    {
	if(prev != NULL)
	    prev->next = del->next;

	if(del == e_bullet_curr)
	{
	    e_bullet_curr = prev;
	}
	if(del == e_bullet_head)
	{
	    e_bullet_head = del->next;
	}
    }

    free(del->val);
    free(del->old_val);
    free(del);
    del = NULL;
    e_bullet_listSize--;

    return 0;
}

/** moveBullets *******************************************
 *
 *********************************************************/
void moveBullets() {

    struct bullet_llist* temp = get_bullet_head();
    struct bullet_llist* del = NULL;

    while(temp != NULL) {

        //Move the bullet vertically based on vert_velocity
	temp->val->row += temp->val->vert_velocity;

        if(temp->val->horz_velocity != 0) {

            temp->val->horz_step--;

            //If it's time to move horizontally, then move horizontally.
            if(temp->val->horz_step == 0) {

                if(temp->val->horz_velocity > 0) {
                    temp->val->col++;
                }
                else {
                    temp->val->col--;
                }

                //We want to reset horz_step now.
                if(temp->val->horz_velocity < 0) {
                    temp->val->horz_step = -1*temp->val->horz_velocity;
                }
                else {
                    temp->val->horz_step = temp->val->horz_velocity;
                }
            }
        }

	if(temp->val->row <= 3 || temp->val->col >= 237 || temp->val->col <= 1) {
	    del = temp; 
	} else {
	    del = NULL;
	}

	temp = temp->next;

	if(del != NULL) {
            drawRect(del->old_val->row, del->old_val->col, del->old_val->height, del->old_val->width, BGCOLOR);
	    delete_from_bullet_list(del->val);
	}

    }

    temp = e_get_bullet_head();
    del = NULL;
    while(temp != NULL) {

        //Move the bullet vertically based on vert_velocity
	temp->val->row += temp->val->vert_velocity;

        if(temp->val->horz_velocity != 0) {

            temp->val->horz_step--;

            //If it's time to move horizontally, then move horizontally.
            if(temp->val->horz_step == 0) {

                //If it's a homing bullet...
                if(temp->val->horz_velocity == HOMING) {

                    //Determines which way the bullet needs to turn.
                    signed int dist = ship.col + ship.width/2 - temp->val->col + 1;

                    //Move the bullet once towards the ship.
                    //  If the bullet is past row 80, move it an extra step towards the ship.
                    if(dist > 0) {
                        temp->val->col++;
                        if(temp->val->row >= 80) temp->val->col++;
                    }
                    else {
                        temp->val->col--;
                        if(temp->val->row >= 80) temp->val->col--;
                    }
                }
                //If the bullet isn't homing, then just move it according to horz_velocity
                else {
                    if(temp->val->horz_velocity > 0) {
                        temp->val->col++;
                    }
                    else {
                        temp->val->col--;
                    }
                }

                //We want to reset horz_step now.
                //If the bullet is a homing bullet, set step rate equal to 3 automatically.
                if(temp->val->horz_velocity == HOMING) {
                    temp->val->horz_step = 1;
                }
                //If it's not a homing bullet...
                else {
                    //We want the step rate to be positive always
                    if(temp->val->horz_velocity < 0) {
                        temp->val->horz_step = -1*temp->val->horz_velocity;
                    }
                    else {
                        temp->val->horz_step = temp->val->horz_velocity;
                    }
                }
            }
        }

	if(temp->val->row >= 152 || temp->val->col >= 237 || temp->val->col <= 1) {
	    del = temp; 
	} else {
	    del = NULL;
	}

	temp = temp->next;

	if(del != NULL) {
            drawRect(del->old_val->row, del->old_val->col, del->old_val->height, del->old_val->width, BGCOLOR);
	    e_delete_from_bullet_list(del->val);
	}

    }
}

void initBullet(BULLET* bullet, int row, int col, signed short horz_velocity) {
    bullet->col = col;
    bullet->row = row;
    bullet->height = 5;
    bullet->width = 3;
    bullet->vert_velocity = -3;
    bullet->horz_velocity = horz_velocity;

    //We want the step rate to be positive always
    if(horz_velocity < 0) {
        bullet->horz_step = -1*horz_velocity;
    }
    else {
        bullet->horz_step = horz_velocity;
    }
}

void e_initBullet(BULLET* bullet, int row, int col, signed short horz_velocity) {
    bullet->col = col;
    bullet->row = row;
    bullet->height = 5;
    bullet->width = 3;
    bullet->vert_velocity = 3;
    bullet->horz_velocity = horz_velocity;

    //If the bullet is a homing bullet, set step rate equal to 3 automatically.
    if(horz_velocity == HOMING) {
        bullet->horz_step = 1;
    }
    //If it's not a homing bullet...
    else {
        //We want the step rate to be positive always
        if(horz_velocity < 0) {
            bullet->horz_step = -1*horz_velocity;
        }
        else {
            bullet->horz_step = horz_velocity;
        }
    }
}

void updateOldBullet(struct bullet_llist* node) {
    node->old_val->row = node->val->row;
    node->old_val->col = node->val->col;
    node->old_val->height = node->val->height;
    node->old_val->width = node->val->width;
    node->old_val->vert_velocity = node->val->vert_velocity;
    node->old_val->horz_velocity = node->val->horz_velocity;
    node->old_val->horz_step = node->val->horz_step;
}
