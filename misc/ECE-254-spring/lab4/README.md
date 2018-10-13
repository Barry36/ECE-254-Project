# Lab 4 - Memory Allocation 
By: Michael Rokas & Luke Seewald

## Description
The goal of this lab is to create two memory allocation systems using both best fit and worst fit algorithms. The implementation is contained
in [mem.c](mem.c). To use these algorithms the user must first initialize them by calling the init function for the desired algorithm type, this will
allocate an initial memory space using ```malloc``` which will be managed by the defined memory allocation algorithm. The user can then freely request and release
memory as needed using the respective calls.

These algorthms both manage memory using a doubly linked list approach where information about the memory block is stored in its respective node which is also
stored within the initial allocated memory space. There are various pros and cons to this approach in respects to both speed and memory usage which are further
discussed in the [lab report](report.pdf).

## How to build and run test cases
To build the test program simply run the ```make``` command.

To run the test program simply call: ```./main test.out```

The specific test cases are both commented an described in the [lab report](report).

Alternatively the [mem.c](mem.c) file and its respecteive header can be used in other programs by using the function calls as described below.

## Function Descriptions

```print_mem_info(int type)```: A utility function to print info on the current memory space, pass 0 to print info on the 
worst fit address space and 1 to see info on the best fit address space.

```int best_fit_memory_init(size_t size)```: Creates the initial memory space of the passed size to be allocated by the best fit algorithm.

```int worst_fit_memory_init(size_t size)```: Creates the initial memory space of the passed size to be allocated by the worst fit algorithm.

```void *best_fit_alloc(size_t size)```: Returns a pointer to a memory block of the requested size rounded up to the nearest increment of 4
using the best fit algorithm.

```void *worst_fit_alloc(size_t size)```: Returns a pointer to a memory block of the requested size rounded up to the nearest increment of 4
using the worst fit algorithm.

```void best_fit_dealloc(void *ptr)```: Deallocates the given memory provided it was allocated with the best fit algorithm.

```void worst_fit_dealloc(void *ptr)```: Deallocates the given memory provided it was allocated with the worst fit algorithm.

```int best_fit_count_extfrag(size_t size)```: A utility function to return the number of spaces in the best fit memory lower than passed size.

```int worst_fit_count_extfrag(size_t size)```: A utility function to return the number of spaces in the best fit memory lower than passed size.
