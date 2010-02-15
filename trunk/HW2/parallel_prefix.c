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
	int dummy;
	while(stride < numtasks)
	{
		if(id % (2*stride) == 0)
		{
			copyArray(nodeval[id],ltally[id+stride]);
			combine(ltally[id+stride], nodeval[id+stride], nodeval[id]);
			
			// check if another process needs this value later in this loop
			for(i = 0; i < numtasks; i++)
			{
				if(i != id)
				{
					dummy = stride;
					while(dummy < numtasks)
					{
						if(id == i+dummy && i % (2*dummy) == 0) // Send only to processes that will need the value later
						{
//printf("1 proc %d send to %d\n",id,i);
							MPI_Send(nodeval[id],nodeval_elem,MPI_INT,i,id*dummy,MPI_COMM_WORLD);
							break;
						}
						dummy = 2*dummy;
					}
				}
			}

// 			// check which process will need this value after this loop
// 			if(2*stride >= numtasks) // this means this is the last iteration of this loop
// 			{
// 				for(i = 0; i < numtasks; i++)
// 				{
// 					dummy = numtasks/2;
// 					while(dummy >= 1)
// 					{
// 						if(i % (2*dummy) == 0 && id == i+dummy)
// 						{
// printf("2 proc %d send to %d\n",id,i);
// 							MPI_Send(ltally[id+stride],nodeval_elem,MPI_INT,i,id*dummy,MPI_COMM_WORLD);
// 							MPI_Send(nodeval[id],nodeval_elem,MPI_INT,i,id*dummy+1,MPI_COMM_WORLD);
// 							break;
// 						}
// 						dummy = dummy / 2;
// 					}
// 				}
// 			}
		}

		// receive the values that are needed later in this loop
		for(i=0; i<numtasks; i++)
		{
			if(id != i && i % (2*stride) == 0) // some other process changed/will change ltally and nodeval
			{
				dummy = stride;
				while(dummy < numtasks)
				{
					if(i == id+dummy && id % (2*dummy) == 0)
					{
//printf("1 proc %d recv from %d\n",id,i);
						copyArray(nodeval[i],ltally[i+stride]);
						MPI_Recv(nodeval[i],nodeval_elem,MPI_INT,i,i*dummy,MPI_COMM_WORLD,&status);
						break;
					}
					dummy = 2*dummy;
				}
			}
		}

// 		// receive the values that are needed after this loop
// 		if(2*stride >= numtasks) // this means that this is the last iteration of this loop
// 		{
// 			for(i = 0; i < numtasks; i++)
// 			{
// 				if(id != i && i % (2*stride) == 0)
// 				{
// 					dummy = numtasks/2;
// 					while(dummy >= 1)
// 					{
// 						if(id % (2*dummy) == 0 && i == id+dummy)
// 						{
// printf("2 proc %d recv from %d\n",id,i);
// 							MPI_Recv(ltally[i+stride],nodeval_elem,MPI_INT,i,id*dummy,MPI_COMM_WORLD,&status);
// 							MPI_Recv(nodeval[i],nodeval_elem,MPI_INT,i,i*dummy+1,MPI_COMM_WORLD,&status);
// 							break;
// 						}
// 						dummy = dummy / 2;
// 					}
// 				}
// 			}
// 		}
//		MPI_Barrier(MPI_COMM_WORLD); // make sure everyone has completed this round
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
//printf("1 proc %d send to %d\n",id,id+stride);
			MPI_Send(nodeval[id+stride],nodeval_elem,MPI_INT,id+stride,id,MPI_COMM_WORLD);
		}
		for(i=0; i<numtasks; i++)
		{
			if(id == i+stride && i % (2*stride) == 0) // some other process changed/will change nodeval
			{
//printf("1 proc %d recv from %d\n",id,i);
				MPI_Recv(nodeval[i+stride],nodeval_elem,MPI_INT,i,i,MPI_COMM_WORLD,&status);
			}
		}
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
fflush(stdout);
MPI_Barrier(MPI_COMM_WORLD);
}


