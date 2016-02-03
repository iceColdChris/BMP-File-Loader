#include "semaphore.h"

int semaphore_init(semaphore* sp, unsigned int count) {
	int rv;

	sp->count = count;
	rv = pthread_mutex_init(&sp->mutex, NULL);
	if (rv != 0)
		return rv;
	rv = pthread_cond_init(&sp->cond, NULL);
	if (rv != 0)
		pthread_mutex_destroy(&sp->mutex);
	return rv;
}

int mySemaphore_destroy(semaphore* sp) {
	int rv;

	rv = pthread_cond_destroy(&sp->cond);
	if (rv != 0) {
		pthread_mutex_destroy(&sp->mutex);
		return rv;
	}
	return pthread_mutex_destroy(&sp->mutex);
}

int semaphore_down(semaphore* sp) {
	pthread_mutex_lock(&sp->mutex);
	if (sp->count > 0) {
		sp->count--;
	} else {
		pthread_cond_wait(&sp->cond, &sp->mutex);
		sp->count--;
	}
	return pthread_mutex_unlock(&sp->mutex);
}

int semaphore_up(semaphore* sp) {
	pthread_mutex_lock(&sp->mutex);
	sp->count++;
	if (sp->count == 1) {
		pthread_cond_signal(&sp->cond);
	}
	return pthread_mutex_unlock(&sp->mutex);
}

int semaphore_getCount(const semaphore* sp) {
	return sp->count;
}
