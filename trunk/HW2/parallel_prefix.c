#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "array_funcs.h"
#include "parallel_prefix.h"
#include "mpi.h"

//int *inputArray;
//int inputArraySize;
int *outputArray;
//void **nodeval;
//void **ltally;

void *init();
void accum(void *, void *);
void combine(void *, void *, void *);
int scanGen(void *, void *);

void parallel_prefix(void *data, int num_elem, int nodeval_elem)
{
	int numtasks,rank,i,j;
//printf("about to call mpi init\n\n");
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

//printf("numtasks: %d\n",numtasks);

	if(rank == 0)
		outputArray = (int *)malloc(sizeof(int)*num_elem);
//printf("Hello World from proc %d\n",rank);
	calculate(data, rank, numtasks, num_elem, nodeval_elem);

//MPI_Barrier(MPI_COMM_WORLD);
//printf("proc %d calling finalize\n",rank);
//fflush(stdout);
//if(rank == 0)
	MPI_Finalize();
	if(rank != 0)
		exit(0);
}

intArray *localize(int *array, int size, int id, int numtasks)
{
	intArray *local = (intArray *)malloc(sizeof(intArray));
	if(id == (numtasks-1)) // last id
		local->size = (size / numtasks) + (size % numtasks);
	else
		local->size = size / numtasks;
	local->data = array + id * (size / numtasks);
	return local;
}

void calculate(void *inputArray, int id, int numtasks, int inputArraySize, int nodeval_elem)
{
	void **nodeval;
	void **ltally;

	int i;
	nodeval = (void **)malloc(sizeof(void **)*numtasks);
	ltally = (void **)malloc(sizeof(void **)*numtasks);
	for(i=0; i<numtasks; i++) {
		nodeval[i] = malloc(sizeof(int)*nodeval_elem);
		ltally[i] = malloc(sizeof(int)*nodeval_elem);
		memset(nodeval[i],0,sizeof(int)*nodeval_elem);
		memset(ltally[i],0,sizeof(int)*nodeval_elem);
	}
	MPI_Status status;
	intArray *tmpMyData = localize((int *)inputArray, inputArraySize, id, numtasks);
	int *myData = tmpMyData->data;
	int myDataSize = tmpMyData->size;
	void *ptally;
	void *tally;
	int stride = 1;
	tally = init();
	ptally = init();
	dataStruct myDataStruct;
	myDataStruct.id = id;
	myDataStruct.processed = 0;
	myDataStruct.inputArraySize = inputArraySize;
	myDataStruct.numtasks = numtasks;
	for(i=0; i < myDataSize; i++) {
		myDataStruct.myData = myData[i];
		accum(tally, &myDataStruct);
		myDataStruct.processed++;
	}
	copyArray(tally, nodeval[id]);
	for(i=0; i<numtasks; i++)
		if(i != id) {
			MPI_Sendrecv(tally,nodeval_elem,MPI_INT,i,id,
			nodeval[i],nodeval_elem,MPI_INT,i,i,
			MPI_COMM_WORLD,&status);
		}
	MPI_Barrier(MPI_COMM_WORLD);
	int dummy;
	while(stride < numtasks)
	{
		if(id % (2*stride) == 0)
		{
			copyArray(nodeval[id],ltally[id+stride]);
			combine(ltally[id+stride], nodeval[id+stride], nodeval[id]);

			for(i = 0; i < numtasks; i++)
			{
				if(i != id)
				{
					dummy = numtasks/2;
					while(dummy >= 1)
					{
						if(i % (2*dummy) == 0) // Send only to processes that will need the value later
						{
							MPI_Send(nodeval[id],nodeval_elem,MPI_INT,i,id*dummy,MPI_COMM_WORLD);
						}
						dummy = dummy / 2;
					}
				}
			}
		}
//		else
//		{
			for(i=0; i<numtasks; i++)
			{
				if(id != i && i % (2*stride) == 0) // some other process changed/will change ltally and nodeval
				{
					dummy = numtasks/2;
					while(dummy >= 1)
					{
						if(id % (2*dummy) == 0) // I will need the value later
						{
							copyArray(nodeval[i],ltally[i+stride]);
							MPI_Recv(nodeval[i],nodeval_elem,MPI_INT,i,i*dummy,MPI_COMM_WORLD,&status);
						}
						dummy = dummy / 2;
					}
				}
				//else
					//break;
//			}
		}
//		MPI_Barrier(MPI_COMM_WORLD);
		stride = 2*stride;
	}

	stride = numtasks/2;
	if(id == 0)
	{
		copyArray(nodeval[0],ptally);
		clear(nodeval[0]); // no need to send to other processes
	}
	while(stride >= 1)
	{
		if(id % (2*stride) == 0)
		{
			copyArray(nodeval[id],ptally);
			combine(ptally, ltally[id+stride], nodeval[id+stride]);
			MPI_Send(nodeval[id+stride],nodeval_elem,MPI_INT,id+stride,(id+1)*stride,MPI_COMM_WORLD);
		}
//		else
//		{
			for(i=0; i<numtasks; i++)
			{
				if(i % (2*stride) == 0 && id == i+stride) // some other process changed/will change nodeval
				{
					MPI_Recv(nodeval[i+stride],nodeval_elem,MPI_INT,i,(i+1)*stride,MPI_COMM_WORLD,&status);
				}
			}
//		}
//		MPI_Barrier(MPI_COMM_WORLD);
		stride = stride / 2;
	}

	copyArray(nodeval[id],ptally);
	int elem_per_node = inputArraySize/numtasks;
	if(id == 0)
		for(i=0; i<myDataSize; i++)
		{
			myDataStruct.myData = myData[i];
			myDataStruct.processed = i;
			outputArray[i] = scanGen(ptally, &myDataStruct);
		}
	else
	{
		int *result = (int *)malloc(sizeof(int)*myDataSize);
		for(i = 0; i<myDataSize; i++)
		{
			myDataStruct.myData = myData[i];
			myDataStruct.processed = i;
			result[i] = scanGen(ptally, &myDataStruct);
		}
		MPI_Send(result,myDataSize,MPI_INT,0,id*100,MPI_COMM_WORLD); //send result to root process
		free(result);
	}

	if(id == 0 && numtasks > 1) // receive results from calculate
	{
		int j;
		for(i = 1; i < numtasks-1; i++)
		{
			MPI_Recv(outputArray + (i*elem_per_node),elem_per_node,MPI_INT,i,i*100,MPI_COMM_WORLD,&status);
		}
		MPI_Recv(outputArray + ((numtasks-1)*elem_per_node),elem_per_node + (inputArraySize % numtasks),MPI_INT,numtasks-1,(numtasks-1)*100,MPI_COMM_WORLD,&status);
	}

	free(tmpMyData);
	for(i=0; i<numtasks; i++) {
		free(nodeval[i]);
		free(ltally[i]);
	}
	free(nodeval);
	free(ltally);
	free(tally);
	free(ptally);
//	MPI_Barrier(MPI_COMM_WORLD);
}


