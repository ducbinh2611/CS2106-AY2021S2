/*************************************
* Lab 3 Exercise 2
* Name: Ngo Duc Binh
* Student Id: A0180061U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#ifndef __CS2106_RWLOCKV1_H_
#define __CS2106_RWLOCKV1_H_

#include <pthread.h> // To import the pthread_mutex_t type.

//TODO: You can modify the structure
typedef struct {
  pthread_mutex_t isRoomEmpty;
  pthread_mutex_t readerMutex;
  int reader_count;
  int writer_count;
} rw_lock;

//Note: You are NOT allowed to change the function headers
void initialise(rw_lock* lock);
void writer_acquire(rw_lock* lock);
void writer_release(rw_lock* lock);
void reader_acquire(rw_lock* lock);
void reader_release(rw_lock* lock);
void cleanup(rw_lock* lock);

#endif