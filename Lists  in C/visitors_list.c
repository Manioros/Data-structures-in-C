#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "zoo.h"

struct visitor *sentinel;
struct visitor *visitors_list_search(int vid);
void visitors_list_print();
int visitors_list_create();
int visitors_list_insert(int vid, int last_visit);
struct visitor *visitors_list_search(int vid);
int visitors_list_update(struct visitor *_visitor, int last_visit);
int visitors_list_delete(unsigned int years_interval);
void visitors_list_free();


void visitors_list_free()
{
   struct visitor *temp = visitors_list;
   
   while(temp != sentinel)
   {
      visitors_list = visitors_list->next;
      free(temp);
      temp = NULL;
      temp = visitors_list;
   }
   free(sentinel);
   visitors_list = NULL;
   sentinel = NULL;
}

int visitors_list_create()
{
   assert(sentinel == NULL);
   sentinel = (struct visitor *)malloc(sizeof(struct visitor));
   if(sentinel == NULL)
   {
      fprintf(stderr, "Memory allocation failure.\n");
      return 0;
   }

   sentinel->vid = -1;
   sentinel->last_visit = -1;
   sentinel->visits = -1;
   sentinel->next = NULL;

   visitors_list = sentinel;

   return 1;
}

int visitors_list_insert(int vid, int last_visit)
{
   assert(sentinel != NULL);
   assert(visitors_list_search(vid) == NULL); /*To insert visitor with vid should not already exist in the list */

   struct visitor *temp;
   struct visitor *new_node = (struct visitor *)malloc(sizeof(struct visitor));
   if(new_node == NULL)
   {
      fprintf(stderr, "Memory allocation failure.\n");
      return 0;
   }

   new_node->vid = vid;
   new_node->last_visit = last_visit;
   new_node->visits = 1;
   
   new_node->next = visitors_list;
   visitors_list = new_node;

   return 1;
}


void visitors_list_print()
{
   assert(sentinel != NULL);
   assert(visitors_list != NULL);
   struct visitor *temp = visitors_list;
   
   printf("Visitors = ");
   while(temp != sentinel)
   {
      printf("<vid%d: %d: %d>", temp->vid, temp->visits, temp->last_visit);
      temp = temp->next;
      if(temp != sentinel)
	 printf(", ");
   }
   printf("\n");
}

struct visitor *visitors_list_search(int vid)
{
   assert(vid >= 0);
   assert(sentinel != NULL);
   assert(visitors_list != NULL);

   struct visitor *temp = visitors_list;
   
   while(temp != sentinel)
   {
      if(temp->vid == vid)
	return temp;
      temp = temp->next; 
   }
   return NULL;
}

int visitors_list_update(struct visitor *_visitor, int last_visit)
{
   assert(_visitor != NULL);
   assert(_visitor != sentinel);

   _visitor->last_visit = last_visit;
   _visitor->visits++;
   return 1;
}

int visitors_list_delete(unsigned int years_interval)
{
   assert(years_interval >= 0);
   assert(sentinel != NULL);
   assert(visitors_list != NULL);

   struct visitor *temp = visitors_list;
   struct visitor *previous = visitors_list;
   int current_year = 2016;
   
   while(temp != sentinel)
   {
      if(current_year - temp->last_visit > years_interval)
      {
	 struct visitor *_temp = temp;
	 if(temp == visitors_list)
	 {
	    visitors_list = visitors_list->next;
	    previous = visitors_list;
	    temp = visitors_list->next;
	    free(_temp);
	    _temp = NULL;
	 }
	 else
	 {
	    previous->next = temp->next;
	    temp = temp->next;
	    free(_temp);
	    _temp = NULL;
	 }
      }
      else
      {
	 previous = temp;
	 temp = temp->next;
      }
   }

   return 1;
}
