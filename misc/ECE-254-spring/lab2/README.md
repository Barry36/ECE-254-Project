# Lab 2 - Real-Time Memory Allocation
By: Michael Rokas & Luke Seewald

## Description
The goal of this lab is to create an ARM RL-RTX library for the Cortex M3, and implement the functions <b>rt_mem_alloc</b> to allocate a block in a given mempool
and <b>rt_mem_free</b> to free a block in a given mempool. Both of these functions can be found in the [rt_MemBox_ext.c](starter/RTX_CM3/SRC/CM/rt_MemBox_ext.c) file.

The project was built with Keil uVision 4 and runs on the Cortex M3.

## Test Tasks
The test tasks used to test the added functions can be found in the [main_task_exp.c](starter/RTX_App/src/main_task_exp.c) file and their functionality behaves as follows:

* <b>Task1:</b> A task to allocate all 20 blocks of a mempool and then release them for the two consumer tasks
* <b>Single Box Consumer Task:</b> Attempts to allocate a memory block and gets blocked until one is available (lower priority than single_box_consumer_task2)
* <b>Single Box Consumer Task2:</b> Attempts to allocate a memory block and gets blocked until one is available (higher priority than single_box_consumer_task)
* <b>Free and Consume No Queue:</b> Frees a memory block with no other tasks in the queue and immediatly re-allocates it
* <b>Watch Task:</b> Prints the states of the other active tasks
