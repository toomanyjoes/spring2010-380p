/*
 * presumCompare.c -- prefix sum
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "array_funcs.h"
#include "mpi.h"

int main(int argc, char* argv[]) {
	int numtasks,rank,i,j;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	struct timeval begin, end;
	int *inputArray;
	int inputArraySize;
	intArray *tmpArray = readArray(argv[1]);
	inputArray = tmpArray->data;
	inputArraySize = tmpArray->size;
	int *outputArray = (int *)malloc(sizeof(int)*inputArraySize);
	int test;

	gettimeofday(&begin, NULL);
	MPI_Scan((void *)inputArray, (void *)&test, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	printf("proc %d test: %d\n",rank,test);
	MPI_Finalize();
	if(rank != 0)
		exit(0);
	gettimeofday(&end, NULL);

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
