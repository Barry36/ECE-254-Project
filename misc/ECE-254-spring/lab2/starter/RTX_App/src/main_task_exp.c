/**
 * @brief: ECE254 Keil ARM RL-RTX Task Management Lab starter file that calls os_tsk_get()
 * @file: main_task_exp.c
 * @date: 2015/09/13
 */
/* NOTE: This release is for students to use */

#include <LPC17xx.h>
#include "uart_polling.h" 
#include <RTL.h>
#include "../../RTX_CM3/INC/RTL_ext.h" /* extended interface header file */
#include <stdio.h>
#include <string.h>

<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
#define NUM_NAMES 7

_declare_box(mympool,12,20);
void* box;

=======
#define NUM_NAMES 8
_declare_box(mempool,12,20);

void* box;
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c

struct func_info {
  void (*p)();      /* function pointer */
  char name[16];    /* name of the function */
};

extern void os_idle_demon(void);
__task void task1(void);
<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
__task void single_box_consumer_task(void);
__task void single_box_consumer_task2(void);
__task void free_and_consume_no_queue(void);
__task void watch_task(void);
=======
__task void task2(void);
__task void task3(void);
__task void task4(void);
__task void task5(void);
__task void task6(void);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
__task void init (void);
 
extern char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_TID g_tid = 255;

int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

struct func_info g_task_map[NUM_NAMES] = \
{
	{NULL,  "os_idle_demon"},
  {task1, "task1"},   \
<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
	{single_box_consumer_task, "cnsmr1"},   \
	{single_box_consumer_task2, "cnsmr2"},   \
  {watch_task, "watch"},   \
	{free_and_consume_no_queue, "F&C"},   \
  {init,  "init" }
};

/*--------------------------- watch_task -----------------------------------*/
/* checking states of all tasks in the system                               */
/*--------------------------------------------------------------------------*/
__task void watch_task(void)
{
	U8 i=1;
	RL_TASK_INFO task_info;
=======
  {task2, "task2"},   \
  {task3, "task3"},	  \
  {task4, "task4"},   \
  {task5, "task5"},	  \
  {task6, "task6"},	  \
  {init,  "init" },		

};

/* no local variables defined, use one global var */
/*--------------------------- task1 -----------------------------------*/
__task void task1(void)
{
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task1: I am wating for task 2 to finish\n");
	os_mut_release(g_mut_uart);
	
	os_dly_wait(2);
	
	/****/os_mut_wait(g_mut_evnt, 0xFFFF);
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task1: I am Doing Stuff\n");
	os_mut_release(g_mut_uart);
	
	/****/os_mut_release(g_mut_evnt);
	
	
	os_mem_alloc(NULL);
	os_mem_free(NULL,NULL);
	for (;;) {
		g_counter++;
	}
}


/*--------------------------- task2 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/
__task void task2(void)
{
	U8 i=1;
	RL_TASK_INFO task_info;
	/****/os_mut_wait(g_mut_evnt, 0xFFFF);
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task2: I am Doing Stuff that must occur before Task 1 runs\n");
	os_mut_release(g_mut_uart);
	
	/****/os_mut_release(g_mut_evnt);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	
  for(;;) {
		os_mut_wait(g_mut_uart, 0xFFFF);
		printf("\nTID\tNAME\t\tPRIO\tSTATE\n");
		os_mut_release(g_mut_uart);
			
		for(i = 0; i <NUM_NAMES; i++) {
			if (os_tsk_get(i+1, &task_info) == OS_R_OK) {
				os_mut_wait(g_mut_uart, 0xFFFF);  
				printf("%d\t%s\t\t%d\t%s\n", \
							 task_info.task_id, \
							 fp2name(task_info.ptask, g_tsk_name), \
							 task_info.prio, \
							 state2str(task_info.state, g_str));
				os_mut_release(g_mut_uart);
			} 
		}
			
		if (os_tsk_get(0xFF, &task_info) == OS_R_OK) {
			os_mut_wait(g_mut_uart, 0xFFFF);  
			printf("%d\t%s\t\t%d\t%s\n", \
						 task_info.task_id, \
						 fp2name(task_info.ptask, g_tsk_name), \
						 task_info.prio, \
						 state2str(task_info.state, g_str));
			os_mut_release(g_mut_uart);
		} 
		#ifdef MYSIM
		os_dly_wait(2);
		#else
		os_dly_wait(200);
		#endif
	}
}

<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
//allocate all 20 blocks and then free them for the consumers
__task void task1(void){
	void** boxes[20];
	int i = 0;
	
	//allocate all 20 memory blocks
	for(; i<20; ++i){
		boxes[i]=os_mem_alloc(mympool);
		printf("TID: %d Assigning memory box: %d \n", os_tsk_self(), i);
	}
	printf("TID: %d ALL boxes taken \n", os_tsk_self());
	
	//free one at a time for the consumers
	
	os_dly_wait(15);
	printf("Releasing box 1 %d \n", boxes[0]);
	os_mem_free(mympool, boxes[0]);
=======


/*--------------------------- task3 -----------------------------------*/
/* allocate 20 boxes and free two of them for the task4 and task5      */
/*---------------------------------------------------------------------*/

__task void task3(void){
	void** boxes[20];
	int i = 0;
	
	// allocate 20 memory boxes
	for(i; i < 20; i++){
		boxes[i] = os_mem_alloc(mempool);
		printf("Task ID: %d and assigned memory box: %d \n", os_tsk_self(), i);
	}
	printf("Task ID: %d and all boxes are allocated to this task \n", os_tsk_self());
	
	// free boxes, one at a time
	
	os_dly_wait(15);
	printf("Releasing box 1 %d \n", boxes[0]);
	os_mem_free(mempool, boxes[0]);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	printf("Released box 1\n");
	
	os_dly_wait(10);
	printf("Releasing box 2 %d \n", boxes[1]);
<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
	os_mem_free(mympool, boxes[1]);
=======
	os_mem_free(mempool, boxes[1]);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	printf("Released box 2 \n");
	
	os_tsk_delete_self(); 
}

<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
//attempts to allocate memory block and gets block (created with a lower priority than consumer 2)
__task void single_box_consumer_task(void){
	os_dly_wait(10);
	
	printf("consumer 1 TID: %d Attempting to get box (gets blocked) \n", os_tsk_self());
	box=os_mem_alloc(mympool);
	printf("consumer 1 TID: %d Got box %d (unblocked) \n", os_tsk_self(), box);
=======

/*--------------------------- task4 -----------------------------------*/
/* Allocate memory blocks (created with a lower priority than task5) */
/*---------------------------------------------------------------------*/
__task void task4(void){
	
	os_dly_wait(10);
	printf("Task ID: %d, Attempting to get box (gets blocked) \n", os_tsk_self());
	box = os_mem_alloc(mempool);
	printf("Task ID: %d Got box %d (unblocked) \n", os_tsk_self(), box);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	
	os_tsk_delete_self(); 
}

<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
//attempts to allocate memory block and gets block (created with a higher priority than consumer 1)
__task void single_box_consumer_task2(void){
=======

/*--------------------------- task5 -----------------------------------*/
/* Allocate memory block and gets block (created with a higher priority than task4)
/*---------------------------------------------------------------------*/
__task void task5(void){
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	
	void* box2;
	
	os_dly_wait(10);
<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
	
	printf("consumer 2 TID: %d Attempting to get box (gets blocked) \n", os_tsk_self());
	box2=os_mem_alloc(mympool);
	printf("consumer 2 TID: %d Got box %d (unblocked first as it has higher priority) \n", os_tsk_self(), box2);
=======
	printf("Task ID: %d, Attempting to get box (gets blocked) \n", os_tsk_self());
	box2 = os_mem_alloc(mempool);
	printf("Task ID: %d Got box %d (unblocked first since it has higher priority) \n", os_tsk_self(), box2);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	
	os_tsk_delete_self(); 
}

<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
//frees a block with no other task in the queue and then re-allocates the same block
__task void free_and_consume_no_queue(void){
	
	os_dly_wait(50);
	printf("task 5  TID: %d freeing one box %d \n", os_tsk_self(), box);
	os_mem_free(mympool, box);
	
	printf("task 5 TID: %d consuming one box %d with no tasks waiting in queue \n", os_tsk_self(), box);
	box=os_mem_alloc(mympool);
	printf("task 5 TID: %d now has box %d \n", os_tsk_self(), box);
	
	os_tsk_delete_self();
}
	
=======
/*--------------------------- task6 -----------------------------------*/
/* Free a block with no other task in the queue and then re-allocate the same block
/*---------------------------------------------------------------------*/
__task void task6(void){
	
	os_dly_wait(50);
	printf("task 6 Task ID: %d, Freeing one box %d \n", os_tsk_self(), box);
	os_mem_free(mempool, box);
	
	printf("task 6 Task ID: %d, consuming one box %d with no tasks waiting in queue \n", os_tsk_self(), box);
	box = os_mem_alloc(mempool);
	printf("task 6 Task ID: %d now has box %d \n", os_tsk_self(), box);
	
	os_tsk_delete_self();
}



>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
	_init_box(mympool,sizeof(mympool),12);
	os_mut_init(&g_mut_uart);
  
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: the init Task ID is %d\n", os_tsk_self());
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task1, 3);  /* task 1 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(single_box_consumer_task, 1);  /* consumer task at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created single_box_consumer_task with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(single_box_consumer_task2, 2);  /* consumer task at priority 2 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created single_box_consumer_task2 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(free_and_consume_no_queue, 1);  /* free and consume task at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created free_and_consumer_no_queue_task5 with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(watch_task, 4);  /* watch task at priority 4 */
	os_mut_wait(g_mut_uart, 0xFFFF);
<<<<<<< HEAD:misc/ECE-254-spring/lab2/starter/RTX_App/src/main_task_exp.c
	printf("init: created watch_task with TID %d\n", g_tid);
=======
	printf("init: created task2 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task3, 1);  /* task 3 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task3 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);

	g_tid = os_tsk_create(task4, 1);  /* task4 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("initi: created task4 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task5, 2);  /* task5 at priority 2 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("initi: created task5 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);


	g_tid = os_tsk_create(task6, 1);  /* task6 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("initi: created task6 with Task ID %d\n", g_tid);
>>>>>>> upstream/master:lab2/starter/RTX_App/src/main_task_exp.c
	os_mut_release(g_mut_uart);

  
	os_tsk_delete_self();     /* task MUST delete itself before exiting */
}

/** 
 * @brief: get function name by function pointer
 * @param: p the entry point of a function (i.e. function pointer)
 * @param: str the buffer to return the function name
 * @return: the function name string starting address
 */
char *fp2name(void (*p)(), char *str)
{
	int i;
  
	for ( i = 0; i < NUM_NAMES; i++) {
		if (g_task_map[i].p == p) {
			str = strcpy(str, g_task_map[i].name);  
			return str;
		}
	}
	strcpy(str, "ghost");
	return str;
}

int main(void)
{
	SystemInit();         /* initialize the LPC17xx MCU */
	uart0_init();         /* initialize the first UART */
  
  
	/* fill the fname map with os_idle_demon entry point */
	g_task_map[0].p = os_idle_demon;
  
	printf("Calling os_sys_init()...\n");
	os_sys_init(init);    /* initialize the OS and start the first task */
}
