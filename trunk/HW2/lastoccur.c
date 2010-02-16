/*
 * teamStandings.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/types.h>
#include "array_funcs.h"
#include "parallel_prefix.h"

extern int *outputArray;

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
	dataStruct *val = (dataStruct *)i;
	((int *)tally)[val->myData-1] = val->id * (val->inputArraySize / val->numtasks) + val->processed;
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
	dataStruct *val = (dataStruct *)i;
	int returnval = ((int *)tally)[val->myData-1];
	((int *)tally)[val->myData-1] = val->id * (val->inputArraySize / val->numtasks) + val->processed;
	return returnval;
}

int main(int argc, char* argv[]) {
	struct timeval begin, end;
	if(argc != 2) { printf("Usage:\n   %s [data file]\n",argv[0]); exit(1); }
	int *inputArray = readResults(argv[1]);
	int inputArraySize = stringLength;

	gettimeofday(&begin, NULL);
	parallel_prefix((void *)inputArray, inputArraySize, numValues);
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
}
