#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zoo.h"

struct employee_eco *empl_ecos_list_create();
struct employee_eco *empl_ecos_list_insert(struct employee_eco *ecos_head, int eco_id);
int empl_ecos_list_isempty(struct employee_eco *ecos_head);
void empl_ecos_list_print(struct employee_eco *ecos_head);
int empl_ecos_size(struct employee_eco *ecos_head);
int empl_ecos_free(struct employee_eco *ecos_head);

int empl_ecos_free(struct employee_eco *ecos_head)
{
   if(ecos_head == NULL)
      return 1;
   struct employee_eco *temp;
   temp = ecos_head;
   while(temp != NULL)
   {
      ecos_head = ecos_head->next;
      free(temp);
      temp = NULL;
      temp = ecos_head;
   }
   free(ecos_head);
   ecos_head = NULL;
   temp = NULL;
   return 1;
}

int empl_ecos_list_isempty(struct employee_eco *ecos_head)
{
   assert(ecos_head != NULL);
   return (ecos_head->eco_id == -1);
}

int empl_ecos_size(struct employee_eco *ecos_head)
{
   assert(ecos_head != NULL);

   int size = 0;
   struct employee_eco *temp = ecos_head;
   while(temp != NULL)
   {
      if(temp->eco_id != -1)
	 size++;
      temp = temp->next;
   }
   return size;
}


struct employee_eco *empl_ecos_list_create()
{
   struct employee_eco *ecos_head = (struct employee_eco *)malloc(sizeof(struct employee_eco));
   if(ecos_head == NULL)
   {
      fprintf(stderr, "Memory allocation failure.\n");
      return 0;
   }
   
   ecos_head->eco_id = -1;
   ecos_head->next = NULL;
   return ecos_head;
}

struct employee_eco *empl_ecos_list_insert(struct employee_eco *ecos_head, int eco_id)
{
   assert(ecos_head != NULL);
   assert(eco_id >= 0);

   if(ecos_head->eco_id == -1)
   {
      ecos_head->eco_id = eco_id;
      return ecos_head;
   }

   struct employee_eco *new_node = (struct employee_eco *)malloc(sizeof(struct employee_eco));
   if(new_node == NULL)
   {
      fprintf(stderr, "Memory allocation problem.\n");
      return NULL;
   }

   new_node->eco_id = eco_id;

   new_node->next = ecos_head;
   ecos_head = new_node;
   return ecos_head;
}

void empl_ecos_list_print(struct employee_eco *ecos_head)
{
   assert(ecos_head != NULL);
   struct employee_eco *current = ecos_head;

   if(empl_ecos_list_isempty(ecos_head))
   {
      printf("\n");
      return;
   }

   while(current != NULL)
   {
      printf("<eco_id %d> ", current->eco_id);
      current = current->next;
   }
   printf("\n");
}

/*
int main()
{
   struct employee_eco *emps_ecos = empl_ecos_list_create();
   emps_ecos = empl_ecos_list_insert(emps_ecos, 1);
   emps_ecos = empl_ecos_list_insert(emps_ecos, 2);
   emps_ecos = empl_ecos_list_insert(emps_ecos, 3);
   emps_ecos = empl_ecos_list_insert(emps_ecos, 4);
   emps_ecos = empl_ecos_list_insert(emps_ecos, 5);
   empl_ecos_list_print(emps_ecos);
   printf("Size = %d\n", empl_ecos_size(emps_ecos));

      struct employee_eco *emps_ecos1 = empl_ecos_list_create();
   empl_ecos_list_print(emps_ecos1);
   printf("Size = %d\n", empl_ecos_size(emps_ecos1));
   printf("--------------");
   empl_ecos_free(emps_ecos);
   empl_ecos_free(emps_ecos1);
   empl_ecos_list_print(emps_ecos);
   empl_ecos_list_print(emps_ecos1);
   printf("Size = %d\n", empl_ecos_size(emps_ecos));
   printf("Size = %d\n", empl_ecos_size(emps_ecos1));

}
*/
