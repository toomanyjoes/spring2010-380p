#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "array_funcs.h"
#include "parallel_prefix.h"
#include "barrier.h"

int *inputArray;
int inputArraySize;
int *outputArray;
void **nodeval;
void **ltally;
int num_threads;

void *init();
void accum(void *, void *);
void combine(void *, void *, void *);
int scanGen(void *, void *);

int parallel_prefix(void *data, int num_elem)
{
	long i;
	pthread_attr_t attr;
	pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t)*num_threads);
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	npthread_init_barrier(num_threads);
	for(i=0; i<num_threads; i++) 
		pthread_create(&threads[i], &attr, thread_main, (void *)i);
	for (i=0; i<num_threads; i++)
		pthread_join(threads[i],NULL);
}

intArray *localize(int *array, int size, int id)
{
	intArray *local = (intArray *)malloc(sizeof(intArray));
	if(id == (num_threads-1)) // last id
		local->size = (size / num_threads) + (size % num_threads);
	else
		local->size = size / num_threads;
	local->data = array + id * (size / num_threads);
	return local;
}

void *thread_main(void *arg)
{
	int id = (int)arg;
	intArray *tmpMyData = localize(inputArray, inputArraySize, id);
	int *myData = tmpMyData->data;
	int myDataSize = tmpMyData->size;
	void *ptally;
	int i;
	void *tally;
	int stride = 1;
	tally = init();
	ptally = init();
	dataStruct myDataStruct;
	myDataStruct.id = id;
	myDataStruct.processed = 0;
	for(i=0; i < myDataSize; i++) {
		myDataStruct.myData = myData[i];
		accum(tally, &myDataStruct);
		myDataStruct.processed++;
	}
	copyArray(tally, nodeval[id]);
	npthread_barrier();
	int dummy = stride;
	while(dummy < num_threads)
	{
		if(id % (2*stride) == 0)
		{
			copyArray(nodeval[id],ltally[id+stride]);
			combine(ltally[id+stride], nodeval[id+stride], nodeval[id]);
			stride = 2*stride;
		}
			npthread_barrier();
			dummy = 2*dummy;
	}

	npthread_barrier();
	stride = num_threads/2;
	if(id == 0)
	{
		copyArray(nodeval[0],ptally);
		clear(nodeval[0]);
	}
	while(stride >= 1)
	{
		if(id % (2*stride) == 0)
		{
			copyArray(nodeval[id],ptally);
			combine(ptally, ltally[id+stride], nodeval[id+stride]);
		}
		stride = stride / 2;
		npthread_barrier();
	}
	copyArray(nodeval[id],ptally);
	for(i = 0; i<myDataSize; i++) {
		myDataStruct.myData = myData[i];
		myDataStruct.processed = i;
		outputArray[id*(inputArraySize/num_threads)+i] = scanGen(ptally, &myDataStruct);
	}
	free(tmpMyData);
}

