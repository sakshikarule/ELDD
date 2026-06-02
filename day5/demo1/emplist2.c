#include<string.h>
#include "list.h"

#define offset_of(type, member) \
	    ((long)&(((type *)0)->member))

#define container_of(maddr, type, member) \
	    ((type *)((char *)maddr - offset_of(type,member)))

#define list_entry(maddr, type, member) \
	    container_of(maddr, type, member)

#define list_for_each(trav, head) \
	    for(trav = head->next ; trav != head ; trav = trav->next)

#define list_for_safe(trav, temp, head) \
	    for(trav = head->next, temp = trav->next; trav != head; trav = temp, temp = trav->next)

struct employee{
	   int empid;
	   char name[20];
	   long salary;
	   struct node elist;
};

void display(struct node *head)
{
	struct node *trav;
	printf("Employee list : \n");
	// for(trav = head->next ; trav != head ; trav = trav->next)
	list_for_each(trav, head)
	{
		struct employee *ptr = list_entry(trav,struct empolyee, elist);
		printf("%d %s %ld\n", ptr->empid, ptr->name, ptr->salary);
	}
}

void delete_all(struct node *head)
{
	struct node *trav, *temp;
	//for(trav = head->next, temp = trav->next; trav!= head; trav = temp, temp = trav->next)
	list_for_each_safe(trav, temp, head)
	{
		list_del(trav);
		struct empolyee *ptr = list_entry(trav, struct empolyee, elist);
		free(ptr);
	}
}

int main(void)
{
	struct node head;

	list_init_head(&head);

	struct employee *newemp = (struct employee *)malloc(sizeof(struct employee));
	newemp->empid = 1;
	strcpy(newemp->name, "abc");
	newemp->salary = 12345;
	list_add(&newemp->elist, &head);

	newemp = (struct employee *)malloc(sizeof(struct employee));
	newemp->empid = 2;
	strcpy(newemp->name, "pqr");
	newemp->salary = 23145;
	list_add(&newemp->elist, &head);

	newemp = (struct employee *)malloc(sizeof(struct employee));
	newemp->empid = 3;
	strcpy(newemp->name, "xyz");
	newemp->salary = 43215;
	list_add_tail(&newemp->elist, &head);

	display(&head);
	delete_all(&head);
	return 0;
}








	    
