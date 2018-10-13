/**
  * ECE254 Linux Dynamic Memory Management Lab
  * @file: main_test.c
  * @brief: The main file to write tests of memory allocation algorithms
  */ 

/* includes */
/* system provided header files. You may add more */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* non-system provided header files. 
   Do not include more user-defined header files here
 */
#include "mem.h"
// TODO: void print_mem_info(){}

// tests completely filling the mem space and deallocates two blocks and then allocates one into the larger free block
void test1_worst_fit() {
	printf("Test 1 Worst Fit \n");
	worst_fit_memory_init(1000);

	void* a = worst_fit_alloc(200);
	worst_fit_alloc(220);
	void* c = worst_fit_alloc(220);
	worst_fit_alloc(200);

	printf("Full\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(a);
	worst_fit_dealloc(c);

	printf("Freed blocks 1 and 3\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_alloc(100);
	printf("Allocated 100, should go into block 3\n");
	print_mem_info(0);
	printf("---\n");

	return;
}

//tests deallocation and merging free blocks
//fills mem space then deallocates blocks from left to right and merges
void test2_worst_fit() { 	
	printf("Test 2 Worst Fit \n");
	worst_fit_memory_init(1000);

	void* a = worst_fit_alloc(200);
	void* b = worst_fit_alloc(220);
	void* c = worst_fit_alloc(220);
	void* d = worst_fit_alloc(200);

	printf("Full\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(a);
	printf("Freed block 1\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(b);
	printf("Freed block 2\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(c);
	printf("Freed block 3\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(d);
	printf("Freed block 4, all free\n");
	print_mem_info(0);
	printf("---\n");

	return;
}

//tests deallocation with free blocks on either side
void test3_worst_fit() { 
	printf("Test 3 Worst Fit \n");
	worst_fit_memory_init(1000);

	void* a = worst_fit_alloc(200);
	void* b = worst_fit_alloc(220);
	void* c = worst_fit_alloc(220);
	worst_fit_alloc(200);

	printf("Full\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(a);
	printf("Freed block 1\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(c);
	printf("Freed block 3\n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(b);
	printf("Freed block 2\n");
	print_mem_info(0);
	printf("---\n");

	return;
}

//tests to make sure we allocate in incrments of 4 and that we can handle fragmentation
//Note: there is 28 bytes not being controlled since we need a min of 40 for a control node
void test4_worst_fit() {		
	printf("Test 4 Worst Fit \n");
	worst_fit_memory_init(1000);

	worst_fit_alloc(201);
	worst_fit_alloc(202);
	worst_fit_alloc(203);
	worst_fit_alloc(203);

	print_mem_info(0);
	printf("---\n");
}

//trys to ask for too much memory
void test5_worst_fit() {
	printf("Test 5 Worst Fit \n");
	worst_fit_memory_init(1000);

	worst_fit_alloc(200);
	worst_fit_alloc(500);
	worst_fit_alloc(100);
	print_mem_info(0);
	printf("---\n");

	printf("Asking for the last block which is too much mem\n");
	worst_fit_alloc(300);

	print_mem_info(0);
	printf("---\n");
}

//allocating/deallocating memory and looking at how many fragments
void test6_worst_fit() {
	printf("Test 6 Worst Fit \n");
	worst_fit_memory_init(1000);

	worst_fit_alloc(100);
	void* b = worst_fit_alloc(150);
	worst_fit_alloc(200);	
	worst_fit_alloc(100);
	worst_fit_alloc(100);
	worst_fit_alloc(40);

	worst_fit_dealloc(b);
	worst_fit_alloc(100);

	printf("Number of free blocks with under 100 bytes: %d \n", worst_fit_count_extfrag(100));
	print_mem_info(0);
	printf("---\n");
}

void test7_worst_fit(){ // tests for invalid starting size (less than a control node)
	printf("Test 7 Worst Fit \n");
	int result = worst_fit_memory_init(30);
	printf("0 if memory init worked, -1 if it did not: %d \n", result);
	printf("---\n");
}

void test8_worst_fit(){ //allocates some memory and uses it
	printf("Test 8 Worst Fit \n");
	int result = worst_fit_memory_init(1000);
	if(result == -1){
		return;
	}
	void *a = worst_fit_alloc(10);
	*(int*)a=25; 
	printf("Should say 25: %d \n", *(int*)a);	
	printf("---\n");

	void *b = worst_fit_alloc(10);
	*(int*)b=20; 

	*(int*)a = *(int*)a + *(int*)b;
	printf("Should say 45: %d \n", *(int*)a);	
	printf("---\n");

	worst_fit_dealloc(b);
	printf("Should say 45: %d \n", *(int*)a);
	printf("---\n");
}

//Tests wherther or not we can handle being passed an incorrrect start address for deallocation
void test9_worst_fit() {
	printf("Test 9 Worst Fit \n");
	worst_fit_memory_init(1000);

	worst_fit_alloc(200);
	void* b = worst_fit_alloc(220);
	worst_fit_alloc(220);	
	worst_fit_alloc(200);
	
	printf("Allocations: \n");
	print_mem_info(0);
	printf("---\n");

	printf("Trying to deallocate a block that is not the starting address we gave\n");
	worst_fit_dealloc(b+12);
	print_mem_info(0);
	printf("---\n");

	printf("Deallocating the same block of memory by giving the correct start address\n");
	worst_fit_dealloc(b);
	print_mem_info(0);
	printf("---\n");
}

void test1_best_fit() {
	printf("Test 1 Best Fit \n");
	best_fit_memory_init(1000);

	void* a = best_fit_alloc(200);
	best_fit_alloc(220);
	void* c = best_fit_alloc(220);
	best_fit_alloc(200);

	printf("Full\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(a);
	best_fit_dealloc(c);

	printf("Freed blocks 1 and 3\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_alloc(100);
	printf("Allocated 100, should go into block 1\n");
	print_mem_info(1);
	printf("---\n");

	return;
}

void test2_best_fit() {
	printf("Test 2 Best Fit \n");
	best_fit_memory_init(1000);

	void* a = best_fit_alloc(200);
	void* b = best_fit_alloc(220);
	void* c = best_fit_alloc(220);
	void* d = best_fit_alloc(200);

	printf("Full\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(a);
	printf("Freed block 1\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(b);
	printf("Freed block 2\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(c);
	printf("Freed block 3\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(d);
	printf("Freed block 4, all free\n");
	print_mem_info(1);
	printf("---\n");

	return;
}

void test3_best_fit(){
	printf("Test 3 Best Fit \n");
	best_fit_memory_init(1000);

	void* a = best_fit_alloc(200);
	void* b = best_fit_alloc(220);
	void* c = best_fit_alloc(220);
	best_fit_alloc(200);

	printf("Full\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(a);
	printf("Freed block 1\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(c);
	printf("Freed block 3\n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(b);
	printf("Freed block 2\n");
	print_mem_info(1);
	printf("---\n");

	return;
}

void test4_best_fit(){  
	printf("Test 4 Best Fit \n");
	best_fit_memory_init(1000);

	best_fit_alloc(201);
	best_fit_alloc(203);
	best_fit_alloc(203);
	best_fit_alloc(203);

	print_mem_info(1);
	printf("---\n");
}

void test5_best_fit() {  
	printf("Test 5 Best Fit \n");
	best_fit_memory_init(1000);

	best_fit_alloc(200);
	best_fit_alloc(500);
	best_fit_alloc(100);
	print_mem_info(1);
	printf("---\n");

	printf("Asking for the last block which is too much mem \n");
	best_fit_alloc(300);

	print_mem_info(1);
	printf("---\n");
}

//allocating/deallocating memory and looking at how many fragments
void test6_best_fit() {
	printf("Test 6 Best Fit \n");
	best_fit_memory_init(1000);

	best_fit_alloc(100);
	void* b = best_fit_alloc(150);
	best_fit_alloc(200);	
	best_fit_alloc(100);
	best_fit_alloc(100);
	best_fit_alloc(40);

	best_fit_dealloc(b);
	best_fit_alloc(100);

	printf("Number of free blocks with under 100 bytes: %d \n", best_fit_count_extfrag(100));
	print_mem_info(1);
	printf("---\n");
}

void test7_best_fit(){ // tests for invalid starting size (less than a control node)
	printf("Test 7 Best Fit \n");
	int result = best_fit_memory_init(30);
	printf("0 if memory init worked, -1 if it did not. Should be -1: %d \n", result);
	printf("---\n");
}

void test8_best_fit(){ //allocates some memory and uses it
	printf("Test 8 Best Fit \n");
	int result = best_fit_memory_init(1000);
	if(result == -1){
		return;
	}
	void *a = best_fit_alloc(10);
	*(int*)a=25; 
	printf("Should say 25: %d \n", *(int*)a);	
	printf("---\n");

	void *b = best_fit_alloc(10);
	*(int*)b=20; 

	*(int*)a = *(int*)a + *(int*)b;
	printf("Should say 45: %d \n", *(int*)a);	
	printf("---\n");

	best_fit_dealloc(b);
	printf("Should say 45: %d \n", *(int*)a);
	printf("---\n");

}

//Tests wherther or not we can handle being passed an incorrrect start address for deallocation
void test9_best_fit() {
	printf("Test 9 Best Fit \n");
	best_fit_memory_init(1000);

	best_fit_alloc(200);
	void* b = best_fit_alloc(220);
	best_fit_alloc(220);	
	best_fit_alloc(200);

	printf("Allocations: \n");
	print_mem_info(1);
	printf("---\n");

	printf("Trying to deallocate a block that is not the starting address we gave\n");
	best_fit_dealloc(b+12);
	print_mem_info(1);
	printf("---\n");

	printf("Deallocating the same block of memory by giving the correct start address\n");
	best_fit_dealloc(b);
	print_mem_info(1);
	printf("---\n");
}

void worst_vs_best_fragments(){ // allocates a lot of small blocks deallocates some and allocates small spaces, makes fragments in best fit 
	printf("Test Worst Fit Vs. Best Fit Fragments: Best Fit Case \n");
	int result = best_fit_memory_init(1236);
	if(result == -1){
		return;
	}

	void *a = best_fit_alloc(52);
	best_fit_alloc(4);
	void *b=best_fit_alloc(100);
	best_fit_alloc(4);

	void *c = best_fit_alloc(52);
	best_fit_alloc(4);
	void *d=best_fit_alloc(100);
	best_fit_alloc(4);

	void *e = best_fit_alloc(52);
	best_fit_alloc(4);
	void *f=best_fit_alloc(100);
	best_fit_alloc(4);

	void *g = best_fit_alloc(52);
	best_fit_alloc(4);
	void *h=best_fit_alloc(100);
	

	printf("After allocating \n");
	print_mem_info(1);
	printf("---\n");

	best_fit_dealloc(a);
	best_fit_dealloc(b);
	best_fit_dealloc(c);
	best_fit_dealloc(d);
	best_fit_dealloc(e);
	best_fit_dealloc(f);
	best_fit_dealloc(g);
	best_fit_dealloc(h);
	
	printf("After deallocating \n");
	print_mem_info(1);
	printf("---\n");

	best_fit_alloc(8);
	best_fit_alloc(8);
	best_fit_alloc(8);
	best_fit_alloc(8);

	printf("After allocating 8\n");
	print_mem_info(1);
	printf("---\n");

	printf("Best fit fragmentation count below 8 bytes: %d \n", best_fit_count_extfrag(8));
	printf("---\n");

	printf("Test Worst Fit Vs. Best Fit Fragments: Worst Fit Case \n");
	int result2 = worst_fit_memory_init(1236);
	if(result2 == -1){
		return;
	}

	void *a2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *b2 = worst_fit_alloc(100);
	worst_fit_alloc(4);

	void *c2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *d2 = worst_fit_alloc(100);
	worst_fit_alloc(4);

	void *e2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *f2 = worst_fit_alloc(100);
	worst_fit_alloc(4);

	void *g2 = worst_fit_alloc(52);
	worst_fit_alloc(4);
	void *h2 = worst_fit_alloc(100);
	

	printf("After allocating \n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_dealloc(a2);
	worst_fit_dealloc(b2);
	worst_fit_dealloc(c2);
	worst_fit_dealloc(d2);
	worst_fit_dealloc(e2);
	worst_fit_dealloc(f2);
	worst_fit_dealloc(g2);
	worst_fit_dealloc(h2);
	
	printf("After deallocating \n");
	print_mem_info(0);
	printf("---\n");

	worst_fit_alloc(8);
	worst_fit_alloc(8);
	worst_fit_alloc(8);
	worst_fit_alloc(8);

	printf("After allocating 8 \n");
	print_mem_info(0);
	printf("---\n");

	printf("worst fit fragmentation count below 8 bytes: %d \n", worst_fit_count_extfrag(8));
	printf("---\n");

}

void best_vs_worst_large_data() {
	//create worst fit memory space with divider in middle
	worst_fit_memory_init(2124);
	void* a = worst_fit_alloc(1000);
	worst_fit_alloc(4);
	void* b = worst_fit_alloc(1000);
	worst_fit_dealloc(a);
	worst_fit_dealloc(b);

	//create best fit memory space with divider in middle
	best_fit_memory_init(2124);
	void* c = best_fit_alloc(1000);
	best_fit_alloc(4);
	void* d = best_fit_alloc(1000);
	best_fit_dealloc(c);
	best_fit_dealloc(d);

	int i;
	for(i = 0; i < 16; i++) {
		worst_fit_alloc(20);
		best_fit_alloc(20);
	}

	void* kB_worst = worst_fit_alloc(1000);
	void* kB_best = best_fit_alloc(1000);

	printf("worst kB: %lu best kB: %lu\n", (long unsigned int)kB_worst, (long unsigned int)kB_best);
}

int main(int argc, char *argv[]) {
	test1_worst_fit();
	test2_worst_fit();
	test3_worst_fit();
	test4_worst_fit();
	test5_worst_fit();
	test6_worst_fit();
	test7_worst_fit();
	test8_worst_fit();
	test9_worst_fit();

	test1_best_fit();
	test2_best_fit();
	test3_best_fit();
	test4_best_fit();
	test5_best_fit();
	test6_best_fit();
	test7_best_fit();
	test8_best_fit();
	test9_best_fit();

	worst_vs_best_fragments();
	best_vs_worst_large_data();

	return 0;
}
