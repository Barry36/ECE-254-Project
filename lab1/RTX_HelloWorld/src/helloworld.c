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

OS_TID test_task_1;
OS_TID test_task_2;
OS_TID test_task_3;
OS_TID test_task_4;
OS_TID test_task_5;





__task void task1()
{
while(1){
RL_TASK_INFO test2;
RL_TASK_INFO test3;
os_tsk_get(test_task_2,&test2);
os_tsk_get(test_task_3,&test3);
printf("Task2 state: %d\n", test2.state);
printf("Task3 stack_usage: %d\n", test3.stack_usage);
}
}

__task void task2()
{
while(1){
printf("This is q...\n");
os_dly_wait(10);
}
}


__task void testTask()
{
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
printf("Number tasks: %d\n", os_tsk_count_get());
os_dly_wait(30);
}
}

__task void task5() {
while(1){

RL_TASK_INFO test3;

os_tsk_get(test_task_3,&test3);

printf("Task3 state: %d \n", test3.state);

printf("Task3 prio: %d \n", test3.prio);

printf("Task3 task id: %d \n", test3.task_id);

printf("Task3 stack usage: %d \n", test3.stack_usage);

printf("Task3 ptask: %d \n", test3.ptask);

os_dly_wait(30);
}
}

__task void init(void)
{


test_task_1 = os_tsk_create(task1, 1); 
test_task_2 = os_tsk_create(task2, 1); 
test_task_3 = os_tsk_create(testTask, 1); 
test_task_4 = os_tsk_create(task4, 1);
test_task_5 = os_tsk_create(task5, 1);

// Delete before exit, this is very important!!
os_tsk_delete_self(); 
}
int main ()

{
SystemInit();
uart0_init();
os_sys_init(init);
}

