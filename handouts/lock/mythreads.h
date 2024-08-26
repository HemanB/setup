#ifndef __MYTHREADS_h__
#define __MYTHREADS_h__

#include <pthread.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/syscall.h>

// to initialize a lock
extern void Pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr);

// to lock
extern void Pthread_mutex_lock(pthread_mutex_t *m);

// to unlock                                                         
extern void Pthread_mutex_unlock(pthread_mutex_t *m);

// to create threads                                                     
extern void Pthread_create(pthread_t *thread, const pthread_attr_t *attr, 
            void *(*start_routine)(void*), void *arg);

// use this to join threads
extern void Pthread_join(pthread_t thread, void **value_ptr);

void my_threads_init();
void my_threads_end();

// the worker function to call at create.
extern void *thread_worker(void *args);

// use this to obtain a thread id from the system.
extern long Thread_gettid();

// simple print_thread_info routine
extern void print_thread_info();

// unique to this problem


#endif // __MYTHREADS_h__
