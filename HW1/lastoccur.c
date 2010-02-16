/*
 * teamStandings.c
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
extern void **nodeval;
extern void **ltally;
extern int num_threads;

int numValues;
int stringLength;

int maxIndex(int *array, int size)
{
	int i;
	int max = array[0];
	int maxIndex = 0;
	for(i=1; i < size; i++) {
		if(array[i] >= max) {
			if(array[i] == max) {
				maxIndex = 0;
}
			else {
				max = array[i];
				maxIndex = i;
			}
		}
	}
	return maxIndex;
}

int *readResults(char *fname)
{
	FILE *file = fopen(fname, "r");
	if(!file) {
		fprintf(stderr,"Error opening file: %s\n", fname);
		exit(1);
	}
	int *array;
	// first int is the number possible values
	if(!fscanf(file,"%d",&(numValues))) {
		fprintf(stderr,"Error reading number of values\n");
		exit(1);
	}
	// second int is the length of the input string
	if(!fscanf(file,"%d",&(stringLength))) {
		fprintf(stderr,"Error reading string length\n");
		exit(1);
	}
	array = (int*)malloc(sizeof(int)*stringLength);
	int i;
	for(i=0; i<stringLength; i++) {
		if(!fscanf(file,"%d",array+i)) {
			fprintf(stderr,"Error reading element %d",i);
			exit(1);
		}
	}
	fclose(file);
	return array;
}

void copyArray(void *vsrc, void *vdest)
{
	int i;
	int *src = (int *)vsrc;
	int *dest = (int *)vdest;
	for(i=0;i<numValues; i++) {
		dest[i] = src[i];
	}
}

void clear(void *val)
{
	int *array = ((int *)val);
	int i;
	for(i=0;i<numValues;i++)
		array[i] = 0;
}

void *init()
{
	void *val = malloc(sizeof(int) * numValues);
	memset(val, 0, sizeof(int) * numValues);
	return val;
}

void accum(void *tally, void *i)
{
	// val[0] = number from 1...k   val[1] = thread id    val[2] = count of numbers processed by this thread
	int *val = (int *)i;
	((int *)tally)[val[0]-1] = val[1] * (inputArraySize / num_threads) + val[2];
}

void combine(void *vleft, void *vright, void *vassignmentVal)
{
	int *left = (int *)vleft;
	int *right = (int *)vright;
	int *assignmentVal = (int *)vassignmentVal;
	int i;
	for(i=0; i < numValues; i++)
		assignmentVal[i] = (left[i] > right[i] ? left[i] : right[i]);
}

int scanGen(void *tally, void *i)
{
	int *val = (int *)i;
	int returnval = ((int *)tally)[val[0]-1];
	((int *)tally)[val[0]-1] = val[1] * (inputArraySize / num_threads) + val[2];
	return returnval;
}

int main(int argc, char* argv[]) {
	if(argc != 3) { printf("Usage:\n   %s [data file] [num threads]\n",argv[0]); exit(1); }
	inputArray = readResults(argv[1]);
	inputArraySize = stringLength;
	num_threads = atoi(argv[2]);
	if ((num_threads < 1) || (num_threads > MAX_THREAD)) {
		printf ("The number of threads must be between 1 and %d.\n",MAX_THREAD);
		exit(1);
	}

	int i;
	nodeval = malloc(sizeof(void *)*num_threads);
	ltally = malloc(sizeof(void *)*num_threads);
	for(i=0; i<num_threads; i++) {
		nodeval[i] = malloc(sizeof(int)*numValues);
		ltally[i] = malloc(sizeof(int)*numValues);
		memset(nodeval[i],0,sizeof(int)*numValues);
	}

	outputArray = (int *)malloc(sizeof(int)*(inputArraySize));

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
	free(inputArray);
}
