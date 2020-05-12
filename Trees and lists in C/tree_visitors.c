#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "zoo.h"

#define CURRENT_YEAR 2016

/*Returns 1 if tree is perfect, otherwise 0*/
int vt_isperfect();

/*Returns 1 if visitors_node is left child, otherwise 0. Visitors_node must not be NULL or the root of the tree*/
int vt_isleftchild(struct visitor *visitors_node);
struct visitor *vt_search(struct visitor *visitor_node, int vid);
int vt_insert(int vid, int last_visit);
void vt_delete(struct visitor *visitor_node);
void vt_oldvisitors_delete(int years_interval);
void vt_print();
void vt_free(struct visitor *visitors_node);


/*---------------------------------------------------------------------------------------------------------------------*/
int enqueue(struct visitor *visitor_node);
int queue_isempty();
struct visitor *dequeue();
void queue_empty();

int enqueue(struct visitor *visitor_node)
{
	assert(visitor_node != NULL);
	struct queue_item *new_item = (struct queue_item *)malloc(sizeof(struct queue_item));
	if (new_item == NULL)
	{
		fprintf(stderr, "ENQUEUE: Memory allocation failure.\n");
		return 0;
	}

	new_item->visitor_node = visitor_node;
	new_item->next = NULL;

	if (queue_head == NULL)
	{
		queue_head = new_item;
		queue_tail = new_item;
		return 1;
	}

	queue_tail->next = new_item;
	queue_tail = new_item;

	return 1;
}

int queue_isempty()
{
	return (queue_head == NULL);
}

struct visitor *dequeue()
{
	if (queue_isempty())
	{
		printf("Queue is emppty.\n");
		return NULL;
	}
	struct visitor *dequeued_visitor = queue_head->visitor_node;
	struct queue_item *dequeued_item = queue_head;
	queue_head = queue_head->next;
	free(dequeued_item);
	return dequeued_visitor;
}

void queue_empty()
{
	while (!queue_isempty())
		dequeue();
	queue_tail = NULL;
}

void queue_delete_rightmost()
{
	struct queue_item *curr = queue_head;
	struct queue_item *prev = curr;
	if (queue_head->visitor_node == rightmost_visitor)
	{
		queue_head = NULL;
		free(curr);
		return;
	}
	curr = curr->next;
	while (curr != NULL)
	{
		if (curr->visitor_node == rightmost_visitor)
		{
			//printf("Found rightmost \t QueueTail:%d\tPrev:%d\n", queue_tail->visitor_node->vid, prev->visitor_node->vid);
			queue_tail = prev;
			free(curr);
			queue_tail->next = NULL;
			return;
		}
		prev = curr;
		curr = curr->next;
	}
}

void queue_print()
{
	struct queue_item *curr = queue_head;
	while (curr != NULL)
	{
		printf("%d ", curr->visitor_node->vid);
		curr = curr->next;
	}
	printf("\n");
	
}
/*-------------------------------------------------------------------------------------------------------------*/

void vt_free(struct visitor *visitors_node)
{
   if(visitors_node == NULL)
      return;
   vt_free(visitors_node->lc);
   vt_free(visitors_node->rc);
   free(visitors_node);
}


int vt_isperfect()
{
	struct visitor *current = visitors_tree;
	if (visitors_tree == NULL)
		return 0;

	while (current->rc != NULL)
		current = current->rc;

	if (current == rightmost_visitor)
		return 1;
	else
		return 0;
}

int vt_insert(int vid, int last_visit)
{
	struct visitor *current, *temp_parent;
	
	if ((current = vt_search(visitors_tree, vid)) != NULL)
	{
		current->last_visit = last_visit;
		current->visits++;
		return 1;
	}

	struct visitor *new_node = (struct visitor *)malloc(sizeof(struct visitor));
	if (new_node == NULL)
	{
		fprintf(stderr, "Memory allocation failure");
		return 0;
	}
	new_node->vid = vid;
	new_node->last_visit = last_visit;
	new_node->visits = 1;
	new_node->lc = NULL;
	new_node->rc = NULL;
	new_node->parent = NULL;

	if (visitors_tree == NULL)
	{
		visitors_tree = new_node;
		new_node->parent = NULL;
		rightmost_visitor = visitors_tree;
		
		return 1;
	}
	if (vt_isperfect())
	{
		current = visitors_tree;
		while (current->lc != NULL)
			current = current->lc;

		current->lc = new_node;
		new_node->parent = current;
		rightmost_visitor = new_node;
		
		return 1;
	}
	
	if (vt_isleftchild(rightmost_visitor))
	{
		rightmost_visitor->parent->rc = new_node;
		new_node->parent = rightmost_visitor->parent;
		rightmost_visitor = new_node;
		
		return 1;
	}
	if (!vt_isleftchild(rightmost_visitor))
	{
		current = rightmost_visitor->parent;
		while (current != NULL)
		{
			if (vt_isleftchild(current))
			{
				current = current->parent;
				break;
			}
			current = current->parent;
		}


		current = current->rc;
		while (current != NULL)
		{
			temp_parent = current;
			current = current->lc;
		}
		current = new_node;
		temp_parent->lc = new_node;
		new_node->parent = temp_parent;
		rightmost_visitor = new_node;
		return 1;


	}
}

int vt_isleftchild(struct visitor *visitors_node)
{
	assert(visitors_node != NULL);
	if (visitors_node->parent == NULL)
	{
		printf("vt_isleftchild\tRoot node was given\n");
		return 0;
	}
	
	if (visitors_node->parent->lc == visitors_node)
		return 1;
	else
		return 0;
}

struct visitor *vt_search(struct visitor *visitor_node, int vid)
{
	struct visitor *found = NULL;
	if (visitor_node == NULL)
		return NULL;
	if (visitor_node->vid == vid)
		return visitor_node;
	else
	{
		if (visitor_node->lc != NULL)
			if ((found = vt_search(visitor_node->lc, vid)))
				return found;
		if (visitor_node->rc != NULL)
			if ((found = vt_search(visitor_node->rc, vid)))
				return found;
	}

	return NULL;
}

void vt_oldvisitors_delete(int years_interval)
{
	struct visitor *curr_visitor;
	queue_empty();
	enqueue(visitors_tree);
	while (!queue_isempty())
	{
		//queue_print();
		curr_visitor = queue_head->visitor_node;
		if (CURRENT_YEAR - curr_visitor->last_visit > years_interval)
		{
			queue_delete_rightmost();
			vt_delete(curr_visitor);
			continue;
		}

		curr_visitor = dequeue();

		if (curr_visitor->lc != NULL)
			enqueue(curr_visitor->lc);
		if (curr_visitor->rc != NULL)
			enqueue(curr_visitor->rc);
	}
	queue_empty();
}

int vt_rightmost_isallleft()
{
	if (visitors_tree == NULL)
		return 0;
	struct visitor *current = visitors_tree;

	while (current->lc != NULL)
		current = current->lc;
	if (current == rightmost_visitor)
		return 1;
	return 0;
}

void vt_exchange_data(struct visitor *source, struct visitor *destination)
{
	destination->last_visit = source->last_visit;
	destination->vid = source->vid;
	destination->visits = source->visits;
}

void vt_delete(struct visitor *visitor_node)
{
	struct visitor *temp;
	assert(visitor_node != NULL);
	//if(visitor_node->parent != NULL)
	//    printf("---\nAttempt to delete %d (parent=%d, isleft=%d)\n", visitor_node->vid, visitor_node->parent->vid, vt_isleftchild(visitor_node));
	//else
	//    printf("---\nAttempt to delete %d\n", visitor_node->vid);

	/*If tree is made up only by its root.*/
	if ((visitor_node == visitors_tree) && (visitor_node->lc == NULL) && (visitor_node->rc == NULL))
	{
		free(visitors_tree);
		visitors_tree = NULL;
		return;
	}

	if (vt_isperfect(visitors_tree))
	{
		//printf("vt_isperfect(visitors_tree)\n");
		temp = rightmost_visitor->parent;
						//temp->rc = NULL; //CHANGED
		vt_exchange_data(rightmost_visitor, visitor_node);
		rightmost_visitor = temp->lc;
		free(temp->rc);
		temp->rc = NULL;
		//printf("Now Rightmost is %d\n", rightmost_visitor->vid);
	}
	else if (vt_rightmost_isallleft())
	{
		//printf("vt_rightmost_isallleft()\t\tRightmostvisitor is %d and isleft=%d\n", rightmost_visitor->parent->rc->vid, vt_isleftchild(rightmost_visitor));
		temp = rightmost_visitor->parent;
				//temp->lc = NULL; //CHANGED
		vt_exchange_data(rightmost_visitor, visitor_node);
		free(temp->lc);
		temp->lc = NULL;
		temp = visitors_tree;
		while (temp->rc != NULL)
		{
			//printf("Passing from node %d\n", temp->vid);
			temp = temp->rc;
		}
		rightmost_visitor = temp;
		//printf("Now Rightmost is %d\n", rightmost_visitor->vid);
	}
	else if (vt_isleftchild(rightmost_visitor))
	{
	    struct visitor *previous_left;
		//printf("vt_isleftchild(rightmost_visitor)\n");
		//printf("Rightmost is %d\n", rightmost_visitor->vid);
		temp = rightmost_visitor->parent;
				//temp->lc = NULL; //CHANGED
		previous_left = rightmost_visitor;
		//printf("Rightmost parent is %d\n", rightmost_visitor->parent->vid);
		vt_exchange_data(rightmost_visitor, visitor_node);
		//printf("Before while temp is %d\n", temp->vid);
		//printf("Before while temp->parent is %d\n", temp->parent->vid);
		while(temp->lc == previous_left)
		{
		    if(temp == visitors_tree)
		        break;
		    previous_left = temp;
		    temp = temp->parent;
		}
		temp = temp->lc;
		while (temp->rc != NULL)
			temp = temp->rc;
		rightmost_visitor->parent->lc = NULL;
		free(rightmost_visitor);
		rightmost_visitor = NULL;
		rightmost_visitor = temp;
	    //printf("Now Rightmost is %d\n", rightmost_visitor->vid);
	}
	else if (!vt_isleftchild(rightmost_visitor))
	{
		//printf("!vt_isleftchild(rightmost_visitor)\n");
		//printf("I am coming here\n");
		temp = rightmost_visitor->parent;
				//temp->rc = NULL; //CHANGED
		vt_exchange_data(rightmost_visitor, visitor_node);
		free(temp->rc);
		temp->rc = NULL;
		rightmost_visitor = temp->lc;
        //printf("Now Rightmost is %d\n", rightmost_visitor->vid);
		//printf("Now right_most is %d\n", rightmost_visitor->vid);
	}
	else
		assert(0);

	//vt_print();

}

int sumof2powers(int level)
{
	int i, sum = 1;
	for (i = 1; i <= level; i++)
	{
		sum = sum + (int)pow(2, i-1);
	}
	return sum;
}

void vt_print()
{
	int nodes_counter = 0;
	int level = 0;
	struct visitor *curr_visitor;
	queue_empty();
	enqueue(visitors_tree);
	while (!queue_isempty())
	{
		curr_visitor = dequeue();
		nodes_counter++;

		if (nodes_counter == sumof2powers(level))
		{
			level++;
			printf("\nLevel %d\t", level);
		}
		printf("<%d:%d:%d> ", curr_visitor->vid, curr_visitor->visits, curr_visitor->last_visit);
		
		if (curr_visitor->lc != NULL)
			enqueue(curr_visitor->lc);
		if (curr_visitor->rc != NULL)
			enqueue(curr_visitor->rc);
	}
}

/*
int main()
	{
		vt_insert(1, 2010);
		vt_insert(2, 2010);
		vt_insert(3, 2016);
		vt_insert(4, 2016);
		vt_insert(5, 2010);
		vt_insert(6, 2010);
		vt_insert(7, 2010);
		vt_insert(8, 2016);
		vt_insert(9, 2010);
		vt_insert(10, 2010);
		vt_insert(11, 2010);
		vt_insert(12, 2016);
		vt_insert(13, 2016);
		vt_insert(14, 2010);
		vt_insert(15, 2016);
		vt_insert(16, 2010);
		vt_insert(17, 2016);
		vt_insert(18, 2010);
		//vt_delete(vt_search(visitors_tree, 81));
		//vt_delete(vt_search(visitors_tree, 91));
		
		vt_insert(92, 2010);
		vt_insert(93, 2010);
		vt_insert(94, 2010);
		vt_insert(95, 2010);
		vt_insert(96, 2010);
		vt_insert(97, 2010);
		vt_insert(98, 2010);
		vt_insert(121, 2010);
		vt_insert(122, 2010);
		vt_insert(123, 2016);
		vt_insert(124, 2016);
		vt_insert(125, 2010);
		vt_insert(126, 2010);
		vt_insert(127, 2010);
		vt_insert(128, 2016);
		vt_insert(129, 2010);
		vt_insert(1211, 2010);
		vt_insert(1221, 2010);
		vt_insert(1231, 2016);
		vt_insert(1241, 2016);
		vt_insert(1251, 2010);
		vt_insert(1261, 2016);
		vt_insert(1271, 2010);
		vt_insert(1281, 2016);
		vt_insert(1291, 2010);
		vt_insert(1292, 2010);
		vt_insert(1293, 2010);
		vt_insert(1294, 2010);
		vt_insert(1295, 2010);
		vt_insert(1296, 2010);
		vt_insert(1297, 2010);
		vt_insert(1298, 2010);		
		//vt_print(visitors_tree);
			
		vt_insert(1, 2010);
		vt_insert(8, 2016);
		vt_insert(3, 2016);
		vt_insert(4, 2016);
		vt_insert(7, 2010);
		vt_insert(6, 2016);
		//vt_delete(vt_search(visitors_tree, 7));
		//vt_delete(vt_search(visitors_tree, 1));
		
		
		vt_oldvisitors_delete(5);
		//printf("Main's print\n");
		//vt_print();
		//vt_insert(6, 2016);
		vt_print();
	}
	*/
