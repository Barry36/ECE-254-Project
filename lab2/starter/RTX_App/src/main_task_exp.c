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

#define NUM_NAMES 8

_declare_box(mympool,12,20);
void* box;


struct func_info {
  void (*p)();      /* function pointer */
  char name[16];    /* name of the function */
};

extern void os_idle_demon(void);
__task void task0(void);
__task void task1(void);
__task void single_box_consumer_task(void);
__task void single_box_consumer_task2(void);
__task void free_and_consume_no_queue(void);
__task void watch_task(void);
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
  {task0, "task0"},   \
  {task1, "task1"},   \
	{single_box_consumer_task, "cnsmr1"},   \
	{single_box_consumer_task2, "cnsmr2"},   \
  {watch_task, "watch"},   \
	{free_and_consume_no_queue, "F&C"},   \
  {init,  "init" }
};

/*--------------------------- task0 ----------------------------------------*/
/* 							checking asfafaasfa system                         		      */
/*--------------------------------------------------------------------------*/
__task void task0(void)
{
	printf("Friebdship!!\n");
}

/*--------------------------- watch_task -----------------------------------*/
/* checking states of all tasks in the system                               */
/*--------------------------------------------------------------------------*/
__task void watch_task(void)
{
	U8 i=1;
	RL_TASK_INFO task_info;
	
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
	printf("Released box 1\n");
	
	os_dly_wait(10);
	printf("Releasing box 2 %d \n", boxes[1]);
	os_mem_free(mympool, boxes[1]);
	printf("Released box 2 \n");
	
	os_tsk_delete_self(); 
}

//attempts to allocate memory block and gets block (created with a lower priority than consumer 2)
__task void single_box_consumer_task(void){
	os_dly_wait(10);
	
	printf("consumer 1 TID: %d Attempting to get box (gets blocked) \n", os_tsk_self());
	box=os_mem_alloc(mympool);
	printf("consumer 1 TID: %d Got box %d (unblocked) \n", os_tsk_self(), box);
	
	os_tsk_delete_self(); 
}

//attempts to allocate memory block and gets block (created with a higher priority than consumer 1)
__task void single_box_consumer_task2(void){
	
	void* box2;
	
	os_dly_wait(10);
	
	printf("consumer 2 TID: %d Attempting to get box (gets blocked) \n", os_tsk_self());
	box2=os_mem_alloc(mympool);
	printf("consumer 2 TID: %d Got box %d (unblocked first as it has higher priority) \n", os_tsk_self(), box2);
	
	os_tsk_delete_self(); 
}

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
	
/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
	_init_box(mympool,sizeof(mympool),12);
	os_mut_init(&g_mut_uart);
  
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: the init TID is %d\n", os_tsk_self());
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task1, 3);  /* task 1 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with TID %d\n", g_tid);
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
	printf("init: created watch_task with TID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task0, 1);  /* task 0 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with TID %d\n", g_tid);
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
