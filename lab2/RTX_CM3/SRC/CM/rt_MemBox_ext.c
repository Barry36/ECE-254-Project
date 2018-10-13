/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_ext.C
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code is extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
 
/*----------------------------------------------------------------------------
 *      Includes
 *---------------------------------------------------------------------------*/
 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_MemBox.h"
#include "rt_HAL_CM.h"
#include "rt_Task.h"       /* added in ECE254 lab keil_proc */ 
#include "rt_MemBox_ext.h" /* added in ECE254 lab keil_proc */   

/* ECE254 Lab Comment: You may need to include more header files */
#include "rt_List.h"


/*----------------------------------------------------------------------------
 *      Global Variables
 *---------------------------------------------------------------------------*/
struct OS_XCB queue;

/*----------------------------------------------------------------------------
 *      Global Functions
 *---------------------------------------------------------------------------*/

/*==========================================================================*/
/*  The following are added for ECE254 Lab Task Management Assignmet       */
/*==========================================================================*/

/**  
 * os_mem_alloc(mem_pool) calls this function.
 * @brief: Request a block of memory from the memory pool and block the calling
 * 			task if no memory is available in the memory pool
 */
void *rt_mem_alloc (void *mem_pool) {
	/* Add your own code here. Change the following line accordingly */

	void* block_ptr;
	P_TCB task;

	// Allocate a block of memory to the task
	if(mem_pool == NULL){
		return NULL;
	}
	block_ptr = rt_alloc_box(mem_pool);
	
	if(block_ptr != NULL){
		printf("************block_ptr is NULL \n***********");
	}else{
		printf("***********block_ptr is not NULL \n***********");
	}
	// Return the pointer pointing to the allocated memory block 
	// if we can successfully allocate a block of memory for the given task

	// Otherwise we block the task until there is an available block of memory
	if(block_ptr != NULL){
		return block_ptr;
	}else {
		task = os_tsk.run;
		rt_put_prio(&queue, task);
		rt_block(0xffff, WAIT_MEM);
	}
	
	return NULL;
}


/**
 * os_mem_free(mem_pool, box) calls this function.
 * free the memory pointed by box, it will unblock any task that might be 
 * 		waiting for memory.
 * @return: OS_R_OK on success and OS_R_NOK if box does not belong to mem_pool 
 */
OS_RESULT rt_mem_free (void *mem_pool, void *box) {
	/* Add your own code here. Change the following line accordingly */
	if (box == NULL){
		return OS_R_NOK;
	}
	
	// if no tasks are waiting for a box then free the box
	// Otherwise assign the first task in the priority queue to the box

	if(queue.p_lnk == NULL) {
		rt_free_box(mem_pool, box);
	}else { 
		P_TCB task = rt_get_first(&queue);

		// ret_val is the return value upon completion of a wait 
		
		task -> ret_val = (U32)box;
		rt_dispatch(task);
	}
		
	return (OS_R_OK);
}
/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/
