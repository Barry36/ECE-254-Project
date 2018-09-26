/**
 * @file:   rt_Task_ext.c
 */
// Modified by Jianqing Wang @ Sep 26th 
#include "rt_TypeDef.h"
#include "RTX_Config.h"
#include "rt_System.h"
#include "rt_Task.h"
#include "rt_List.h"
#include "rt_Robin.h"
#include "rt_HAL_CM.h"
#include "rt_Task_ext.h"
#include <stdio.h>

/*----------------------- rt_tsk_count_get --------------------------*/
/* added for ECE254 lab */
int rt_tsk_count_get (void) {
	// This function counts the number of active tasks
	// add your own code here
	int counter = 0;
	U32 tid;
	P_TCB p_tcb;

  	for (tid = 1; tid <= os_maxtaskrun; tid++) {
		p_tcb = os_active_TCB[tid - 1];
    		if (p_tcb != NULL && p_tcb -> state != INACTIVE){
      			++counter;
    		}
  	}
	
	// Count idle TCB
	if(os_idle_TCB.state != INACTIVE){
		++counter;
	}
	return counter;
}

/*----------------------- rt_tsk_get --------------------------------*/
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
	// add your own code here

	// top_stack is the top of the stack of a particular the task
	// top_task is the top of the task stack (R13)
	// stack_size = (U16)os_stackinfo;	
	U32 stack_size, top_stack, top_task, used;
	
	// If task is active the proceed
	if(os_active_TCB[task_id - 1] != NULL){

		P_TCB task_active = os_active_TCB[task_id-1];
			
		stack_size = task_active -> priv_stack;
		if (stack_size == 0) {
			stack_size = (U16)os_stackinfo;
		}

		// If state is not RUNNING, set top_task to the current task stack pointer (R13)
		// If it's running, then get the address of the last item from the stack.
		if(task_active -> state != RUNNING){	
			top_task = (U32)task_active -> tsk_stack;				
		}
		else{
			top_task = (U32)rt_get_PSP();
		}
			
		top_stack = (U32)task_active -> stack;
		used = stack_size - (top_task - top_stack);
			
		p_task_info -> stack_usage = (100 * 2 * used) / stack_size;
		p_task_info -> task_id = task_id;
		p_task_info -> state = task_active -> state;
		p_task_info -> prio = task_active -> prio;
		p_task_info -> ptask = task_active -> ptask;		
			
		return OS_R_OK;
	}	
	return OS_R_NOK;
}
/* end of file */
 