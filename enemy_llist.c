#include "enemy_llist.h"
#include "myLib.h"
#include "levels.h"
#include "text.h"

struct enemy_llist *enemy_head = NULL;
struct enemy_llist *enemy_curr = NULL;
int enemy_listSize = 0;

signed short curr_velocity = RIGHT;

struct enemy_llist* create_enemy_list(char type)
{
    struct enemy_llist *ptr = (struct enemy_llist*)malloc(sizeof(struct enemy_llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->old_val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->next = NULL;

    initEnemy(ptr->val, type);
    updateOldEnemy(ptr);

    enemy_listSize++;
    enemy_head = ptr;
    enemy_curr = ptr;
    return ptr;
}

void empty_enemy_list(void) {

    if(NULL == enemy_head) return;

    struct enemy_llist* ptr = enemy_head;
    while(ptr->next != NULL)
    {
	delete_from_enemy_list(ptr->next->val);
    }
    delete_from_enemy_list(ptr->val);
    return;
}

int get_enemy_list_size(void) {
    return enemy_listSize;
}

ENEMY* get_enemy(int index) {
    if(index < 0 || index >= enemy_listSize) {
	return NULL;
    }
    int i;
    struct enemy_llist* ptr = enemy_head;
    for(i = 0; i < index; i++) {
	ptr = ptr->next;
    }
    return ptr->val;
}

struct enemy_llist* get_enemy_head(void) {
    return enemy_head;
}

struct enemy_llist* get_enemy_tail(void) {
    return enemy_curr;
}

struct enemy_llist* add_to_enemy_list(char type, int add_to_end)
{
    if(NULL == enemy_head)
    {
	return (create_enemy_list(type));
    }

    struct enemy_llist *ptr = (struct enemy_llist*)malloc(sizeof(struct enemy_llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->old_val = (ENEMY*)malloc(sizeof(ENEMY)); 
    ptr->next = NULL;

    initEnemy(ptr->val, type);
    updateOldEnemy(ptr);

    if(add_to_end)
    {
	enemy_curr->next = ptr;
	enemy_curr = ptr;
    }
    else
    {
	ptr->next = enemy_head;
	enemy_head = ptr;
    }
    enemy_listSize++;
    return ptr;
}

struct enemy_llist* search_in_enemy_list(ENEMY* val, struct enemy_llist **prev)
{
    struct enemy_llist *ptr = enemy_head;
    struct enemy_llist *tmp = NULL;
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

int delete_from_enemy_list(ENEMY* val)
{
    struct enemy_llist *prev = NULL;
    struct enemy_llist *del = NULL;

    del = search_in_enemy_list(val,&prev);
    if(del == NULL)
    {
	return -1;
    }
    else
    {
	if(prev != NULL)
	    prev->next = del->next;

	if(del == enemy_curr)
	{
	    enemy_curr = prev;
	}
	if(del == enemy_head)
	{
	    enemy_head = del->next;
	}
    }

    free(del->val);
    free(del->old_val);
    free(del);
    del = NULL;
    enemy_listSize--;

    return 0;
}

void addEnemies() {

    static int add_enemy_cnt = 0;

    //If there are less than 8 enemies currently falling, then drop one...
    if(get_enemy_list_size() < NUM_ENEMIES) {

        //If the next enemy isn't a boss or the enemy is is empty, 
        //  start incrementing the count.
	if(getNextEnemyType() != BOSS || !get_enemy_list_size()) add_enemy_cnt++;

	if(determineEnemySpawn(getNextEnemyType(), add_enemy_cnt)) {

            //If the next enemy is a boss and there are no enemies on the screen...
	    if(getNextEnemyType() == BOSS) {
                if(get_enemy_list_size() == 0) {
                    add_to_enemy_list(pullNextEnemy(), TRUE);
                    add_enemy_cnt = 0;
                }
	    }
            //If the next enemy isn't a boss...
	    else {
		if(curr_velocity == LEFT) add_to_enemy_list(pullNextEnemy(), TRUE);
		else if(curr_velocity == RIGHT) add_to_enemy_list(pullNextEnemy(), FALSE);
		add_enemy_cnt = 0;
	    }
	}
    }
}

/** moveEnemies *******************************************
 * Takes in our array of enemy OBJECTs and moves all of
 *  them. 
 *********************************************************/
void moveEnemies() {

    struct enemy_llist* temp = get_enemy_head();

    if(get_enemy_head() != NULL) {
        if(get_enemy_head()->val->col == 20 && curr_velocity == LEFT)
            curr_velocity = RIGHT;
        if(get_enemy_tail()->val->col >= (220-get_enemy_tail()->val->width) && curr_velocity == RIGHT)
            curr_velocity = LEFT;
    }

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
	    if(curr_velocity == RIGHT) {
		if(temp_enemy->col + 20 < temp->next->val->col) {
		    temp_enemy->col++;
		}
	    }
	    else {
		if(temp_enemy->col + 20 < temp->next->val->col) {
		    temp->next->val->col--;
		}
	    }
	}

	temp = temp->next;
    }

}

/** determineEnemySpawn ****************************************************
 *
 ******************************************************************/
char determineEnemySpawn(char enemyType, int add_enemy_cnt) {
    if(add_enemy_cnt >= 20) {
        if(enemyType == BOSS && add_enemy_cnt >= 40) {
            return TRUE;
        }
        else {
            return TRUE;
        }
    }
    return FALSE;
}

/** printEnemy ****************************************************
 * Prints values of the variables for this enemy to the screen.
 * For testing purposes only.
 ******************************************************************/
void printEnemy(ENEMY* enemy) {

    char temp[] = "velocity:    ";
    
    sprintf(temp, "row: %02d", enemy->row);
    drawString(10, 10, temp, GREEN);
    sprintf(temp, "col: %02d", enemy->col);
    drawString(20, 10, temp, GREEN);
    sprintf(temp, "height: %02d", enemy->height);
    drawString(30, 10, temp, GREEN);
    sprintf(temp, "width: %02d", enemy->width);
    drawString(40, 10, temp, GREEN);
    sprintf(temp, "velocity: %02d", enemy->velocity);
    drawString(50, 10, temp, GREEN);
    sprintf(temp, "type: %02d", enemy->type);
    drawString(60, 10, temp, GREEN);
    sprintf(temp, "health: %02d", enemy->health);
    drawString(70, 10, temp, GREEN);

}

void initEnemy(ENEMY* enemy, char type) {

    if(get_enemy_head() != NULL) {
	if(curr_velocity == LEFT)
	    enemy->col = get_enemy_tail()->val->col + 20;
	else if(curr_velocity == RIGHT)
	    enemy->col = get_enemy_head()->val->col - 20;

	enemy->row = 0;
	enemy->height = 10;
	enemy->width = 20;
	enemy->velocity = curr_velocity;
	enemy->type = type;
	enemy->health = 10;
    }
    else {
	curr_velocity = RIGHT;
	enemy->row = 0;
	enemy->col = 0;
	enemy->height = 10;
	enemy->width = 20;
	enemy->velocity = curr_velocity;
	enemy->type = type;
	enemy->health = 10;
    }

    if(type == NORM) {
	enemy->health = 10;
    }
    else if(type == BIGG) {
        enemy->health = 20;
    }
    else if(type == TRIS) {
        enemy->health = 40;
    }
    else if(type == BOSS) {
        enemy->health = 40*(1+getCurrentLevel());
	enemy->height = 20;
	enemy->width = 40;
    }
    
}

void updateOldEnemy(struct enemy_llist* node) {
    node->old_val->row = node->val->row;
    node->old_val->col = node->val->col;
    node->old_val->height = node->val->height;
    node->old_val->width = node->val->width;
    node->old_val->velocity = node->val->velocity;
    node->old_val->delay = node->val->delay;
    node->old_val->health = node->val->health;
}
