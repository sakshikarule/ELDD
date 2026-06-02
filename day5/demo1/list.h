#ifndef __LIST_H
#define __LIST_H

#include<stdio.h>
#include<stdlib.h>

struct node{
	struct node *next;
	struct node *prev;
};

void list_init_head(struct node *head);

void list_add(struct node *newnode, struct node *head);
void list_add_tail(struct node *newnode, struct node *head);

void list_del(struct node *node);

#endif









