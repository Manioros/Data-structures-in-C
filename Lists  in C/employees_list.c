#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zoo.h"


int employees_list_create();
int employees_list_insert(int eid);
void employees_list_print();
struct employee *employees_list_find(int eid);
int employees_list_delete(int eid);


void employees_list_free()
{
   struct employee *current = employees_sentinel;
   struct employee *temp = current;

   while(temp != employees_sentinel)
   {
      empl_ecos_free(temp->employee_ecos);
      free(temp);
      temp = NULL;
      temp = current->next;      
   }
   free(employees_sentinel);
   employees_sentinel = NULL;
}

int employees_list_create()
{
   employees_sentinel = (struct employee *)malloc(sizeof(struct employee));
   if(employees_sentinel == NULL)
   {
      fprintf(stderr, "Memory allocation failure.\n");
      return 0;
   }
   
   employees_sentinel->eid = -1;
   employees_sentinel->eco_count = -1;
   employees_sentinel->prev = employees_sentinel;
   employees_sentinel->next = employees_sentinel;
}

int employees_list_insert(int eid)
{
   if(employees_sentinel == NULL)
      employees_list_create();
   assert(employees_sentinel != NULL);
   struct employee *new_node = (struct employee *)malloc(sizeof(struct employee));
   if(new_node == NULL)
   {
      fprintf(stderr, "Memory allocation failure.\n");
      return 0;
   }   
   new_node->eid = eid;
   new_node->eco_count = 0;
   new_node->employee_ecos = (struct employee_eco *)empl_ecos_list_create();
   assert(new_node->employee_ecos != NULL);

   new_node->next = employees_sentinel->next;
   new_node->prev = employees_sentinel;
   employees_sentinel->next->prev = new_node;
   employees_sentinel->next = new_node;

   return 1;
}


void employees_list_print()
{
   assert(employees_sentinel != NULL && employees_sentinel->next != NULL && employees_sentinel->prev != NULL);
   struct employee *current = employees_sentinel->next;
   printf("EMPLOYEES: ");
   while(current != employees_sentinel)
   {
      printf("<eid %d>", current->eid);
      current = current->next;
      if(current != employees_sentinel)
         printf(", ");
   }
   printf("\n");
}

void employees_list_fullprint()
{
   assert(employees_sentinel != NULL && employees_sentinel->next != NULL && employees_sentinel->prev != NULL);
   struct employee *current = employees_sentinel->next;
   printf("EMPLOYEES:\n");
   while(current != employees_sentinel)
   {
      printf("<employee %d: %d>: ", current->eid, current->eco_count);
      empl_ecos_list_print(current->employee_ecos);
      current = current->next;
   }
   printf("\n");
}

struct employee *employees_list_find(int eid)
{
   struct employee *current = employees_sentinel->next;

   while(current != employees_sentinel)
   {
      if(current->eid == eid)
	 return current;
      current = current->next;
   }
   return NULL;
}

int employees_list_delete(int eid)
{
   struct employee *empl_to_delete = employees_list_find(eid);
   if(empl_to_delete == NULL)
      return 0;
   empl_to_delete->prev->next = empl_to_delete->next;
   empl_to_delete->next->prev = empl_to_delete->prev;
   free(empl_to_delete);
   empl_to_delete = NULL;
   return;
}

/*
int main()
{
   employees_list_insert(1);
   employees_list_insert(2);
   employees_list_insert(3);
   employees_list_insert(4);
   employees_list_insert(5);
   employees_list_insert(6);
   employees_list_insert(7);
   employees_list_print();

   employees_list_delete(1);
   employees_list_print();
   employees_list_delete(7);
   employees_list_print();
   employees_list_delete(4);
   employees_list_print();
}
*/
