/*----------------------------------------------------------------------------
 *      ECE254 Lab Task Management
 *----------------------------------------------------------------------------
 *      Name:    RT_MEMBOX_EXT.H
 *      Purpose: Interface functions for blocking 
 *               fixed memory block management system
 *      Rev.:    V4.60
 *----------------------------------------------------------------------------
 *      This code extends the RealView Run-Time Library.
 *      Created by University of Waterloo ECE254 Lab Staff.
 *---------------------------------------------------------------------------*/
/* Variables */

/* Functions */
extern void     *rt_mem_alloc (void *mem_pool);
extern OS_RESULT rt_mem_free  (void *mem_pool, void *box);

/*----------------------------------------------------------------------------
 * end of file
 *---------------------------------------------------------------------------*/

