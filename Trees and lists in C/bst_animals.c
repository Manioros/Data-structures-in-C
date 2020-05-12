#include <stdio.h>
#include <stdlib.h>
#include "zoo.h"

int bst_animals_insert(int aid);
struct animal *bst_animals_search(int aid);
void bst_animals_print(struct animal *animals_node);
struct animal *bst_animals_split(int aid);
void bst_animals_free(struct animal *animals_node);

void bst_animals_free(struct animal *animals_node)
{
   if(animals_node == NULL)
      return;
   bst_animals_free(animals_node->lc);
   bst_animals_free(animals_node->rc);
   free(animals_node);
}

struct animal *bst_animals_search(int aid)
{
	if (animals_tree == NULL)
		return 0;
	struct animal *curr = animals_tree;
	while (curr != NULL)
	{
		if (aid > curr->aid)
			curr = curr->rc;
		else if (aid < curr->aid)
			curr = curr->lc;
		else
			return curr;
	}
	return NULL;
}


struct animal *bst_animals_split(int aid)
{
	if (aid < 0)
		return NULL;
	struct animal *T1 = NULL, *T2 = NULL, *curr, *temp;
	struct animal *lastleft_t2 = NULL, *lastright_t1 = NULL;
	curr = animals_tree;

	while (curr != NULL)
	{
		if (aid > curr->aid) {
			if (T1 == NULL)
			{
				T1 = curr;
				lastright_t1 = curr;
			}
			else {
				lastright_t1->rc = curr;
				lastright_t1 = curr;
			}
			curr = curr->rc;
		}
		else if (aid < curr->aid) {
			if (T2 == NULL)
			{
				T2 = curr;
				lastleft_t2 = curr;
			}
			else {
				lastleft_t2->lc = curr;
				lastleft_t2 = curr;
			}

			curr = curr->lc;
		}
		else {
			if (T1 == NULL && T2 == NULL)
			{
				T1 = curr;
				T2 = curr->rc;
				animals_tree = T2;
				T1->rc = NULL;
				return T1;
			}
			else if (T1 == NULL)
			{
				T1 = curr;
				lastleft_t2->lc = curr->rc;
				animals_tree = T2;
				curr->rc = NULL;
				return T1;
			}
			else if (T2 == NULL)
			{
				T2 = curr->rc;
				animals_tree = T2;
				lastright_t1->rc = curr;
				curr->rc = NULL;
				return T1;
			}
			lastright_t1->rc = curr;
			lastleft_t2->lc = curr->rc;
			animals_tree = T2;
			curr->rc = NULL;
			return T1;
		}
	}
	return NULL;
}

int bst_animals_insert(int aid)
{
	struct animal *prev = NULL;
	struct animal *current = animals_tree;
	struct animal *new_node = (struct animal *)malloc(sizeof(struct animal));
	if (new_node == NULL)
	{
		fprintf(stderr, "merory allocation failure.");
		return 0;
	}
		
	new_node->aid = aid;
	new_node->lc = NULL;
	new_node->rc = NULL;
	
	if (animals_tree == NULL)
	{
		animals_tree = new_node;
		return 1;
	}
	while (current != NULL)
	{
		if (current->aid == aid)
		{
			fprintf(stderr, "Animal not inserted. Already exists.");
			return 0;
		}

		prev = current;
		if (aid > current->aid)
			current = current->rc;
		else if (aid < current->aid)
			current = current->lc;
	}
	
	if (prev->aid > aid)
		prev->lc = new_node;
	else
		prev->rc = new_node;
	printf("\n");
	return 1;
}

void bst_animals_print(struct animal *animals_node)
{
	if (animals_node == NULL)
		return;

	bst_animals_print(animals_node->lc);
	printf("<%d>, ", animals_node->aid);
	bst_animals_print(animals_node->rc);
}

/*
int main()
{
	bst_animals_insert(15);
	bst_animals_insert(10);
	bst_animals_insert(25);
	bst_animals_insert(8);
	bst_animals_insert(13);
	bst_animals_insert(12);
	bst_animals_insert(14);
	bst_animals_insert(17);
	bst_animals_insert(27);
	bst_animals_insert(16);
	bst_animals_insert(20);
	bst_animals_insert(19);
	bst_animals_insert(22);
	bst_animals_insert(26);
	bst_animals_insert(30);

	//printf("%d\n", animals_tree->aid);
	//printf("%d\n", animals_tree->lc->aid);
	//printf("%d\n", animals_tree->rc->aid);
	//printf("%d\n", animals_tree->rc->rc->aid);

	bst_animals_print(bst_animals_split(15));
	printf("\n");
	bst_animals_print(animals_tree);
}
*/
