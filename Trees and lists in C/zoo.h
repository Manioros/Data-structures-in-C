struct animal {
	int aid;
	struct animal * lc;
	struct animal * rc;
};

struct ecosystem {
	int eco_id;
	struct animal * animals_tree;
};

struct visitor {
	int vid;
	int last_visit;
	int visits;
	struct visitor * parent;
	struct visitor * lc;
	struct visitor * rc;
};

struct employee_animal {
	int aid;
	struct employee_animal * next;
};

struct employee {
	int eid;
	int animal_count;
	struct employee_animal * employee_animal_list;
	struct employee * lc;
	struct employee * rc;
};

struct animal_to_employee {
	int aid;
	int eid;
	struct animal_to_employee * next;
};

/*The array of the park’s ecosystems */
struct ecosystem ecosystems_array [50];
 
/* global variable, pointer to the root of the animals tree*/
struct animal * animals_tree;

/* global variable, pointer to the root of the visitors tree*/
struct visitor * visitors_tree;
/* global variable, pointer to the rightmost node of the visitors tree*/
struct visitor * rightmost_visitor;

/* global variable, pointer to the root of the employees tree */
struct employee * employees_tree;

/* global variable, pointer to the sentinel node of the employees tree */
struct employee * employees_sentinel;

/* global variable, the hash table mapping animal ids to employee ids */
struct animal_to_employee *ANIMALS_TO_EMPLOYEES[128];
/* global variable, the maximum number of animals that are going to live in the park */
int total_animal_count;
/* global variable, the maximum animal id*/
int max_aid;
/* global variable, the array of primes for hashing */
int primes[168];

/*global variable for a node in queue implemented as a simple linked list*/
struct queue_item {
	struct visitor *visitor_node;
	struct queue_item *next;
};

/*Global variables for the head and the tail of the queue level based traversal of a tree*/
struct queue_item *queue_head, *queue_tail;
