/*************************************
* Lab 3 Exercise 1
* Name:
* Student Id: A????????
* Lab Group: B??
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/**
 * CS2106 AY 20/21 Semester 2 - Lab 3
 *
 * This file contains function definitions. Your implementation should go in
 * this file.
 */

#include "barrier.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

// Initialise barrier here
void barrier_init ( barrier_t *barrier, int count ) 
{
    //TODO: Implement the function
    barrier->count = count;
    barrier->semaphore = malloc(sizeof(sem_t));
    barrier->mutex = malloc(sizeof(sem_t));
    sem_init((barrier->semaphore), 0, 0);
    sem_init((barrier->mutex), 0, 1);
}

void barrier_wait ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_wait(barrier->mutex);
    barrier->count = barrier->count - 1;
    sem_post((barrier->mutex));

    if (barrier->count == 0) {
        sem_post(barrier->semaphore);
    }
    sem_wait(barrier->semaphore);
    sem_post(barrier->semaphore);
}

// Perform cleanup here if you need to
void barrier_destroy ( barrier_t *barrier ) 
{
    //TODO: Implement the function
    sem_destroy( barrier->mutex );
    sem_destroy(barrier->semaphore);
    free(barrier->mutex);
    free(barrier->semaphore);
}
