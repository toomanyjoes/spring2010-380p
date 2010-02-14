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
	nodeval = malloc(sizeof(void *)*numtasks);
	ltally = malloc(sizeof(void *)*numtasks);
	for(i=0; i<numtasks; i++) {
		nodeval[i] = malloc(sizeof(int)*nodeval_elem);
		ltally[i] = malloc(sizeof(int)*nodeval_elem);
		memset(nodeval[i],0,sizeof(int)*nodeval_elem);
	}
	MPI_Status status;
// commit a new contiguous datatype to store nodeval and ltally values
//	MPI_Datatype nodeval_type;
//	MPI_Type_contiguous(nodeval_elem, MPI_INT, &nodeval_type);
//	MPI_Type_commit(&nodeval_type);

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


char str[256];
int len;

	for(i=0; i < myDataSize; i++) {
		myDataStruct.myData = myData[i];
		accum(tally, &myDataStruct);
		myDataStruct.processed++;
	}
	copyArray(tally, nodeval[id]);
	for(i=0; i<numtasks; i++)
		if(i != id) {
//			printf("proc: %d calling sendrecv to proc %d\n",id,i);fflush(stdout);fflush(stderr);
			MPI_Sendrecv(tally,nodeval_elem,MPI_INT,i,id,
			nodeval[i],nodeval_elem,MPI_INT,i,i,
			MPI_COMM_WORLD,&status);
//			MPI_Sendrecv(tally,1,nodeval_type,i,id,
//			nodeval[i],1,nodeval_type,i,i,
//			MPI_COMM_WORLD,&status);

//printf("proc %d returning from sendrecv\n",id);fflush(stdout);fflush(stderr);


//MPI_Error_string(status.MPI_ERROR, str, &len);
//printf("error: %s\n",str); 
/*printf("proc: %d calling send  dest: %d tag: %d\n",id,i,i);
				MPI_Send(nodeval[id],1,nodeval_type,i,i,MPI_COMM_WORLD);
printf("proc: %d calling recv  dest: %d tag: %d\n",id,i,i);
				MPI_Recv(nodeval[i],1,nodeval_type,i,i,MPI_COMM_WORLD,&status);
*///			printf("proc: %d returning from sendrecv to proc %d\n",id,i);
		}

//	npthread_barrier();
	int dummy = stride;
	while(dummy < numtasks)
	{
		if(id % (2*stride) == 0)
		{
			copyArray(nodeval[id],ltally[id+stride]);
			combine(ltally[id+stride], nodeval[id+stride], nodeval[id]);
			for(i = 0; i < numtasks; i++) // send out the changes
			{
//				printf("1 proc: %d calling send  dest: %d tag: %d\n",id,i,i);fflush(stdout);fflush(stderr);
				MPI_Send(nodeval[id],nodeval_elem,MPI_INT,i,id,MPI_COMM_WORLD);
//				MPI_Send(nodeval[id],1,nodeval_type,i,id,MPI_COMM_WORLD);
//				printf("1 returning from send\n");fflush(stdout);fflush(stderr);
				MPI_Send(ltally,nodeval_elem,MPI_INT,i,id+1,MPI_COMM_WORLD);
//				MPI_Send(ltally,1,nodeval_type,i,id+1,MPI_COMM_WORLD);
//				printf("1 returning from send\n");fflush(stdout);fflush(stderr);
			}
			stride = 2*stride;
		}
		else
			break;
		//npthread_barrier();
		dummy = 2*dummy;
	}

	dummy = stride;
	while(dummy < numtasks)
	{
		for(i = 0; i < numtasks; i++)
		{
			if(i != id && i % (2*dummy) == 0) // some other process changed/will change ltally and nodeval
			{
//				printf("2 proc: %d calling recv  src: %d tag: %d\n",id,i,i);fflush(stdout);fflush(stderr);
				MPI_Recv(nodeval[i],nodeval_elem,MPI_INT,i,i,MPI_COMM_WORLD,&status);
//				MPI_Recv(nodeval[i],1,nodeval_type,i,i,MPI_COMM_WORLD,&status);
//MPI_Error_string(status.MPI_ERROR, str, &len);
//printf("error: %s\n",str); 
//				printf("2 returning from recv\n");fflush(stdout);fflush(stderr);
				MPI_Recv(ltally[i+dummy],nodeval_elem,MPI_INT,i,i+1,MPI_COMM_WORLD,&status);
//				MPI_Recv(ltally[i+dummy],1,nodeval_type,i,i+1,MPI_COMM_WORLD,&status);
//MPI_Error_string(status.MPI_ERROR, str, &len);
//printf("error: %s\n",str); 
//				printf("2 proc: %d returning from recv to proc %d\n",id,i);fflush(stdout);fflush(stderr);
			}
		}
		dummy = 2*dummy;
	}

	//npthread_barrier();
	stride = dummy = numtasks/2;
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
			for(i = 0; i < numtasks; i++) {  // send out the changes
				if(i != id)
//				printf("3 proc: %d calling send  dest: %d tag: %d\n",id,i,i);fflush(stdout);fflush(stderr);
				MPI_Send(nodeval[id+stride],nodeval_elem,MPI_INT,i,id,MPI_COMM_WORLD);
//				MPI_Send(nodeval[id+stride],1,nodeval_type,i,id,MPI_COMM_WORLD);
// printf("3 returning from send\n");fflush(stdout);fflush(stderr);
			}
		}
		stride = stride / 2;
	}

	while(dummy >= 1)
	{
		for(i=0; i<numtasks; i++)
		{
			if(i != id && i % (2*dummy) == 0) // some other process changed/will change nodeval
			{
//				printf("4 proc: %d calling recv  src: %d tag: %d\n",id,i,i);fflush(stdout);fflush(stderr);
				MPI_Recv(nodeval[i+dummy],nodeval_elem,MPI_INT,i,i,MPI_COMM_WORLD,&status);
//				MPI_Recv(nodeval[i+dummy],1,nodeval_type,i,i,MPI_COMM_WORLD,&status);
//MPI_Error_string(status.MPI_ERROR, str, &len);
//printf("error: %s\n",str); 
//				printf("4 proc: %d returning from recv to proc %d\n",id,i);fflush(stdout);fflush(stderr);
			}
		}
		dummy = dummy / 2;
	}

	copyArray(nodeval[id],ptally);
	int result;
	int elem_per_node = inputArraySize/numtasks;
	for(i = 0; i<myDataSize; i++) {
		myDataStruct.myData = myData[i];
		myDataStruct.processed = i;
		if(id == 0)
			outputArray[i] = scanGen(ptally, &myDataStruct);
		else
		{
			result = scanGen(ptally, &myDataStruct);
			printf("5 proc: %d calling send, sending: %d to  dest: %d tag: %d\n",id,result,0,id*elem_per_node+i);fflush(stdout);fflush(stderr);
//			MPI_Send(&result,1,MPI_INT,0,id*elem_per_node+i,MPI_COMM_WORLD); //send result to root process
			MPI_Send(&result,1,MPI_INT,0,id*elem_per_node+i,MPI_COMM_WORLD); //send result to root process
//printf("5 returning from send\n");fflush(stdout);fflush(stderr);
		}
	}

	if(id == 0) // receive results from calculate
	{
		int j;
		int elem_per_node = inputArraySize/numtasks;
		for(i = 1; i < numtasks; i++)
			for(j = 0; j < elem_per_node; j++) {
printf("6 proc: %d calling recv receiving: %d src: %d tag: %d  i: %d j: %d\n",id,outputArray[i*elem_per_node+j],i,i,i,j);fflush(stdout);fflush(stderr);
			  MPI_Recv(&outputArray[i*elem_per_node+j],1,MPI_INT,i,i*elem_per_node+j,MPI_COMM_WORLD,&status);
//MPI_Error_string(status.MPI_ERROR, str, &len);
//printf("error: %s\n",str); 
//printf("6 returning from recv\n");fflush(stdout);fflush(stderr);
			}
	}

//	MPI_Type_free(&nodeval_type);

//printf("proc %d after MPI free\n",id);fflush(stdout);fflush(stderr);
	free(tmpMyData);
//printf("proc %d after tmpMyData\n",id);
	for(i=0; i<numtasks; i++) {
		free(nodeval[i]);
		free(ltally[i]);
	}
//printf("proc %d after loop\n",id);
	free(nodeval);
//printf("proc %d after nodeval\n",id);
	free(ltally);
//printf("proc %d after ltally\n",id);fflush(stdout);fflush(stderr);
}

