struct animal {
	int aid;
	int ecosystem_id;
	struct animal *next;
};

struct ecosystem {
	int eco_id;
	int animal_count;
	struct animal * animals;
};

struct visitor {
	int vid;
	int last_visit;
	int visits;
	struct visitor * next;
};

struct employee_eco {
	int eco_id;
	struct employee_eco * next;
};

struct employee {
	int eid;
	int eco_count;
	struct employee_eco * employee_ecos;
	struct employee * prev;
	struct employee * next;
};

/*The array of the park’s ecosystems */
struct ecosystem ecosystems_array [50];
 
/* global variable, pointer to the beginning of the animals list*/
struct animal * animals_list; 	 

/* global variable, pointer to the beginning of the animals list*/
struct visitor * visitors_list; 

/* global variable, pointer to the sentinel node of the users list */
struct employee * employees_sentinel; 

struct visitor *sentinel;
