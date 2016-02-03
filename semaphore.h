#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

#include <pthread.h>

typedef struct {
	unsigned int count;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} semaphore;

int semaphore_init(semaphore* sp, unsigned int count);
int mySemaphore_destroy(semaphore* sp);
int semaphore_down(semaphore* sp);
int semaphore_up(semaphore* sp);
int semaphore_getCount(const semaphore* sp);

#endif
