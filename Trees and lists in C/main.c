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

 int max_animal_id = 999;
 int animals_total_count = 1000;
 int primes[168] = {    5,   7,  11,  13,  17,  19,  23,  29,  31,  37,
                       41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                       83,  89,  97, 101, 103, 107, 109, 113, 127, 131,
                      137, 139, 149, 151, 157, 163, 167, 173, 179, 181,
                      191, 193, 197, 199, 211, 223, 227, 229, 233, 239,
                      241, 251, 257, 263, 269, 271, 277, 281, 283, 293,
                      307, 311, 313, 317, 331, 337, 347, 349, 353, 359,
                      367, 373, 379, 383, 389, 397, 401, 409, 419, 421,
                      431, 433, 439, 443, 449, 457, 461, 463, 467, 479,
                      487, 491, 499, 503, 509, 521, 523, 541, 547, 557,
                      563, 569, 571, 577, 587, 593, 599, 601, 607, 613,
                      617, 619, 631, 641, 643, 647, 653, 659, 661, 673,
                      677, 683, 691, 701, 709, 719, 727, 733, 739, 743,
                      751, 757, 761, 769, 773, 787, 797, 809, 811, 821,
                      823, 827, 829, 839, 853, 857, 859, 863, 877, 881,
                      883, 887, 907, 911, 919, 929, 937, 941, 947, 953,
					  967, 971, 977, 983, 991, 997, 1009, 1013};

int initialize() {
   	int i;
	for(i = 0; i < 128; i++)
	{
	   ANIMALS_TO_EMPLOYEES[i] = malloc(sizeof(struct animal_to_employee));
	   ANIMALS_TO_EMPLOYEES[i] = (struct animal_to_employee *)empl_animal_sorted_list_create();
	}
	return 1;
}

int animal_lives_in_eco(int aid) {
	int result = bst_animals_insert(aid);
	printf("L <%d>\n", aid);
	bst_animals_print(animals_tree);
	printf("\nDONE\n");
	return result;
}

int distribute_animals() {
	int i;
	printf("D\n");
	printf("\tECOSYSTEMS:\n");
	for (i = 0; i < 50; i++)
	{
		ecosystems_array[i].eco_id = i;
		int split_aid = ecosystem_max_aid((i + 1) * 20 - 1);
		ecosystems_array[i].animals_tree = (struct animal *)bst_animals_split(split_aid);
		printf("\t<%d>: ", i);
		bst_animals_print(ecosystems_array[i].animals_tree);
		printf("\n");
	}
	printf("DONE\n");
	return 1;
}

/*Returns tha maximum aid in the ecosystem less than the give as argument aid*/
int ecosystem_max_aid(int aid) {
	int i;
	for (i = aid; i > 0; i--)
	{
		if ((struct animal *)bst_animals_search(i) != NULL)
			return i;
	}
	return -1;
}

int visit(int vid, int year) {
	int result = vt_insert(vid, year);
	printf("V <%d> <%d>\n", vid, year);
	vt_print();
	printf("\nDONE\n");
	return 1;
}

int delete_old_visitors(int years_interval) {
	printf("O <%d>\n", years_interval);
	vt_oldvisitors_delete(years_interval);
	vt_print();
	printf("\nDONE\n");
	return 1;
}

int hire(int eid) {
	printf("H <%d>\n\tEMPLOYEES:\n", eid);
	bst_employees_insert(eid);
	bst_employees_print(employees_tree, 0);
	printf("\nDONE\n");
	return 1;
}

int assign_animal(int eid, int aid) {
	printf("A <%d> <%d>\n\tEMPLOYEES:\n", eid, aid);
	struct employee *empl = (struct employee *)bst_employees_search(eid);
	
	if (empl != NULL){
		empl->employee_animal_list = (struct employee_animal *)empl_animal_list_insert(empl->employee_animal_list, aid);
		empl->animal_count++;

		ANIMALS_TO_EMPLOYEES[((eid*(eid*aid) + aid) % primes[167]) % 128] = empl_animal_sorted_list_insert(ANIMALS_TO_EMPLOYEES[((eid*(eid*aid)+aid)%primes[167])%128], aid, eid);

		assert(empl->employee_animal_list != NULL);
		bst_employees_print(employees_tree, 1);
		printf("\nDONE\n");
		return 1;
	}
	printf("\nDONE\n");
	return 0;
}

int retire(int eid) {
   	
	printf("R <%d> \n\tEMPLOYEES:\n", eid);
   	bst_employees_retire(eid);
	bst_employees_print(employees_tree, 1);
	printf("\nDONE\n");
	return 1;
}

int print_ecos(void) {
	int i;
	printf("X\n");
	printf("\tECOSYSTEMS:\n");
	for (i = 0; i < 50; i++)
	{
		printf("\t<%d>: ", i);
		bst_animals_print(ecosystems_array[i].animals_tree);
		printf("\n");
	}
	printf("DONE\n");
	return 1;
}

int print_visitors(void) {
	printf("\nY\n");
	vt_print();
	printf("\nDONE\n");
	return 1;
}

int print_employees(void) {
	printf("Z");
	printf("\tEMPLOYEES:\n");
	bst_employees_print(employees_tree, 1);
	printf("\nDONE\n");
	return 1;
}

int print_animal_to_employees(void) {
   	int i = 0;
	printf("\nE:\n");
	for(i = 0; i < 128; i++)
	{
	   printf("Chain <%d>:\n", i);
	   empl_animal_sorted_list_print(ANIMALS_TO_EMPLOYEES[i]);
	}
	printf("\nDONE\n");
	return 1;
}

int free_world() {
   	int i;
   	vt_free(visitors_tree);
	bst_animals_free(animals_tree);
   	for(i = 0; i < 49; i++)
	   bst_animals_free(ecosystems_array[i].animals_tree);
	for (i = 0; i < 128; i++)
		empl_animal_sorted_list_free(ANIMALS_TO_EMPLOYEES[i]);
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
		 * L <aid> */
		case 'L':
		{
			int aid;
			sscanf(buff, "%c %d", &event, &aid);
			DPRINT("%c %d\n", event, aid);


			if ( animal_lives_in_eco(aid) ) {
				DPRINT("L %d succeeded\n", aid);
			} else {
				fprintf(stderr, "L %d failed\n", aid);
			}

			break;
		}

		/* Distribute animals
		 * D */
		case 'D':
		{
			//bst_animals_print(bst_animals_split(579));
			//printf("--------------------------------\n");
			//bst_animals_print(animals_tree);
			//return;
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

		/* Assign animal to employee
		 * A <eid> <aid> */
		case 'A':
		{
			int eid;
			int aid;
			sscanf(buff, "%c %d %d", &event, &eid, &aid);
			DPRINT("%c %d %d\n", event, eid, aid);

			if ( assign_animal(eid, aid) ) {
				DPRINT("%c %d %d succeeded\n", event, eid, aid);
			} else {
				fprintf(stderr, "%c %d %d failed\n", event, eid, aid);
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

		/* Print animal_to_employees mapping
		 * E */
		case 'E':
		{
			sscanf(buff, "%c", &event);
			DPRINT("%c\n", event);

			if ( print_animal_to_employees() ) {
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
