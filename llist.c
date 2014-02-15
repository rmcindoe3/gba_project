#include "llist.h"

struct llist *head = NULL;
struct llist *curr = NULL;
int listSize = 0;

struct llist* create_list(ENEMY* val)
{
    struct llist *ptr = (struct llist*)malloc(sizeof(struct llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = val; 
    ptr->next = NULL;

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

struct llist* add_to_list(ENEMY* val, int add_to_end)
{
    if(NULL == head)
    {
	return (create_list(val));
    }

    struct llist *ptr = (struct llist*)malloc(sizeof(struct llist));
    if(NULL == ptr)
    {
	printf("\n Node creation failed \n");
	return NULL;
    }
    ptr->val = val; 
    ptr->next = NULL;

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

    free(del);
    del = NULL;
    listSize--;

    return 0;
}

