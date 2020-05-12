#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zoo.h"

struct employee_animal *empl_animal_list_create();
struct employee_animal *empl_animal_list_insert(struct employee_animal *animals_head, int aid);
void empl_animals_list_print(struct employee_animal *animals_head);
int empl_animals_free(struct employee_animal *animals_head);
int empl_animals_list_isempty(struct employee_animal *animals_head);
int empl_animals_size(struct employee_animal *animals_head);

struct employee_animal *empl_animal_list_create()
{
	struct employee_animal *animals_head = (struct employee_animal *)malloc(sizeof(struct employee_animal));
	if (animals_head == NULL)
	{
		fprintf(stderr, "Memory allocation failure.\n");
		return 0;
	}

	animals_head->aid = -1;
	animals_head->next = NULL;
	return animals_head;
}

struct employee_animal *empl_animal_list_insert(struct employee_animal *animals_head, int aid)
{
	assert(animals_head != NULL);
	assert(aid >= 0);

	/* In the creation of the list a node is created with eid = -1.
	Here in the insertion we check if such a node exists. In such case the list is empty and the
	data of the inserted ecosystem (that is eco_id) replace the -1 in the eco_id field (so we don't need to allocate memory).
	In any other case we allocate memory for a new node and it is placed in beginning of the list. */
	if (animals_head->aid == -1)
	{
		animals_head->aid = aid;
		return animals_head;
	}

	struct employee_animal *new_node = (struct employee_animal *)malloc(sizeof(struct employee_animal));
	if (new_node == NULL)
	{
		fprintf(stderr, "Memory allocation problem.\n");
		return NULL;
	}

	new_node->aid = aid;
	new_node->next = animals_head;
	animals_head = new_node;
	return animals_head;
}

void empl_animals_list_print(struct employee_animal *animals_head)
{
	if(animals_head == NULL)
	{
	   printf("\n");
	   return;
	}
	
	struct employee_animal *current = animals_head;

	if (empl_animals_list_isempty(animals_head))
	{
		printf("\n");
		return;
	}

	while (current != NULL)
	{
		printf("<%d> ", current->aid);
		current = current->next;
	}
	printf("\n");
}

int empl_animals_free(struct employee_animal *animals_head)
{
	if (animals_head == NULL)
		return 1;
	struct employee_animal *temp;
	temp = animals_head;
	while (temp != NULL)
	{
		animals_head = animals_head->next;
		free(temp);
		temp = NULL;
		temp = animals_head;
	}
	free(animals_head);
	animals_head = NULL;
	temp = NULL;
	return 1;
}

int empl_animals_list_isempty(struct employee_animal *animals_head)
{
	assert(animals_head != NULL);
	return (animals_head->aid == -1);
}

int empl_animals_size(struct employee_animal *animals_head)
{
	assert(animals_head != NULL);

	int size = 0;
	struct employee_animal *temp = animals_head;
	while (temp != NULL)
	{
		if (temp->aid != -1)
			size++;
		temp = temp->next;
	}
	return size;
}

/*
int main()
{
	struct employee_animal *animals = empl_animal_list_create();
	animals = empl_animal_list_insert(animals, 1);
	animals = empl_animal_list_insert(animals, 12);
	animals = empl_animal_list_insert(animals, 3);
	animals = empl_animal_list_insert(animals, 4);
	animals = empl_animal_list_insert(animals, 15);
	animals = empl_animal_list_insert(animals, 6);
	animals = empl_animal_list_insert(animals, 17);
	animals = empl_animal_list_insert(animals, 8);
	animals = empl_animal_list_insert(animals, 19);
	animals = empl_animal_list_insert(animals, 10);
	empl_animals_list_print(animals);

	
}*/
