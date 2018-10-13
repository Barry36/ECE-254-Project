/**
 * @file:   rt_Task_ext.c
 */
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
	// add your own code here
	int count=0;
	U32 tid;
	P_TCB p_task_test;
  	for (tid = 1; tid <= os_maxtaskrun; tid++) {
		p_task_test=os_active_TCB[tid-1];
    		if (p_task_test!= NULL && p_task_test->state != INACTIVE){
      			++count;
    		}
  	}
	
	if(os_idle_TCB.state != INACTIVE){
		++count;
	}
	return count;
}

/*----------------------- rt_tsk_get --------------------------------*/
OS_RESULT rt_tsk_get (OS_TID task_id, RL_TASK_INFO *p_task_info) {
	// add your own code here
	U32 size;
	U32 top;
	U32 top_task;
	U32 used;
	
	if(os_active_TCB[task_id - 1] != NULL){
		P_TCB active_task=os_active_TCB[task_id-1];
			
		size = active_task->priv_stack;
		if (size == 0) {
			size = (U16)os_stackinfo;
		}
			
		if(active_task->state!=RUNNING){	
			top_task=(U32)active_task->tsk_stack;				
		}
		else{
			top_task=(U32)rt_get_PSP();
		}
			
		top=(U32)active_task->stack;
		used=size-(top_task-top);
			
		p_task_info->stack_usage=(100*used)/size;
		p_task_info->task_id=task_id;
		p_task_info->state=active_task->state;
		p_task_info->prio=active_task->prio;
		p_task_info->ptask=active_task->ptask;		
			
		return OS_R_OK;
	}	
	return OS_R_NOK;
}
/* end of file */
