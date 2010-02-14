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
#include "mpi.h"

extern int *outputArray;

int numTeams;
int numGames;

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
	// first int is the number of teams
	if(!fscanf(file,"%d",&(numTeams))) {
		fprintf(stderr,"Error reading number of teams\n");
		exit(1);
	}
	// second int is the number of games
	if(!fscanf(file,"%d",&(numGames))) {
		fprintf(stderr,"Error reading number of games\n");
		exit(1);
	}
	array = (int*)malloc(sizeof(int)*numGames);
	int i;
	for(i=0; i<numGames; i++) {
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
	for(i=0;i<numTeams; i++) {
		dest[i] = src[i];
	}
}

void clear(void *val)
{
	int *array = ((int *)val);
	int i;
	for(i=0;i<numTeams;i++)
		array[i] = 0;
}

void *init()
{
	void *val = malloc(sizeof(int) * numTeams);
	memset(val, 0, sizeof(int) * numTeams);
	return val;
}

void accum(void *tally, void *i)
{
	((int *)tally)[ ((int *)i)[0] ]++;
}

void combine(void *vleft, void *vright, void *vassignmentVal)
{
	int *left = (int *)vleft;
	int *right = (int *)vright;
	int *assignmentVal = (int *)vassignmentVal;
	int i;
	for(i=0; i < numTeams; i++)
		assignmentVal[i] = left[i] + right[i];
}

int scanGen(void *tally, void *i)
{
	((int *)tally)[ ((int *)i)[0] ]++;
	return maxIndex(((int *)tally),numTeams);
}

int main(int argc, char* argv[]) {
	struct timeval begin, end;
	if(argc != 2) { printf("Usage:\n   %s [data file]\n",argv[0]); exit(1); }
	int *inputArray = readResults(argv[1]);
	int inputArraySize = numGames;

	gettimeofday(&begin, NULL);
	parallel_prefix((void *)inputArray, inputArraySize, numTeams);
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
