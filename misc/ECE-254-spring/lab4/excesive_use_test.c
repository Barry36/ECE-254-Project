#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "mem.h"

int total_worst_fail;
int total_best_fail;
int total_worst_frag;
int total_best_frag;

void excesive_use(){
	void* addresses1[1000];
	void* addresses2[1000];

	best_fit_memory_init(100000);
	worst_fit_memory_init(100000);

	srand(time(NULL));

	int worst_failed = 0;
	int best_failed = 0;

	int i;
	for(i = 0; i <= 1000; i++) {
		int alloc_dealloc = rand()%2;

		if(alloc_dealloc == 0){
			int index = rand()%1000; 
			worst_fit_dealloc(addresses1[index]);
			best_fit_dealloc(addresses2[index]);
			addresses1[index] = 0;
			addresses2[index] = 0;
		} else {
			int size = rand()%1000 + 1;
			
			int empty_address1_index = -1;
			int j;
			for(j = 0; j < 1000; j++) {
				if(addresses1[j] == 0) {
					empty_address1_index = j;
					break;
				}
			}

			if(empty_address1_index != -1) {
				addresses1[empty_address1_index] = worst_fit_alloc(size);
				if(addresses1[empty_address1_index] == 0) {
					//printf("wrst failed %d\n", size);
					worst_failed++;
				}
			}

			int empty_address2_index = -1;
			for(j = 0; j < 1000; j++) {
				if(addresses2[j] == 0) {
					empty_address2_index = j;
					break;
				}
			}

			if(empty_address2_index != -1) {
				addresses2[empty_address2_index] = best_fit_alloc(size);
				if(addresses2[empty_address2_index] == 0) {
					//printf("bst failed %d\n", size);
					best_failed++;
				}
			}
		}
	}

	total_worst_fail += worst_failed;
	total_best_fail += best_failed;

	total_worst_frag += worst_fit_count_extfrag(16);
	total_best_frag += best_fit_count_extfrag(16);

	//printf("worst failed: %d best failed: %d\n", worst_failed, best_failed);
}

int main(int argc, char *argv[]) {
	int i;
	for(i = 0; i < 500; i++) {
		excesive_use();
	}

	printf("avg -- wrst fail: %d best fail: %d worst frag: %d best frag: %d\n",
			total_worst_fail/1000, total_best_fail/1000, total_worst_frag/1000, total_best_frag/1000);

	return 0;
}