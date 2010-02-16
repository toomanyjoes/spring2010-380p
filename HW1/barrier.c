#include "barrier.h"

/* Disclaimer: Any bugs, fix it yourself. :) */

int user_thread_count;
int cur_thread_count;
pthread_mutex_t barrier_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barrier_cond_t = PTHREAD_COND_INITIALIZER;


/* call this function BEFORE creation of your
 * threads. Pass the number of threads as an argument.*/
void npthread_init_barrier(int count_of_threads) {
	user_thread_count = count_of_threads;
	cur_thread_count = count_of_threads;
}

/* Call this wherever you want to enforce a barrier inside
 * the thread code. */
void npthread_barrier() {
	pthread_mutex_lock(&barrier_lock);
	cur_thread_count--;
	if(cur_thread_count == 0) {
		cur_thread_count = user_thread_count;
		pthread_cond_broadcast(&barrier_cond_t);
	} else {
		pthread_cond_wait(&barrier_cond_t, &barrier_lock);
	}
	pthread_mutex_unlock(&barrier_lock);
}
