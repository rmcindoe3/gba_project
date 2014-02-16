#include "llist.h"

struct llist *head = NULL;
struct llist *curr = NULL;
int listSize = 0;

struct llist* create_list()
{
    struct llist *ptr = (struct llist*)malloc(sizeof(struct llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->old_val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->next = NULL;

    initEnemy(ptr->val);
    updateOldEnemy(ptr);

    listSize++;
    head = ptr;
    curr = ptr;
    return ptr;
}

void empty_list(void) {

    if(NULL == head) return;

    struct llist* ptr = head;
    while(ptr->next != NULL)
    {
	delete_from_list(ptr->next->val);
    }
    delete_from_list(ptr->val);
    return;
}

int get_size(void) {
    return listSize;
}

ENEMY* get(int index) {
    if(index < 0 || index >= listSize) {
	return NULL;
    }
    int i;
    struct llist* ptr = head;
    for(i = 0; i < index; i++) {
	ptr = ptr->next;
    }
    return ptr->val;
}

struct llist* get_head(void) {
    return head;
}

struct llist* get_tail(void) {
    return curr;
}

struct llist* add_to_list(int add_to_end)
{
    if(NULL == head)
    {
	return (create_list());
    }

    struct llist *ptr = (struct llist*)malloc(sizeof(struct llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->old_val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->next = NULL;

    initEnemy(ptr->val);
    updateOldEnemy(ptr);

    if(add_to_end)
    {
	curr->next = ptr;
	curr = ptr;
    }
    else
    {
	ptr->next = head;
	head = ptr;
    }
    listSize++;
    return ptr;
}

struct llist* search_in_list(ENEMY* val, struct llist **prev)
{
    struct llist *ptr = head;
    struct llist *tmp = NULL;
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

int delete_from_list(ENEMY* val)
{
    struct llist *prev = NULL;
    struct llist *del = NULL;

    del = search_in_list(val,&prev);
    if(del == NULL)
    {
	return -1;
    }
    else
    {
	if(prev != NULL)
	    prev->next = del->next;

	if(del == curr)
	{
	    curr = prev;
	}
	if(del == head)
	{
	    head = del->next;
	}
    }

    free(del->val);
    free(del->old_val);
    free(del);
    del = NULL;
    listSize--;

    return 0;
}

/** moveEnemies *******************************************
 * Takes in our array of enemy OBJECTs and moves all of
 *  them. 
 *********************************************************/
void moveEnemies() {

    static int add_enemy_cnt = 0;
    //If there are less than 8 enemies currently falling, then drop one...
    if(get_size() < NUM_ENEMIES) {
	add_enemy_cnt++;
	//This rand() is to make sure the enemies don't all fall one after each other
	if(add_enemy_cnt == 20) {
	    if(curr_velocity == LEFT) add_to_list(TRUE);
	    else if(curr_velocity == RIGHT) add_to_list(FALSE);
	    add_enemy_cnt = 0;
	}
    }

    struct llist* temp = get_head();
    if(get_head()->val->col == 20 && curr_velocity == LEFT)
	curr_velocity = RIGHT;
    if(get_tail()->val->col >= 200 && curr_velocity == RIGHT)
	curr_velocity = LEFT;

    while(temp != NULL) {

	ENEMY* temp_enemy = temp->val;

	//If the enemy isn't on row 40, move it down
	if(temp_enemy->row != 30) {
	    temp_enemy->row++;
	}

	//If we're turning the enemies, change their velocities
	temp_enemy->velocity = curr_velocity;

	//Move them according to velocity
	temp_enemy->col += temp_enemy->velocity;
	
	//If our enemy isn't close to the next enemy, move it closer
	if(temp->next != NULL) {
	    if(temp_enemy->col + 20 < temp->next->val->col) {
		temp_enemy->col++;
	    }
	}

	temp = temp->next;
    }

}

void initEnemy(ENEMY* enemy) {

    if(get_head() != NULL) {
	if(curr_velocity == LEFT)
	    enemy->col = get_tail()->val->col + 20;
	else if(curr_velocity == RIGHT)
	    enemy->col = get_head()->val->col - 20;

	enemy->row = 0;
	enemy->height = 10;
	enemy->width = 20;
	enemy->velocity = curr_velocity;
    }
    else {
	curr_velocity = RIGHT;
	enemy->row = 0;
	enemy->col = 0;
	enemy->height = 10;
	enemy->width = 20;
	enemy->velocity = curr_velocity;
    }
}

void updateOldEnemy(struct llist* node) {
    node->old_val->row = node->val->row;
    node->old_val->col = node->val->col;
    node->old_val->height = node->val->height;
    node->old_val->width = node->val->width;
    node->old_val->velocity = node->val->velocity;
    node->old_val->delay = node->val->delay;
}
