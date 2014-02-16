#include "bullet_llist.h"

struct bullet_llist *bullet_head = NULL;
struct bullet_llist *bullet_curr = NULL;
int bullet_listSize = 0;

struct bullet_llist *e_bullet_head = NULL;
struct bullet_llist *e_bullet_curr = NULL;
int e_bullet_listSize = 0;

struct bullet_llist* create_bullet_list(int row, int col)
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

    initBullet(ptr->val, row, col);
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

struct bullet_llist* add_to_bullet_list(int row, int col, int add_to_end)
{
    if(NULL == bullet_head)
    {
	return (create_bullet_list(row, col));
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

    initBullet(ptr->val, row, col);
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

struct bullet_llist* e_create_bullet_list(int row, int col)
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

    e_initBullet(ptr->val, row, col);
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

struct bullet_llist* e_add_to_bullet_list(int row, int col, int add_to_end)
{
    if(NULL == e_bullet_head)
    {
	return (e_create_bullet_list(row, col));
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

    e_initBullet(ptr->val, row, col);
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

/** moveBullet *******************************************
 * Takes in our array of bullet OBJECTs and moves all of
 *  them. 
 *********************************************************/
void moveBullets() {

    struct bullet_llist* temp = get_bullet_head();
    struct bullet_llist* del = NULL;

    while(temp != NULL) {

	//Move them according to velocity
	temp->val->row += temp->val->velocity;

	if(temp->val->row <= 3) {
	    del = temp; 
	} else {
	    del = NULL;
	}

	temp = temp->next;

	if(del != NULL) {
	    delete_from_bullet_list(del->val);
	}

    }

    temp = e_get_bullet_head();
    del = NULL;
    while(temp != NULL) {

	//Move them according to velocity
	temp->val->row += temp->val->velocity;

	if(temp->val->row >= 152) {
	    del = temp; 
	} else {
	    del = NULL;
	}

	temp = temp->next;

	if(del != NULL) {
	    e_delete_from_bullet_list(del->val);
	}

    }
}

void initBullet(BULLET* bullet, int row, int col) {
    bullet->col = col;
    bullet->row = row;
    bullet->height = 5;
    bullet->width = 3;
    bullet->velocity = -3;
}

void e_initBullet(BULLET* bullet, int row, int col) {
    bullet->col = col;
    bullet->row = row;
    bullet->height = 5;
    bullet->width = 3;
    bullet->velocity = 3;
}

void updateOldBullet(struct bullet_llist* node) {
    node->old_val->row = node->val->row;
    node->old_val->col = node->val->col;
    node->old_val->height = node->val->height;
    node->old_val->width = node->val->width;
    node->old_val->velocity = node->val->velocity;
}
