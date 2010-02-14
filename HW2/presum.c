/*
 * presum.c -- prefix sum
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "array_funcs.h"
#include "parallel_prefix.h"

extern int *outputArray;

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
	struct timeval begin, end;
	MPI_Init(NULL,NULL);
	int *inputArray;
	int inputArraySize;
	intArray *tmpArray = readArray(argv[1]);
	inputArray = tmpArray->data;
	inputArraySize = tmpArray->size;

	gettimeofday(&begin, NULL);
	parallel_prefix((void *)inputArray, inputArraySize, 1);
	gettimeofday(&end, NULL);

	int i;
	for(i = 0;i<inputArraySize; i++)
		printf("%d ", outputArray[i]);
	printf("\n");
	double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	printf("Time: %lf\n",endsec-beginsec);

	free(outputArray);
	free(inputArray);
	free(tmpArray);
}
