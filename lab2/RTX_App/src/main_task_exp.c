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
_declare_box(mempool,12,20);

void* box;

struct func_info {
  void (*p)();      /* function pointer */
  char name[16];    /* name of the function */
};

extern void os_idle_demon(void);
__task void task1(void);
__task void task2(void);
__task void task3(void);
__task void task4(void);
__task void task5(void);
__task void task6(void);
__task void init (void);
 
extern char *state2str(unsigned char state, char *str);
char *fp2name(void (*p)(), char *str);

OS_MUT g_mut_uart;
OS_MUT g_mut_evnt;
OS_TID g_tid = 255;

int  g_counter = 0;  // a global counter
char g_str[16];
char g_tsk_name[16];

struct func_info g_task_map[NUM_NAMES] = \
{
  /* os_idle_demon function ptr to be initialized in main */
  {NULL,  "os_idle_demon"}, \
  {task1, "task1"},   \
  {task2, "task2"},   \
  {task3, "task3"},	  \
  {task4, "task4"},   \
  {task5, "task5"},	  \
  {task6, "task6"},	  \
  {init,  "init" },		

};

/* no local variables defined, use one global var */

/*--------------------------- task1 -----------------------------------*/
/* os_mem_alloc gets blocked until memory is available                 */
/*---------------------------------------------------------------------*/
__task void task1(void)
{
	void* box3;
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task1 ID: %d, Attempting to get box (no memory available, gets blocked) \n", os_tsk_self());
	os_mut_release(g_mut_uart);
	box3 = os_mem_alloc(mempool);
	
	os_dly_wait(5);
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task1 ID: %d Got box %d (unblocked) \n", os_tsk_self(), box3);
	os_mut_release(g_mut_uart);
	
	os_tsk_delete_self(); 
}

/*--------------------------- task2 -----------------------------------*/
/* checking states of all tasks in the system                          */
/*---------------------------------------------------------------------*/
__task void task2(void)
{
	U8 i=1;
	RL_TASK_INFO task_info;
	
  for(;;) {
		os_mut_wait(g_mut_uart, 0xFFFF);
		printf("\nTID\tNAME\t\tPRIO\tSTATE\n");
		os_mut_release(g_mut_uart);
			
		for(i = 0; i <3; i++) { // this is a lazy way of doing loop.
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



/*--------------------------- task3 -----------------------------------*/
/* allocate 20 boxes and free two of them for the task4 and task5      */
/*---------------------------------------------------------------------*/
__task void task3(void){
	void** boxes[20];
	int i = 0;
	os_dly_wait(5);
	
	// allocate 20 memory boxes
	for(i; i < 20; i++){
		boxes[i] = os_mem_alloc(mempool);
		os_mut_wait(g_mut_uart, 0xFFFF);
		printf("Task3 ID: %d and assigned memory box: %d \n", os_tsk_self(), i);
		os_mut_release(g_mut_uart);
	}
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task3 ID: %d and all boxes are allocated to this task \n", os_tsk_self());
	os_mut_release(g_mut_uart);
	
	// free boxes, one at a time	
	os_dly_wait(15);
	printf("Releasing box 1 %d \n", boxes[0]);
	os_mem_free(mempool, boxes[0]);
	printf("Released box 1\n");
	
	os_dly_wait(10);
	printf("Releasing box 2 %d \n", boxes[1]);
	os_mem_free(mempool, boxes[1]);
	printf("Released box 2 \n");
	
	os_tsk_delete_self(); 
}


/*--------------------------- task4 -----------------------------------*/
/* Allocate memory blocks (created with a lower priority than task5)   */
/*---------------------------------------------------------------------*/
__task void task4(void){
	
	os_dly_wait(10);
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task4 ID: %d, Attempting to get box (gets blocked) \n", os_tsk_self());
	os_mut_release(g_mut_uart);
	box = os_mem_alloc(mempool);
	
//	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task4 ID: %d Got box %d (unblocked) \n", os_tsk_self(), box);
//	os_mut_release(g_mut_uart);
	
	os_tsk_delete_self(); 
}


/*--------------------------- task5 -----------------------------------*/
/* Allocate memory block and gets block (higher priority than task4)	 */
/*---------------------------------------------------------------------*/
__task void task5(void){
	
	void* box2;
	
	os_dly_wait(10);
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task5 ID: %d, Attempting to get box (gets blocked) \n", os_tsk_self());
	os_mut_release(g_mut_uart);
	box2 = os_mem_alloc(mempool);
//	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task5 ID: %d Got box %d (unblocked first since it has higher priority) \n", os_tsk_self(), box2);
//	os_mut_release(g_mut_uart);
	
	os_tsk_delete_self(); 
}

/*--------------------------- task6 ------------------------------------------------*/
/* Free a block with no other task in the queue and then re-allocate the same block */
/*----------------------------------------------------------------------------------*/
__task void task6(void){
	
	os_dly_wait(50);
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task6 ID: %d, Freeing one box %d \n", os_tsk_self(), box);
	os_mut_release(g_mut_uart);
	os_mem_free(mempool, box);
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task6 ID: %d, consuming one box %d with no tasks waiting in queue \n", os_tsk_self(), box);
	os_mut_release(g_mut_uart);
	box = os_mem_alloc(mempool);
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("Task6 ID: %d now has box %d \n", os_tsk_self(), box);
	os_mut_release(g_mut_uart);
	
	os_tsk_delete_self();
}


/*--------------------------- init ------------------------------------*/
/* initialize system resources and create other tasks                  */
/*---------------------------------------------------------------------*/
__task void init(void)
{
	os_mut_init(&g_mut_uart);
	os_mut_init(&g_mut_evnt);
	_init_box(mempool, sizeof(mempool), 12);
	
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: the init Task ID is %d\n", os_tsk_self());
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task1, 5);  /* task 1 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task1 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);
  
	g_tid = os_tsk_create(task2, 4);  /* task 2 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task2 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task3, 3);  /* task 3 at priority 3 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task3 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);

	g_tid = os_tsk_create(task4, 1);  /* task4 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task4 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);
	
	g_tid = os_tsk_create(task5, 2);  /* task5 at priority 2 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task5 with Task ID %d\n", g_tid);
	os_mut_release(g_mut_uart);

	g_tid = os_tsk_create(task6, 1);  /* task6 at priority 1 */
	os_mut_wait(g_mut_uart, 0xFFFF);
	printf("init: created task6 with Task ID %d\n", g_tid);
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

// int main(void)
// {
// 	SystemInit();         /* initialize the LPC17xx MCU */
// 	uart0_init();         /* initialize the first UART */
//   
//   
// 	/* fill the fname map with os_idle_demon entry point */
// 	g_task_map[0].p = os_idle_demon;
//   
// 	printf("========================================================\n");
// 	printf("==                  NEW RUN                           ==\n");
// 	printf("========================================================\n");
// 	printf("Calling os_sys_init()...\n");
// 	os_sys_init(init);    /* initialize the OS and start the first task */
// }
