#include "list.h"

void list_init_head(struct node *head)
{
	head->next = head->prev = head;
}

void list_add(struct node *newnode, struct node *head)
{
	newnode->next = head->next;
	newnode->prev = head;
	head->next->prev = newnode;
	head->next = newnode;
}
void list_add_tail(struct node *newnode, struct node *head)
{
	newnode->next = head;
	newnode->prev = head->prev;
	head->prev->next = newnode;
	head->prev = newnode;
}

void list_del(struct node *node)
{
	node->prev->next = node->next;
	node->next->prev = node->prev;
}




