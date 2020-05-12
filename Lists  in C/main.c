#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#include "zoo.h"

#define BUFFER_SIZE 1024  /**< Maximum length of a line in input file */

/* Uncomment the following line to enable debugging prints 
 * or comment to disable it */
#define DEBUG

#ifdef DEBUG
#define DPRINT(...) fprintf(stderr, __VA_ARGS__);
#else  /* DEBUG */
#define DPRINT(...)
#endif /* DEBUG */


int initialize() {
   int i;

   for(i = 0; i < 50; i++)
   {
      ecosystems_array[i].eco_id = i;
      ecosystems_array[i].animal_count = 0;
      ecosystems_array[i].animals = NULL;
   }

   if(!visitors_list_create())
   {
      fprintf(stderr, "Failed creating visitors' list.\n");
      return 0;
   }

   return 1;
}

int animal_lives_in_eco(int aid, int eco_id) {
   return animal_list_insert(aid, eco_id);
}

int distribute_animals() {
   distribute_in_ecosystems();

   int i;
   printf("ECOSYSTEMS:\n");
   for(i = 0; i < 50; i++)
   {
      printf("<ecosystem %d>:\t", ecosystems_array[i].eco_id);
      animal_list_print(ecosystems_array[i].animals);
      if(ecosystems_array[i].animal_count == 0)
	 printf("\n");
   }
   return 1;
}

int visit(int vid, int year) {
   struct visitor *_visitor;
   _visitor = (struct visitor *)visitors_list_search(vid);

   if(_visitor == NULL)
      return visitors_list_insert(vid, year);
   else
      return visitors_list_update(_visitor, year);
return 1;
}

int delete_old_visitors(int years_interval) {
   visitors_list_delete(years_interval);
   visitors_list_print();
   return 1;
}

int hire(int eid) {
   employees_list_insert(eid);
   employees_list_print();
   return 1;
}

int assign_eco(int eid, int eco_id) {
   struct employee *empl = (struct employee *)employees_list_find(eid);
   if(empl == NULL)
      return 0;
   empl->employee_ecos = (struct employee_eco *)empl_ecos_list_insert(empl->employee_ecos, eco_id);
   empl->eco_count++;
   assert(empl->employee_ecos != NULL);
   employees_list_fullprint(); 
   return 1;
}

int retire(int eid) {
   struct employee_eco *temp_eco;
   struct employee *six_employees[6], *temp_empl;
   struct employee *retired_empl = (struct employee *)employees_list_find(eid);
   int limit, i, j, num_of_empl_ecosystems, share, rest = 0; /* rest will keep num_of_empl_ecosystems % 6 */
   if(retired_empl == NULL)
      return 0;

   temp_eco = retired_empl->employee_ecos;

   num_of_empl_ecosystems = empl_ecos_size(retired_empl->employee_ecos);
   share = num_of_empl_ecosystems/6;
   rest = num_of_empl_ecosystems%6;

   /* The following two whiles keep the six employees in an array (struct employees *six_employees[6])*/
   temp_empl = retired_empl->next;
   j = 3;
   while(temp_empl != NULL)
   {
      if(j == 6)
	 break;
      if(temp_empl->eid != -1)
      {
	 six_employees[j] = temp_empl;
	 j++;
      }
      temp_empl = temp_empl->next;
   }
   
   temp_empl = retired_empl->prev;
   j = 0;
   while(temp_empl != NULL)
   {
      if(j == 3)
	 break;      
      if(temp_empl->eid != -1)
      {
	 six_employees[j] = temp_empl;
	 j++;
      }
      temp_empl = temp_empl->prev;
   }

   for(i = 0; i < 5; i++)
   {
      for(j = 0; j < share; j++)
      {
	 six_employees[i]->employee_ecos = (struct employee_eco *)empl_ecos_list_insert(six_employees[i]->employee_ecos, temp_eco->eco_id);
	 temp_eco = temp_eco->next;
      }
   }

   assert(six_employees[i]->employee_ecos != NULL);
   if(rest == 0)
   {
      for(j = 0; j < share; j++)
      {
	 assert(temp_eco != NULL);
	 six_employees[i]->employee_ecos = (struct employee_eco *)empl_ecos_list_insert(six_employees[i]->employee_ecos, temp_eco->eco_id);
	 temp_eco = temp_eco->next;
      }
   }
   else
   {      
      for(j = 0; j < rest; j++)
      {
	 assert(temp_eco != NULL);
	 six_employees[i]->employee_ecos = (struct employee_eco *)empl_ecos_list_insert(six_employees[i]->employee_ecos, temp_eco->eco_id);
	 temp_eco = temp_eco->next;
      }

   }
   
   empl_ecos_free(retired_empl->employee_ecos);
   employees_list_delete(retired_empl->eid);

   employees_list_fullprint();
      
   return 1;
}

/* Returns the position of the minimum positive number in the array nums */
int minimum(int nums[])
{
   int i, min = -1;

   for(i = 0; i < 5; i++)
      if(nums[i] < nums[min] && nums[i] >= 0)
	 min = i;

   return min;
}

int gold_visitors() {
   assert(visitors_list != NULL);
   struct visitor *temp = visitors_list;
   struct visitor *goldvisitors[5];
   int i, visits_counter[5];
   for(i = 0; i < 5; i++)
   {
      if(temp != NULL && temp != sentinel)
      {
	 goldvisitors[i] = temp;
	 visits_counter[i] = temp->visits;
      }
      else
      {
	 goldvisitors[i] = NULL;
	 visits_counter[i] = -1;
      }
      temp = temp->next;
   }

   while(temp != sentinel)
   {
      int current_minimum_visits_index = minimum(visits_counter);
      if(visits_counter[current_minimum_visits_index] < temp->visits)
      {
	 visits_counter[current_minimum_visits_index] = temp->visits;
	 goldvisitors[current_minimum_visits_index] = temp;
      }
      temp = temp->next;
   }
   temp = visitors_list;

   printf("Gold Visitors = ");
   for(i = 0; i < 5; i++)
   {
      printf("<vid %d: %d: %d>", goldvisitors[i]->vid, goldvisitors[i]->visits, goldvisitors[i]->last_visit);
      if(i != 4)
	 printf(", ");
   }
   printf("\n");

   return 1;
}

int print_ecos(void) {
   int i = 0;
   printf("ECOSYSTEMS:\n");
   for(i = 0; i < 50; i++)
   {
      printf("<ecosystem %d: %d>: ", i, ecosystems_array[i].animal_count);
      animal_list_print(ecosystems_array[i].animals);
      if(ecosystems_array[i].animal_count == 0)
	 printf("\n");
   }
      
   return 1;
}

int print_visitors(void) {
   visitors_list_print();
   return 1;
}

int print_employees(void) {
   employees_list_fullprint();
   return 1;
}

int free_world() {
   int i;
   animal_list_free(animals_list);
   for(i = 0; i < 50; i++)
      animal_list_free(ecosystems_array[i].animals);
   visitors_list_free();
   employees_list_free(); /* Here employees ecosystem list is also freed */
   return 1;
}



/**
 * @brief The main function
 *
 * @param argc Number of arguments
 * @param argv Argument vector
 *
 * @return 0 on success
 *         1 on failure
 */
int main(int argc, char** argv)
{
   FILE *fin = NULL;
   char buff[BUFFER_SIZE], event;

   initialize();

   /* Check command buff arguments */
   if ( argc != 2 ) {
      fprintf(stderr, "Usage: %s <input_file> \n", argv[0]);
      return EXIT_FAILURE;
   }

   /* Open input file */
   if (( fin = fopen(argv[1], "r") ) == NULL ) {
      fprintf(stderr, "\n Could not open file: %s\n", argv[1]);
      perror("Opening test file\n");
      return EXIT_FAILURE;
   }

   /* Read input file buff-by-buff and handle the events */
   while ( fgets(buff, BUFFER_SIZE, fin) ) {

      DPRINT("\n>>> Event: %s", buff);

      switch(buff[0]) {

	 /* Comment */
	 case '#':
	    break;

	    /* Animal lives in eco
	     * L <aid> <eco_id> */
	 case 'L':
	    {
	       int aid;
	       int eco_id;
	       sscanf(buff, "%c %d %d", &event, &aid, &eco_id);
	       DPRINT("%c %d %d\n", event, aid, eco_id);

	       if ( animal_lives_in_eco(aid, eco_id) ) {
		  animal_list_print(animals_list);
		  DPRINT("L %d %d succeeded\n", aid, eco_id);
	       } else {
		  fprintf(stderr, "L %d %d failed\n", aid, eco_id);
	       }

	       break;
	    }

	    /* Distribute animals
	     * D */
	 case 'D':
	    {
	       sscanf(buff, "%c", &event);
	       DPRINT("%c\n", event);

	       if ( distribute_animals() ) {
		  DPRINT("%c succeeded\n", event);
	       } else {
		  fprintf(stderr, "%c failed\n", event);
	       }

	       break;
	    }

	    /* Visit
	     * V <vid> <year> */
	 case 'V':
	    {
	       int vid;
	       int year;
	       sscanf(buff, "%c %d %d", &event, &vid, &year);
	       DPRINT("%c %d %d\n", event, vid, year);

	       if ( visit(vid, year) ) {
		  visitors_list_print();
		  DPRINT("%c %d %d succeeded\n", event, vid, year);
	       } else {
		  fprintf(stderr, "%c %d %d failed\n", event, vid, year);
	       }

	       break;
	    }

	    /* Delete old visitors
	     * O <years_interval> */
	 case 'O':
	    {
	       int years_interval;
	       sscanf(buff, "%c %d", &event, &years_interval);
	       DPRINT("%c %d\n", event, years_interval);

	       if ( delete_old_visitors(years_interval) ) {
		  DPRINT("%c %d succeeded\n", event, years_interval);
	       } else {
		  fprintf(stderr, "%c %d failed\n", event, years_interval);
	       }

	       break;
	    }

	    /* Hire employee
	     * H <eid> */
	 case 'H':
	    {
	       int eid;
	       sscanf(buff, "%c %d", &event, &eid);
	       DPRINT("%c %d\n", event, eid);

	       if ( hire(eid) ) {
		  DPRINT("%c %d succeeded\n", event, eid);
	       } else {
		  fprintf(stderr, "%c %d failed\n", event, eid);
	       }

	       break;
	    }

	    /* Assign eco to employee
	     * A <eid> <eco_id> */
	 case 'A':
	    {
	       int eid;
	       int eco_id;
	       sscanf(buff, "%c %d %d", &event, &eid, &eco_id);
	       DPRINT("%c %d %d\n", event, eid, eco_id);

	       if ( assign_eco(eid, eco_id) ) {
		  DPRINT("%c %d %d succeeded\n", event, eid, eco_id);
	       } else {
		  fprintf(stderr, "%c %d %d failed\n", event, eid, eco_id);
	       }

	       break;
	    }

	    /* Retire employee
	     * R <eid> */
	 case 'R':
	    {
	       int eid;
	       sscanf(buff, "%c %d\n", &event, &eid);
	       DPRINT("%c %d\n", event, eid);

	       if ( retire(eid) ) {
		  DPRINT("%c %d succeeded\n", event, eid);
	       } else {
		  fprintf(stderr, "%c %d failed\n", event, eid);
	       }

	       break;
	    }

	    /* Gold (most frequent) visitors
	     * G */
	 case 'G':
	    {
	       sscanf(buff, "%c", &event);
	       DPRINT("%c\n", event);

	       if ( gold_visitors() ) {
		  DPRINT("%c succeeded\n", event);
	       } else {
		  fprintf(stderr, "%c failed\n", event);
	       }

	       break;
	    }

	    /* Print ecosystems
	     * X */
	 case 'X':
	    {
	       sscanf(buff, "%c", &event);
	       DPRINT("%c\n", event);

	       if ( print_ecos() ) {
		  DPRINT("%c succeeded\n", event);
	       } else {
		  fprintf(stderr, "%c failed\n", event);
	       }

	       break;
	    }

	    /* Print visitors
	     * Y */
	 case 'Y':
	    {
	       sscanf(buff, "%c", &event);
	       DPRINT("%c\n", event);

	       if ( print_visitors() ) {
		  DPRINT("%c succeeded\n", event);
	       } else {
		  fprintf(stderr, "%c failed\n", event);
	       }

	       break;
	    }

	    /* Print employees
	     * Z */
	 case 'Z':
	    {
	       sscanf(buff, "%c", &event);
	       DPRINT("%c\n", event);

	       if ( print_employees() ) {
		  DPRINT("%c succeeded\n", event);
	       } else {
		  fprintf(stderr, "%c failed\n", event);
	       }

	       break;
	    }


	    /* Empty line */
	 case '\n':
	    break;

	    /* Ignore everything else */
	 default:
	    DPRINT("Ignoring buff: %s \n", buff);

	    break;
      }
   }

   free_world();
   return (EXIT_SUCCESS);
}
