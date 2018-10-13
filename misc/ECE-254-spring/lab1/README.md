# Lab 1 - Real-Time Tasks
By: Michael Rokas & Luke Seewald

## Description
The goal of this lab is to create an ARM RL-RTX library for the Cortex M3, and implement the functions <b>rl_tsk_count_get</b> to return the number of active tasks and <b>rl_tsk_get</b> to return info about a desired task. Both of these functions can be found in the [rt_Task_ext.c](starter/RTX_CM3/SRC/CM/rt_Task_ext.c) file.

The project was built with Keil uVision 4 and runs on the Cortex M3.

## Test Tasks
The test tasks used to test the added functions can be found in the [helloworld.c](starter/HelloWorld/src/helloworld.c) file and their functionality behaves as follows:

* <b>Task1:</b> A task to print the results of Task2 and Task3
* <b>Task2:</b> A task that waits for long periods to test the state
* <b>Task3:</b> A task to use large amounts of the stack (98-99%)
* <b>Task4:</b> A task to print the number of tasks
* <b>Task5:</b> A task to print the info of all the other tasks
