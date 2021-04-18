/*************************************
* Lab 3 Exercise 2
* Name: Ngo Duc Binh
* Student Id: A0180061U
* Lab Group: B01
*************************************
Note: Duplicate the above and fill in 
for the 2nd member if  you are on a team
*/

#include <pthread.h>
#include "rw_lock.h"

void initialise(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_init(&(lock->readerMutex), NULL);
  pthread_mutex_init(&(lock->isRoomEmpty), NULL);
  lock->reader_count = 0;
  lock->writer_count = 0;
}

void writer_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->isRoomEmpty));
  lock->writer_count++;
}

void writer_release(rw_lock* lock)
{
  //TODO: modify as needed
  lock->writer_count--;
  pthread_mutex_unlock(&(lock->isRoomEmpty));
}

void reader_acquire(rw_lock* lock)
{
  //TODO: modify as needed
  pthread_mutex_lock(&(lock->readerMutex));
  if (lock->reader_count == 0) {
    pthread_mutex_lock(&(lock->isRoomEmpty));
  }
  lock->reader_count++;
  pthread_mutex_unlock(&(lock->readerMutex));
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
  pthread_mutex_destroy(&(lock->readerMutex));
  pthread_mutex_destroy(&(lock->isRoomEmpty));
}