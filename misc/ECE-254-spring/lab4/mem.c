/**
 * @file memory.c
 * @brief: ECE254 Lab: memory allocation algorithm comparison template file
 * @author: 
 * @date: 2015-11-20
 */

/* includes */
#include <stdio.h> 
#include <stdlib.h> 
#include "mem.h"

/* defines */
struct node {
	struct node* prev;
	struct node* next;
	int size;
	void* start;
	unsigned char state;
};

/* global variables */
void* worst_fit_mem;
void* best_fit_mem;

/* Linked List Functions */

//insert function inserts new as current's next node
void insert(struct node* current, struct node* new) {
	struct node* temp = current->next;
	current->next = new;
	new->next = temp;
	new->prev = current;
}

//removes node from linked list
void remove_node(struct node* n) {
	if(n->prev != NULL) {			
		n->prev->next = n->next;
	}
	if(n->next != NULL) {
		n->next->prev = n->prev;
	}
}

/* Functions */

//returns the contol node of the smallest free memory space that can hold size
struct node* find_best_fit(size_t size) {
	struct node* best_fit = NULL;
	struct node* traverse = best_fit_mem;

	//simply traverse all nodes and find best fit
	while(traverse != NULL) {
		if(traverse->state == 0 && traverse->size >= size && (best_fit == NULL || traverse->size < best_fit->size)) {
			best_fit = traverse;
		}
		traverse=traverse->next;
	}

	return best_fit;
}

//returns the contol node of the smallest free memory space that can hold size
struct node* find_worst_fit(size_t size) {
	struct node* worst_fit = NULL;
	struct node* traverse = worst_fit_mem;

	//simply traverse all nodes and find best fit
	while(traverse != NULL) {
		if(traverse->state == 0 && traverse->size >= size && (worst_fit == NULL || traverse->size > worst_fit->size)) {
			worst_fit = traverse;
		}
		traverse=traverse->next;
	}

	return worst_fit;
}

//testing function to view memory of best fit (1) or worst fit (0)
void print_mem_info(int type) {
	struct node* traverse;
	if(type == 0) {
		traverse = (struct node*)worst_fit_mem;
	}
	else if(type == 1) {
		traverse = (struct node*)best_fit_mem;
	}
	
	while(traverse != NULL){
		printf("Control Address: %lu\tSize: %d\tState: %d\n",
			(long unsigned int)traverse, traverse->size, traverse->state);
		traverse=traverse->next;
	}
}

//merge right control node into the left and add there sizes
struct node* merge(struct node* left, struct node* right) {
	left->size=left->size + right->size + sizeof(struct node);
	remove_node(right);
	return left;
}

/* memory initializers */

int best_fit_memory_init(size_t size) {
	//ensure there is room for at least one control node
	if(size < sizeof(struct node)) {
		return -1;
	}

	//allocate the memory space
	best_fit_mem = malloc(size);

	//safety check
	if(best_fit_mem == NULL) {
		return -1;
	}

	//initialize the first control node directly at the start of the memory space
	struct node* head = (struct node*)(best_fit_mem);
	head->next = NULL;
	head->prev = NULL;
	head->size = size - sizeof(struct node);			//size is the rest of the memory space
	head->state = 0;									//0 = free
	head->start = best_fit_mem + sizeof(struct node);	//start is directly after control node

	return 0;
}

int worst_fit_memory_init(size_t size) {
	//ensure there is room for at least one control node
	if(size < sizeof(struct node)) {
		return -1;
	}

	//allocate the memory space
	worst_fit_mem = malloc(size);

	//safety check
	if(worst_fit_mem == NULL){
		return -1;
	}

	//initialize the first control node directly at the start of the memory space
	struct node* head = (struct node*)(worst_fit_mem);
	head->next = NULL;
	head->prev = NULL;
	head->size = size - sizeof(struct node);			//size is the rest of the memory space
	head->state = 0;									//0 = free
	head->start = worst_fit_mem + sizeof(struct node);	//start is directly after control node

	return 0;
}

/* memory allocators */

void *best_fit_alloc(size_t passed_size) {
	//4B align the size
	int size = passed_size%4 == 0 ? passed_size : passed_size + (4 - passed_size%4);

	//find the best fit space for the new block
	struct node* best_fit = find_best_fit(size);

	//ensure there was a block returned
	if(best_fit == NULL) {
		return NULL;
	}

	//if best fit space fit exactly just change state and return the mem address
	if(best_fit->size == size) {
		best_fit->state = 1;
		return best_fit->start;
	}

	//otherwise calculate leftover space
	int leftover_size=((int)(best_fit->size) - (int)size - (int)(sizeof(struct node)));

	if( leftover_size >= 0) {	//check there is enough room for new control node to mark free data
		//create new control node for left over data
		struct node* leftover = best_fit->start + size; 
		leftover->start = leftover + 1;
		leftover->state = 0;
		leftover->size = best_fit->size - size - (int)sizeof(struct node);
		leftover->next = NULL;
		leftover->prev = NULL;

		//set the state size of new allocated data block
		best_fit->state = 1;
		best_fit->size = size;

		//insert the leftover space node after the allocated block control node
		insert(best_fit, leftover);
	}
	else {
		//if there is not enough space for the another node, add the extra space to the allocated block
		best_fit->state = 1;
		best_fit->size = size + (int)(best_fit->size) - (int)size;
	}

	//return the allocated block
	return best_fit->start;
}

void *worst_fit_alloc(size_t passed_size) {
	//4B align the size
	int size = passed_size%4 == 0 ? passed_size : passed_size + (4 - passed_size%4);
	
	//find the worst fit space for the new block
	struct node* worst_fit = find_worst_fit(size);

	//ensure there was a block returned
	if(worst_fit == NULL) {
		return NULL;
	}

	//if worst fit space fit exactly just change state and return the mem address
	if(worst_fit->size == size) {
		worst_fit->state = 1;
		return worst_fit->start;
	}

	//otherwise calculate leftover space
	int leftover_size = ((int)(worst_fit->size) - (int)size - (int)(sizeof(struct node)));

	if( leftover_size >= 0) {	//check there is enough room for new control node to mark free data
		//create new control node for left over data
		struct node* leftover = worst_fit->start + size; 
		leftover->start = leftover + 1;
		leftover->state = 0;
		leftover->size = worst_fit->size - size - (int)sizeof(struct node);
		leftover->next = NULL;
		leftover->prev = NULL;

		//set the state size of new allocated data block
		worst_fit->state = 1;
		worst_fit->size = size;

		//insert the leftover space node after the allocated block control node
		insert(worst_fit, leftover);
	} else {
		//if there is not enough space for the another node, add the extra space to the allocated block
		worst_fit->state = 1;
		worst_fit->size = size + (int)(worst_fit->size) - (int)size;
	}

	//return the allocated block
	return worst_fit->start;
}


/* memory de-allocators */

void best_fit_dealloc(void *ptr) {
	//ensure user did not pass NULL
	if(ptr == NULL){
		return;
	}

	//traverse until finding the control node of ptr
	struct node* free_me = best_fit_mem;
	while(free_me->start != ptr){
		free_me = free_me->next;

		//if it cannot be found return NULL
		if(free_me == NULL) {
			return;
		}
	}

	//check if there is a free node to the left
	if(free_me->prev != NULL && free_me->prev->state == 0) {
		//merge nodes
		struct node* merged_node = merge(free_me->prev, free_me);
		
		//check if there was also a free node to the right
		if(free_me->next != NULL && merged_node->next->state == 0) {
			//merge nodes
			merge(merged_node, merged_node->next);
		}
	} 
	//check if there is a free node to the right
	else if(free_me->next != NULL && free_me->next->state == 0) {
		free_me->state = 0;	//set state to 0 as right gets merged into left
							//and free_me state is currently still 1
		//merge nodes
		merge(free_me, free_me->next);
	} else {
		//if no adjacent nodes are free simply mark the space as free
		free_me->state = 0;
	}

	return;
}

void worst_fit_dealloc(void *ptr) {
	//ensure user did not pass NULL
	if(ptr == NULL){
		return;
	}

	//traverse until finding the control node of ptr
	struct node* free_me = worst_fit_mem;
	while(free_me->start != ptr){
		free_me = free_me->next;

		//if it cannot be found return NULL
		if(free_me == NULL) {
			return;
		}
	}

	//check if there is a free node to the left
	if(free_me->prev != NULL && free_me->prev->state == 0) {
		//merge nodes
		struct node* merged_node = merge(free_me->prev, free_me);
		
		//check if there was also a free node to the right
		if(free_me->next != NULL && merged_node->next->state == 0) {
			//merge nodes
			merge(merged_node, merged_node->next);
		}
	} 
	//check if there is a free node to the right
	else if(free_me->next != NULL && free_me->next->state == 0) {
		free_me->state = 0;	//set state to 0 as right gets merged into left
							//and free_me state is currently still 1
		//merge nodes
		merge(free_me, free_me->next);
	} else {
		//if no adjacent nodes are free simply mark the space as free
		free_me->state = 0;
	}

	return;
}

/* memory algorithm metric utility functions */

/* count how many free blocks are less than the input size */ 
int best_fit_count_extfrag(size_t size) {
	int count=0;
	struct node* traverse = best_fit_mem;

	//traverse the best fit mem space counting all free spaces smaller than size
	while(traverse != NULL){
		if(traverse->state == 0 && traverse->size < size) {
			++count;
		}
		traverse=traverse->next;
	}
	return count;
}

int worst_fit_count_extfrag(size_t size) {
	int count=0;
	struct node* traverse = worst_fit_mem;

	//traverse the best fit mem space counting all free spaces smaller than size
	while(traverse != NULL){
		if(traverse->state == 0 && traverse->size < size) {
			++count;
		}
		traverse=traverse->next;
	}
	return count;
}
