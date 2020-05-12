#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zoo.h"

struct animal_to_employee *empl_animal_sorted_list_create();
struct animal_to_employee *empl_animal_sorted_list_insert(struct animal_to_employee *animals_head, int aid, int eid);
struct animal_to_employee *empl_animal_sorted_list_delete(struct animal_to_employee *animals_head, int aid);
void empl_animal_sorted_list_print(struct animal_to_employee *animals_head);
int empl_animal_sorted_list_free(struct animal_to_employee *animals_head);
int empl_animal_sorted_list_isempty(struct animal_to_employee *animals_head);
int empl_animal_sorted_list_size(struct animal_to_employee *animals_head);

struct animal_to_employee *empl_animal_sorted_list_create()
{
	struct animal_to_employee *animals_head = (struct animal_to_employee *)malloc(sizeof(struct animal_to_employee));
	if (animals_head == NULL)
	{
		fprintf(stderr, "Memory allocation failure.\n");
		return 0;
	}

	animals_head->aid = -1;
	animals_head->eid = -1;
	animals_head->next = NULL;
	return animals_head;
}

struct animal_to_employee *empl_animal_sorted_list_delete(struct animal_to_employee *animals_head, int aid)
{
	struct animal_to_employee *curr, *prev;
	curr = animals_head;
	prev = animals_head;
	if (animals_head->aid == aid)
	{
		animals_head = animals_head->next;
		free(curr);
		curr = NULL;
		return animals_head;
	}

	while (curr->next != NULL)
	{
		prev = curr;
		curr = curr->next;
		if (curr->aid == aid)
		{
			prev->next = curr->next;
			free(curr);
			curr = NULL;
			return animals_head;
		}
	}
	return animals_head;
}

struct animal_to_employee *empl_animal_sorted_list_insert(struct animal_to_employee *animals_head, int aid, int eid)
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
		animals_head->eid = eid;
		return animals_head;
	}

	struct animal_to_employee *new_node = (struct animal_to_employee *)malloc(sizeof(struct animal_to_employee));
	if (new_node == NULL)
	{
		fprintf(stderr, "Memory allocation problem.\n");
		return NULL;
	}

	new_node->aid = aid;
	new_node->eid = eid;
	if(animals_head->aid > aid)
	{
	   new_node->next = animals_head;
	   animals_head = new_node;
	   return animals_head;
	}

	struct animal_to_employee *prev = animals_head;
	struct animal_to_employee *curr = animals_head;
	while(curr != NULL)
	{
	   if(curr->aid == aid)
	   {
	      free(new_node);
	      return NULL;

	   }
	   else if(curr->aid < aid)
	   {
	      prev = curr;
	      curr = curr->next;
	   }
	   else 
	      break;
	}

	prev->next = new_node;
	new_node->next = curr;

        return animals_head;
}

void empl_animal_sorted_list_print(struct animal_to_employee *animals_head)
{
	if(animals_head == NULL)
	{
	   printf("\n");
	   return;
	}
	
	struct animal_to_employee *current = animals_head;

	if (empl_animal_sorted_list_isempty(animals_head))
	{
		printf("\n");
		return;
	}
	//current = current->next->next;

	while (current != NULL)
	{
		printf("\t<%d> : <%d>\n", current->aid, current->eid);
		current = current->next;
	}

}

int empl_animal_sorted_list_free(struct animal_to_employee *animals_head)
{
	if (animals_head == NULL)
		return 1;
	struct animal_to_employee *temp;
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

int empl_animal_sorted_list_isempty(struct animal_to_employee *animals_head)
{
	assert(animals_head != NULL);
	return (animals_head->aid == -1);
}

int empl_animal_sorted_list_size(struct animal_to_employee *animals_head)
{
	assert(animals_head != NULL);

	int size = 0;
	struct animal_to_employee *temp = animals_head;
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
	struct animal_to_employee *animals = empl_animal_sorted_list_create();
	animals = empl_animal_sorted_list_insert(animals, 12, 1);
	animals = empl_animal_sorted_list_insert(animals, 1, 2);
	animals = empl_animal_sorted_list_insert(animals, 3, 1);
	animals = empl_animal_sorted_list_insert(animals, 4, 3);
	animals = empl_animal_sorted_list_insert(animals, 15, 4);
	animals = empl_animal_sorted_list_insert(animals, 6, 1);
	animals = empl_animal_sorted_list_insert(animals, 17, 1);
	animals = empl_animal_sorted_list_insert(animals, 8, 2);
	animals = empl_animal_sorted_list_insert(animals, 19, 3);
	animals = empl_animal_sorted_list_insert(animals, 10, 2);
	
	empl_animal_sorted_list_print(animals);
	animals = empl_animal_sorted_list_delete(animals, 19);
	printf("\n");
	empl_animal_sorted_list_print(animals);
}*/
