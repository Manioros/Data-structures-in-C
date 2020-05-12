#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "zoo.h"

int bst_employees_create();
int bst_employees_insert(int eid);
void bst_employees_print(struct employee *empl_node, int with_animals);
struct employee *bst_employees_search(int eid);
struct employee *bst_employees_predecessor(int eid);
void bst_employees_free(struct employee *empl_node);

void bst_employees_free(struct employee *empl_node)
{
   if(empl_node == NULL)
      return;

   bst_employees_free(empl_node->lc);
   bst_employees_free(empl_node->rc);
   free(empl_node->employee_animal_list);
   free(empl_node);
}

int bst_employees_create()
{
	assert(employees_sentinel == NULL);
	employees_sentinel = (struct employee *)malloc(sizeof(struct employee));
	if (employees_sentinel == NULL)
	{
		fprintf(stderr, "EMPLOYEES: Memory allocation failure.\n");
		return 0;
	}

	employees_sentinel->eid = -1;
	employees_sentinel->animal_count = -1;
	employees_sentinel->employee_animal_list = NULL;
	employees_sentinel->lc = NULL;
	employees_sentinel->rc = NULL;
	return 1;
}

int bst_employees_insert(int eid)
{
	if (employees_sentinel == NULL)
		if (!bst_employees_create())
		{
			fprintf(stderr, "EMPLOYEES: Failure in initializing employees tree.\n");
			return 0;
		}
		
	struct employee *new_empl = (struct employee *)malloc(sizeof(struct employee));
	if (new_empl == NULL)
	{
		fprintf(stderr, "EMPLOYEES: Memory allocation failure.\n");
		return 0;
	}

	new_empl->eid = eid;
	new_empl->animal_count = 0;
	new_empl->employee_animal_list = (struct employee_animal *)empl_animal_list_create();
	new_empl->lc = employees_sentinel;
	new_empl->rc = employees_sentinel;

	if (employees_tree == NULL)
	{
		employees_tree = new_empl;
		return 0;
	}

	struct employee *curr = employees_tree;
	struct employee *prev = curr;
	while (curr != employees_sentinel)
	{
		prev = curr;
		if (eid > curr->eid)
			curr = curr->rc;
		else if (eid < curr->eid)
			curr = curr->lc;
		else
			return 0;
	}
	if (eid > prev->eid)
		prev->rc = new_empl;
	else
		prev->lc = new_empl;

	return 1;	
}

void bst_employees_print(struct employee *empl_node, int with_animals)
{
	assert(empl_node != NULL);
	if (empl_node == employees_sentinel)
		return;

	bst_employees_print(empl_node->lc, with_animals);
	if(with_animals == 0)
		printf("\t<%d>, ", empl_node->eid);
	else {
		printf("\t<%d: %d>", empl_node->eid, empl_node->animal_count);
		empl_animals_list_print(empl_node->employee_animal_list);
	}
	bst_employees_print(empl_node->rc, with_animals);
}

struct employee *bst_employees_parent(int eid)
{
   	assert(employees_tree != NULL);
	if(employees_tree->eid == eid)
	   return NULL;
	struct employee *curr = employees_tree;
	struct employee *parent = NULL;

	while (curr != employees_sentinel)
	{
		if (eid > curr->eid)
		{
		   	parent = curr;
			curr = curr->rc;
		}
		else if (eid < curr->eid)
		{
		   	parent = curr;
			curr = curr->lc;
		}
		else
			break;
	}
	if(curr == employees_sentinel)
	   return NULL;

	return parent;
}

struct employee *bst_employees_search(int eid) {
	struct employee *curr = employees_tree;

	while (curr != employees_sentinel)
	{
		if (eid > curr->eid)
			curr = curr->rc;
		else if (eid < curr->eid)
			curr = curr->lc;
		else
			return curr;
	}
	return NULL;
}


/*if rightone = 1 looks up for the right most leaf 
 * otherwise fot the left most leaf
 */
struct employee *bst_employees_mostleaf(struct employee *node, int rightone)
{
   assert(employees_tree != NULL);
   struct employee *curr = node;
   if(rightone == 1)
   {
      while(curr->rc != employees_sentinel)
	 curr = curr->rc;
   }
   else
   {
      while(curr->lc != employees_sentinel)
	 curr = curr->lc;
   }
   return curr;
}


struct employee *bst_employees_predecessor(int eid)
{
   struct employee *predecessor = NULL;
   struct employee *curr = NULL, *parent = NULL;
   if(employees_tree->eid == eid && employees_tree->lc != NULL)
      return bst_employees_mostleaf(employees_tree->lc, 1);

   if(employees_tree->eid == eid && employees_tree->lc == NULL)
      return NULL;

   curr = employees_tree;
   parent = employees_tree;

   while(curr != employees_sentinel)
   {
      if(eid > curr->eid)
      {
	 if(predecessor == NULL)
	    predecessor = curr;
	 else if(predecessor->eid < curr->eid)
	    predecessor = curr;
	 curr = curr->rc;
      }
      else if(eid < curr->eid)
	 curr = curr->lc;
      else
      {
	 if(curr->lc != employees_sentinel)
	 {
	    struct employee *rightmost = bst_employees_mostleaf(curr->lc, 1);
	    if(rightmost == employees_sentinel)
	       return curr->lc;
	    else 
	       return rightmost;
	 }
	 break;
      }
   }
   return predecessor;
}


struct employee *bst_employees_retire(int eid)
{
   if(employees_tree == NULL || employees_tree == employees_sentinel)
      return NULL;
   
   struct employee *retired = bst_employees_search(eid);
   if(retired == NULL)
      return NULL;

   struct employee *predecessor = bst_employees_predecessor(eid);
   struct employee *employee_to_assign = (predecessor != NULL)?predecessor:bst_employees_mostleaf(employees_tree, 1);
   int assign_eid = employee_to_assign->eid;

   assert(employee_to_assign != NULL);
   
   struct employee_animal *animallist_retired = retired->employee_animal_list;
   while(animallist_retired != NULL && animallist_retired->aid != -1)
   {
      employee_to_assign->employee_animal_list = (struct employee_animal *)empl_animal_list_insert(employee_to_assign->employee_animal_list, animallist_retired->aid);
      employee_to_assign->animal_count++;
	  //printf("Deletion from chain index %d the aid=%d, eid=%d\n", ((eid*(eid*animallist_retired->aid) + animallist_retired->aid) % primes[167]) % 128, animallist_retired->aid, eid);
	  //printf("Insertion to chain index %d the aid=%d, eid=%d\n", ((assign_eid*(assign_eid*animallist_retired->aid) + animallist_retired->aid) % primes[167]) % 128, animallist_retired->aid, assign_eid);
	  ANIMALS_TO_EMPLOYEES[((eid*(eid*animallist_retired->aid) + animallist_retired->aid) % primes[167]) % 128] = 
		  empl_animal_sorted_list_delete(ANIMALS_TO_EMPLOYEES[((eid*(eid*animallist_retired->aid) + animallist_retired->aid) % primes[167]) % 128], animallist_retired->aid);
	  ANIMALS_TO_EMPLOYEES[((assign_eid*(assign_eid*animallist_retired->aid) + animallist_retired->aid) % primes[167]) % 128] = 
		  empl_animal_sorted_list_insert(ANIMALS_TO_EMPLOYEES[((assign_eid*(assign_eid*animallist_retired->aid) + animallist_retired->aid) % primes[167]) % 128], animallist_retired->aid, assign_eid);
	  animallist_retired = animallist_retired->next;
   }
   empl_animals_free(retired->employee_animal_list);
   retired->employee_animal_list = NULL;

   struct employee *parent = bst_employees_parent(eid);
   if(retired->lc == employees_sentinel && retired->rc == employees_sentinel)
   {
      free(retired);
      retired = NULL;
      parent->lc = employees_sentinel;
      parent->rc = employees_sentinel;
   }
   else if(retired->lc == employees_sentinel)
   {
      if(parent->rc == retired)
	 parent->rc = retired->rc;
      else
	 parent->lc = retired->rc;
      free(retired);
      retired = NULL;
   }
   else if(retired->rc == employees_sentinel)
   {
      if(parent->rc == retired)
	 parent->rc = retired->lc;
      else
	 parent->lc = retired->lc;
      free(retired);
      retired = NULL;
   }
   else
   {
      struct employee *predecessor_parent = bst_employees_parent(predecessor->eid);
         
      retired->eid = predecessor->eid;
      retired->animal_count = predecessor->animal_count;
      retired->employee_animal_list = predecessor->employee_animal_list;

      if(predecessor->rc == employees_sentinel && predecessor->lc == employees_sentinel)
      {
	 if(predecessor_parent->lc == predecessor)
	    predecessor_parent->lc = employees_sentinel;
	 else
	    predecessor_parent->rc = employees_sentinel;
	 free(predecessor);
	 predecessor = NULL;
      }
      else if(predecessor->rc != employees_sentinel)
      {
	 if(predecessor_parent->lc == predecessor)
	    predecessor_parent->lc = predecessor->lc;
	 else if(predecessor_parent->rc == predecessor)
	    predecessor_parent->rc = predecessor->lc;
	 free(predecessor);
	 predecessor = NULL;
      }
      else if(predecessor->lc != employees_sentinel)
      {
	 if(predecessor_parent->lc == predecessor)
	    predecessor_parent->lc = predecessor->rc;
	 else if(predecessor_parent->rc == predecessor)
	    predecessor_parent->rc = predecessor->rc;
	 free(predecessor);
	 predecessor = NULL;
      }
   }

   return employee_to_assign;
}

/*
int main()
{
	bst_employees_insert(15);
	bst_employees_insert(10);
	bst_employees_insert(25);
	bst_employees_insert(8);
	bst_employees_insert(12);
	bst_employees_insert(13);
	bst_employees_insert(11);
	bst_employees_insert(14);
	bst_employees_insert(17);
	bst_employees_insert(27);
	bst_employees_insert(16);
	bst_employees_insert(20);
	bst_employees_insert(19);
	bst_employees_insert(22);
	bst_employees_insert(26);
	bst_employees_insert(30);

	int id = 15;
	struct employee *empl13 = bst_employees_search(id);
//	struct employee *empl12 = bst_employees_search(12);
	empl13->employee_animal_list = (struct employee_animal *)empl_animal_list_insert(empl13->employee_animal_list, 1);
	empl13->employee_animal_list = (struct employee_animal *)empl_animal_list_insert(empl13->employee_animal_list, 2);
	empl13->employee_animal_list = (struct employee_animal *)empl_animal_list_insert(empl13->employee_animal_list, 3);
	empl13->employee_animal_list = (struct employee_animal *)empl_animal_list_insert(empl13->employee_animal_list, 4);
	
	bst_employees_print(employees_tree, 0);
	empl_animals_list_print(empl13->employee_animal_list);
	
	empl_animals_list_print(empl12->employee_animal_list);
	if(empl13->employee_animal_list == NULL)
	empl_animals_list_print(empl13->employee_animal_list);*/
	
	//bst_employees_print(employees_tree);
	//printf("Predecessor of 8 = <%d>\n", bst_employees_predecessor(8)->eid);
	/*printf("Predecessor of 15 = <%d>\n", bst_employees_predecessor(15)->eid);
	printf("Predecessor of 10 = <%d>\n", bst_employees_predecessor(10)->eid);
	printf("Predecessor of 25 = <%d>\n", bst_employees_predecessor(25)->eid);
	printf("Predecessor of 13 = <%d>\n", bst_employees_predecessor(13)->eid);
	printf("Predecessor of 12 = <%d>\n", bst_employees_predecessor(12)->eid);
	printf("Predecessor of 14 = <%d>\n", bst_employees_predecessor(14)->eid);
	printf("Predecessor of 17 = <%d>\n", bst_employees_predecessor(17)->eid);
	printf("Predecessor of 27 = <%d>\n", bst_employees_predecessor(27)->eid);
	printf("Predecessor of 16 = <%d>\n", bst_employees_predecessor(16)->eid);
	printf("Predecessor of 20 = <%d>\n", bst_employees_predecessor(20)->eid);
	printf("Predecessor of 19 = <%d>\n", bst_employees_predecessor(19)->eid);
	printf("Predecessor of 22 = <%d>\n", bst_employees_predecessor(22)->eid);
	printf("Predecessor of 26 = <%d>\n", bst_employees_predecessor(26)->eid);
	printf("Predecessor of 30 = <%d>\n", bst_employees_predecessor(30)->eid);
	
	//printf("Predecessor of 10 = <%d>\n", bst_employees_predecessor(10)->eid);
	//printf("Parent of 10 = <%d>\n", bst_employees_parent(10)->eid);
	bst_employees_retire(id);
	bst_employees_print(employees_tree, 1);
}*/
