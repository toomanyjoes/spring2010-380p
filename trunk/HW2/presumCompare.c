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

void sum(int *invec, int *inoutvec, int *len, MPI_Datatype *dtype)
{
    int i,j;
int rank;
MPI_Comm_rank(MPI_COMM_WORLD, &rank);
//	inoutvec[0] += invec[0];
//	if(rank % 2 == 0)
//	{
//		inoutvec[i]
		for(i=0; i<*len; i++){
			for(j=i; j<*len;j++) {
//				printf("rank: %d invec[%d]: %d inoutvec[%d]: %d\n",rank,i,invec[i],i,inoutvec[i]);fflush(stdout);
    				invec[i] += invec[j];
			}
		}
		for(i=0; i<*len; i++){
			printf("rank: %d invec[%d]: %d inoutvec[%d]: %d\n",rank,i,invec[i],i,inoutvec[i]);fflush(stdout);
    			inoutvec[i] += invec[i];
		}
//	}
//	else
//	{
//		
//	}
//	inoutvec[*len-1] += invec[*len-1];
}

int main(int argc, char* argv[]) {
	int numtasks,rank,i,j;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	struct timeval begin, end;
	MPI_Status status;
	int *inputArray;
	int inputArraySize;
	intArray *tmpArray = readArray(argv[1]);
	inputArray = tmpArray->data;
	inputArraySize = tmpArray->size;
	int *outputArray = (int *)malloc(sizeof(int)*inputArraySize);

	gettimeofday(&begin, NULL);
	int myDataSize;
	int *result;
	MPI_Op op_sum;
	MPI_Op_create( (MPI_User_function *)sum, 0, &op_sum );
	if(rank == numtasks-1)
		myDataSize = (inputArraySize/numtasks) + (inputArraySize%numtasks);
	else
		myDataSize = inputArraySize/numtasks;
	result = (int *)malloc(sizeof(int)*myDataSize);
//	MPI_Scan(inputArray, result, myDataSize, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
	MPI_Scan(inputArray+rank*(inputArraySize/numtasks), result, myDataSize, MPI_INT, op_sum, MPI_COMM_WORLD);
// 	for(i=0;i<myDataSize;i++) {
//		MPI_Scan(inputArray+i+rank*(inputArraySize/numtasks), &result[i], 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
//		result[i] = result
 //	}
	printf("rank: %d\n",rank);
	for(i=0;i<myDataSize; i++)
		printf("%d ",result[i]);
	printf("\n");fflush(stdout);
	if(rank == 0)
	{
		memcpy(outputArray,result,sizeof(int)*myDataSize);
		for(i=1;i<numtasks-1;i++) {
			MPI_Recv(outputArray + (i*(inputArraySize/numtasks)),(inputArraySize/numtasks),MPI_INT,i,i,MPI_COMM_WORLD,&status);
		}
		MPI_Recv(outputArray + ((numtasks-1)*(inputArraySize/numtasks)),(inputArraySize/numtasks)+(inputArraySize%numtasks),MPI_INT,(numtasks-1),(numtasks-1),MPI_COMM_WORLD,&status);
	}
	else
		MPI_Send(result,myDataSize,MPI_INT,0,rank,MPI_COMM_WORLD);
	free(result);
	MPI_Op_free( &op_sum );
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
