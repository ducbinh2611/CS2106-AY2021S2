/*************************************
* Lab 3 Exercise 3
* Name: Ngo Duc Binh
* Student Id: A0180061U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

/************************************

You should use ex2 solution as the starting point.

Copy over the solution and modify as needed.
************************************/

#include <pthread.h>
#include "rw_lock.h"
#include <unistd.h>

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_init(&(lock->readerMutex), NULL);
  pthread_mutex_init(&(lock->isRoomEmpty), NULL);
  pthread_mutex_init(&(lock->in), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}
/*
* Having the in semaphore such that when a writer acquire this lock, all the subsequent
* readers will be blocked. Thus, the writer will be given the access first before
* the late-coming readers.
*/
void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->in));
  pthread_mutex_lock(&(lock->isRoomEmpty));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  //TODO: modify as needed
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->isRoomEmpty));
  pthread_mutex_unlock(&(lock->in));
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->in));
  pthread_mutex_lock(&(lock->readerMutex));
  // if this is the first reader, has to wait for the room to be empty
  if (lock->reader_count == 0) {
    pthread_mutex_lock(&(lock->isRoomEmpty));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->readerMutex));
  pthread_mutex_unlock(&(lock->in));
}

void reader_release(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->readerMutex));
  lock->reader_count--;
  if (lock->reader_count == 0) {
    pthread_mutex_unlock(&(lock->isRoomEmpty));
  }
  pthread_mutex_unlock(&(lock->readerMutex));
}

void cleanup(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_destroy(&(lock->isRoomEmpty));
  pthread_mutex_destroy(&(lock->readerMutex));
  pthread_mutex_destroy(&(lock->in));
}
