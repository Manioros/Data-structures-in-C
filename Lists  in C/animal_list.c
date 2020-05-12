#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "zoo.h"

unsigned int animal_list_is_empty();
unsigned int animal_list_size();
int animal_list_insert(int aid, int eco_id);
void animal_list_print(struct animal *head);
int distribute_in_ecosystems();
void animal_list_free();

unsigned int num_of_nodes = 0;


void animal_list_free(struct animal *head)
{
   struct animal *animal_list_current = head;

   while(animal_list_current != NULL)
   {
      head = head->next;
      free(animal_list_current);
      animal_list_current = NULL;
      animal_list_current = head;
   }
   head = NULL;
}

unsigned int animal_list_is_empty()
{
   return (animals_list == NULL);
}

unsigned int animal_list_size()
{
   return num_of_nodes;
}

int animal_list_insert(int aid, int eco_id)
{
   struct animal *temp; 
   struct animal *new_animal_node = (struct animal *)malloc(sizeof(struct animal));
   if(new_animal_node == NULL)
   {
      fprintf(stderr, "Memory allocation failure.");
      return 0;
   }

   struct animal *animal_list_current = animals_list;

   new_animal_node->aid = aid;
   new_animal_node->ecosystem_id = eco_id;
   new_animal_node->next = NULL;

   if(animal_list_is_empty() == 1) /*If list is empty assign new node to head*/
      animals_list = new_animal_node;
   else if(animals_list->aid > aid) /*If new node should be inserted as head */
   {
      temp = animals_list;
      animals_list = new_animal_node;
      animals_list->next = temp;
   }
   else /*find the position of insertion*/
   {
      while(animal_list_current != NULL) 
      {
	 if(animal_list_current->aid == aid)
	 {
	    printf("aid=%d already exists.\n", aid);
	    return 0;
	 }

	 if(animal_list_current->aid < aid)
	 {
	    temp = animal_list_current;
	    animal_list_current = animal_list_current->next;
	 }
	 else
	    break;
      }

      temp->next = new_animal_node;
      new_animal_node->next = animal_list_current;
   }
   
   num_of_nodes++;
   return 1;
}


void animal_list_print(struct animal *head)
{
   if(head == NULL)
      return;

   struct animal *animal_list_current = head;

   printf("Animals = ");
   while(animal_list_current != NULL)
   {
      printf("<aid %d>", animal_list_current->aid);
      animal_list_current = animal_list_current->next;
      if(animal_list_current != NULL)
	 printf(", ");
   }
   printf("\n");
}


int distribute_in_ecosystems()
{
   assert(animals_list != NULL);
   assert(!animal_list_is_empty());

   int i;
   struct animal *ecosystems_array_last_animal[50];
   for(i = 0; i < 50; i++)
      ecosystems_array_last_animal[i] == NULL;

   struct animal *animals_list_current = animals_list;

   while(animals_list_current != NULL)
   {
      int ecosystem_id = animals_list->ecosystem_id;
      assert(ecosystem_id >=0 && ecosystem_id < 50);

      if(ecosystems_array[ecosystem_id].animals == NULL)
      {
	 animals_list = animals_list->next;
	 ecosystems_array[ecosystem_id].animals = animals_list_current;
	 ecosystems_array_last_animal[ecosystem_id] = animals_list_current;
	 ecosystems_array_last_animal[ecosystem_id]->next = NULL;
	 animals_list_current = animals_list;
      }
      else
      {
	 animals_list = animals_list->next;
	 ecosystems_array_last_animal[ecosystem_id]->next = animals_list_current;
	 ecosystems_array_last_animal[ecosystem_id] = animals_list_current; /*We keep the last animal in the list*/
	 ecosystems_array_last_animal[ecosystem_id]->next = NULL;
	 animals_list_current = animals_list;
      }

      ecosystems_array[ecosystem_id].animal_count++;
      num_of_nodes--;
   }
   assert(num_of_nodes == 0);
}
