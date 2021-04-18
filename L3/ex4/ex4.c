/*************************************
* Lab 3 Exercise 4
* Name: Ngo Duc Binh
* Student Id: A0180061U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <semaphore.h>
#include <pthread.h>
#include <stdlib.h>
#include "traffic_synchronizer.h"

//Using extern, you can use the global variables num_of_cars and num_of_segments from 
// ex4_driver.c in your code.
extern int num_of_cars;
extern int num_of_segments;
sem_t *sems;
sem_t notFull;

void initialise()
{   
    //TODO: Your code here
    // Initialize an array of semaphore here
    sems = malloc(sizeof(sem_t) * num_of_segments);
    sem_init(&notFull, 0, num_of_segments - 1);
    for (int i = 0; i < num_of_segments; i++) {
        sem_init(&sems[i], 0, 1);
    }

}

void cleanup()
{
    //TODO: Your code here
    for (int i = 0; i < num_of_segments; i++) {
        sem_destroy(&sems[i]);
    }
    free(sems);
    sem_destroy(&notFull);
}

void* car(void* car)
{   
    //TODO: Your code here, see the general steps below
    car_struct *car1 = (car_struct*) car;
    //This function modeles a thread 
    //A car: 
    //   1. should call enter_roundabout (...)
    //   2. followed by some calls to move_to_next_segment (...)
    //   3. finally call exit_roundabout (...)

    // 1. Enter the roundabout
    sem_wait(&notFull);
    int entry_no = car1->entry_seg;
    sem_wait(&sems[entry_no]);
    enter_roundabout(car1);

    
    // 2. Deal with moving to next roundabout
    while (car1->current_seg != car1->exit_seg) {
        int seg_no = (car1->current_seg);
        int next_seg = NEXT(seg_no, num_of_segments);
        // wait for the segment to be free
        sem_wait(&sems[next_seg]);
        move_to_next_segment(car1);
        // release the mutex for the previous segment
        sem_post(&sems[seg_no]);
    }
    
    // 3. Exit the roundabout the first time it reaches the end roundabout
    exit_roundabout(car1);
    sem_post(&sems[car1->current_seg]);
    sem_post(&notFull);
    pthread_exit(NULL);
}
