/**
* @file: helloworld.c
* @brief: Two simple tasks running pseduo-parallelly
*/
#include <LPC17xx.h>
#include <RTL.h>
#include <stdio.h>
#include "uart_polling.h"
#include "../../RTX_CM3/INC/RTL_ext.h"
#include <stdlib.h>

OS_TID tsk1;
OS_TID tsk2;
OS_TID tsk3;
OS_TID tsk4;
OS_TID tsk5;

__task void task1() {
	while(1){
		RL_TASK_INFO test2;
		RL_TASK_INFO test3;
	
		os_tsk_get(tsk2,&test2);
		os_tsk_get(tsk3,&test3);
	
		printf("Task2 state: %d\n", test2.state);
		printf("Task3 stack_usage: %d\n", test3.stack_usage);
	}
}

__task void task2() {
	while(1){
		printf("I wait for long periods\n");
		os_dly_wait(1000);
	}
}

__task void task3() {
	// Fill stack with identifying values
	U8 t[440];
	U32 i = 0;
	// Fill the stack with 0xAA's (see in Memory window of debugger)
	t[0] = 0xAA;
	for(i=1;i<sizeof(t)/sizeof(U8);i++) {
		t[i] = t[i-1];
	}
	// Keep the task running forever
	for(i=0;; i++){}
}

__task void task4() {
	while(1){
		printf("Num tasks: %d\n", os_tsk_count_get());
		os_dly_wait(300);
	}
}

__task void task5() {
	while(1){
		RL_TASK_INFO test1;
		RL_TASK_INFO test2;
		RL_TASK_INFO test3;
		RL_TASK_INFO test4;
		RL_TASK_INFO test5;
	
		os_tsk_get(tsk1,&test1);
		os_tsk_get(tsk2,&test2);
		os_tsk_get(tsk3,&test3);
		os_tsk_get(tsk4,&test4);
		os_tsk_get(tsk5,&test5);
	
		printf("Task1 state: %d \n", test1.state);
		printf("Task2 state: %d \n", test2.state);
		printf("Task3 state: %d \n", test3.state);
		printf("Task4 state: %d \n", test4.state);
		printf("Task5 state: %d \n", test5.state);
		
		printf("Task1 prio: %d \n", test1.prio);
		printf("Task2 prio: %d \n", test2.prio);
		printf("Task3 prio: %d \n", test3.prio);
		printf("Task4 prio: %d \n", test4.prio);
		printf("Task5 prio: %d \n", test5.prio);
		
		printf("Task1 task id: %d \n", test1.task_id);
		printf("Task2 task id: %d \n", test2.task_id);
		printf("Task3 task id: %d \n", test3.task_id);
		printf("Task4 task id: %d \n", test4.task_id);
		printf("Task5 task id: %d \n", test5.task_id);
		
		printf("Task1 stack usage: %d \n", test1.stack_usage);
		printf("Task2 stack usage: %d \n", test2.stack_usage);
		printf("Task3 stack usage: %d \n", test3.stack_usage);
		printf("Task4 stack usage: %d \n", test4.stack_usage);
		printf("Task5 stack usage: %d \n", test5.stack_usage);
		
		printf("Task1 ptask: %d \n", test1.ptask);
		printf("Task2 ptask: %d \n", test2.ptask);
		printf("Task3 ptask: %d \n", test3.ptask);
		printf("Task4 ptask: %d \n", test4.ptask);
		printf("Task5 ptask: %d \n", test5.ptask);
		
		os_dly_wait(300);
	}
}

__task void init(void)
{

	tsk1=os_tsk_create(task1, 1); // task1 at priority 1
	tsk2=os_tsk_create(task2, 1); // task2 at priority 1
	tsk3=os_tsk_create(task3, 1); // task3 at priority 1
	tsk4=os_tsk_create(task4, 1);
	tsk5=os_tsk_create(task5, 1);
	
	os_tsk_delete_self(); // must delete itself before exiting
}
int main ()

{
	SystemInit();
	uart0_init();
	os_sys_init(init);
}