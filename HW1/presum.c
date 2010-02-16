/*
 * presum.c -- prefix sum
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include "array_funcs.h"
#include "parallel_prefix.h"

extern int *inputArray;
extern int inputArraySize;
extern int *outputArray;
extern int **nodeval;
extern int **ltally;
extern int num_threads;

void *init()
{
	int *val = (int *)malloc(sizeof(int));
	*val = 0;
	return (void *)val;
}

void clear(void *val)
{
	((int *)val)[0] = 0;
}

void copyArray(void *vsrc, void *vdest)
{
	int i;
	int *src = (int *)vsrc;
	int *dest = (int *)vdest;
	for(i=0;i<1; i++) {
		dest[i] = src[i];
	}
}

void accum(void *tally, void *i)
{
	((int *)tally)[0] += ((int *)i)[0];
}

void combine(void *left, void *right, void *assignment)
{
	((int*)assignment)[0] = ((int*)left)[0] + ((int*)right)[0];
}

int scanGen(void *tally, void *i)
{
	return ((int*)tally)[0] += ((int *)i)[0];
}

int main(int argc, char* argv[]) {
	if(argc != 3) { printf("Usage:\n   %s [data file] [num threads]\n",argv[0]); exit(1); }
	intArray *tmpArray = readArray(argv[1]);
	inputArray = tmpArray->data;
	inputArraySize = tmpArray->size;
	num_threads = atoi(argv[2]);
	if ((num_threads < 1) || (num_threads > MAX_THREAD)) {
		printf ("The number of threads must be between 1 and %d.\n",MAX_THREAD);
		exit(1);
	}
	outputArray = (int *)malloc(sizeof(int)*(inputArraySize));
	int i;
	nodeval = malloc(sizeof(void *)*num_threads);
	ltally = malloc(sizeof(void *)*num_threads);
	for(i=0; i<num_threads; i++) {
		nodeval[i] = malloc(sizeof(int));
		ltally[i] = malloc(sizeof(int));
		memset(nodeval[i],0,sizeof(int));
	}


	parallel_prefix((void *)inputArray, inputArraySize);

	for(i = 0;i<inputArraySize; i++)
		printf("%d ", outputArray[i]);
	printf("\n");

	for(i=0; i<num_threads; i++) {
		free(nodeval[i]);
		free(ltally[i]);
	}
	free(nodeval);
	free(ltally);
	free(outputArray);
	free(tmpArray);
	free(inputArray);
}
