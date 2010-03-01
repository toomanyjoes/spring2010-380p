/*
 * matrixmult.c -- Assignment 3 Sparse Matrix-Vector Multiplication using Pthreads
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <bebop/smc/coo_matrix.h>
#include <bebop/smc/csr_matrix.h>
#include <bebop/smc/read_mm.h>
#include "inout.h"
#include "verify.h"
#define MAX_THREAD 1000

void multiply(int num_threads, int rank, csr_matrix_t *inMatrix, vector *inVector, vector *out_result);
int getRowPtrIndex(csr_matrix_t *matrix, unsigned long *takenSets);

const unsigned long masks[] = {0x80000000, 0x40000000, 0x20000000, 0x10000000,
			       0x08000000, 0x04000000, 0x02000000, 0x01000000,
			       0x00800000, 0x00400000, 0x00200000, 0x00100000,
			       0x00080000, 0x00040000, 0x00020000, 0x00010000,
			       0x00008000, 0x00004000, 0x00002000, 0x00001000,
			       0x00000800, 0x00000400, 0x00000200, 0x00000100,
			       0x00000080, 0x00000040, 0x00000020, 0x00000010
			       0x00000008, 0x00000004, 0x00000002, 0x00000001};

int main(int argc, char* argv[])
{
	if(argc != 4 && argc != 5)
	{
		printf("Usage:\n   %s [MMEF in file] [vector in file] [out file] <[num threads]>\n",argv[0]);
		exit(1);
	}
	vector invector;
	vector result;
	csr_matrix_t *csrMatrix;
	int numtasks,rank;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	struct timeval begin, end;
	struct coo_matrix_t cooMatrix;
	read_MMEF(argv[1], &cooMatrix);
	csrMatrix = coo_to_csr(&cooMatrix);
	read_vector(argv[2], &invector);
	char *outfile = argv[3];
	if(argc == 5)
		num_threads = atoi(argv[4]);
	else
		num_threads = 4; // default to 4 threads
	if ((num_threads < 1) || (num_threads > MAX_THREAD))
	{
		printf ("The number of threads must be between 1 and %d.\n",MAX_THREAD);
		exit(1);
	}

	MPI_Barrier(MPI_COMM_WORLD); // wait for all processes to finish I/O
	gettimeofday(&begin, NULL);
	multiply(numtasks, rank, csrMatrix, &invector, &result);
	gettimeofday(&end, NULL);
	double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	printf("Time: %lf\n",endsec-beginsec);
	
	write_vector(outfile, &result);

	// Verify against a sequential multiplication calculation
	// This works if you want to run a sequential version of this algoirthm
	if(verifyMult(csrMatrix, &invector, &result) == 0)
	{
		printf("\nFailed to verify result!\n\n");
	}

	return 0;
}

void multiply(int numtasks, int rank, csr_matrix_t *matrix, vector *vec, vector *out_result)
{
	int i, j, k, totalSets;
	int nextRow, rows_per_iter;
	int takenSetsSize;
	unsigned long *takenSets; // bit array indicating available sets of rows
	unsigned long *recvBuf;
	MPI_Op reduce_op;
	MPI_Op_create((MPI_User_function *)reduce_op_func, 1, &reduce_op);
	out_result->rows = matrix->m;
	out_result->values = (double *)malloc(sizeof(double)*out_result->rows);
	memset(out_result->values,0,sizeof(double)*out_result->rows);
	rows_per_iter = (inMatrix->rowptrsize / (4*numtasks))+1;
	totalSets = inMatrix->rowptrsize / rows_per_iter;
	if(inMatrix->rowptrsize % rows_per_iter != 0)
		totalSets++;
	takenSetsSize = totalSets / 32;
	if(totalSets % 32 != 0)
		takenSetsSize++;
	takenSets = malloc(sizeof(long)*takenSetsSize);
	recvBuf = malloc(sizeof(long)*takenSetsSize);
	memset(takenSets,0,sizeof(long)*takenSetsSize);
	for(i = 0, j = -1, k = 0; i < numtasks; i++)
	{
		if(i % 32 == 0)  { j++; k = 0; }
		takenSets[j] |= masks[k++];
	}

	int takenSetElem = 0;
	int takenSetPos = rank;
	int rowIndex = rank*rows_per_iter;
	int *rowptr = matrix->rowptr;
	int *colidx = matrix->colidx;
	int rowptrsize = matrix->rowptrsize;
	double *mvalues = (double *)matrix->values;
	double *rvalues = out_result->values;
	double *vvalues = vec->values;
	int testval;
	
	while(rowIndex != -1)
	{
		for(j = 0; j < rows_per_iter && rowIndex < rowptrsize; j++,rowIndex++)	// for each row
		{
			if(rowIndex < rowptrsize-1)
			{
				if(rowptr[rowIndex] == rowptr[rowIndex+1])
					continue;
				testval = rowptr[rowIndex+1];
			}
			else
				testval = matrix->nnz;
			for(k = rowptr[rowIndex]; k < testval; k++)	// for each value in the row
			{
				rvalues[rowIndex] += mvalues[k] * vvalues[ colidx[k] ];
			}
		}
		MPI_AllReduce(takenSets,recvBuf,takenSetsSize,MPI_LONG,reduce_op,MPI_COMM_WORLD);
		for(i = takenSetElem; i < takenSetsSize; i++)
		{
			for(j = takenSetPos; j < 32; j++)
				if(
		}
	}
		
	return (void *)0;
}

void reduce_op_func(unsigned long *in, unsigned long *inout, int *len, MPI_Datatype *type)
{
	int i;
	for(i = 0; i < *len; i++)
		inout[i] |= in[i];
}

int getRowPtrIndex(csr_matrix_t *matrix, unsigned long *takenSets)
{
	int myRow;
	if(nextRow == -1)
		return -1;	// no more rows to compute
	pthread_mutex_lock(&availableRowsLock);
	{
		myRow = nextRow;
		if(nextRow != -1)
			nextRow = (nextRow+rows_per_iter < matrix->rowptrsize ? nextRow + rows_per_iter : -1);
	}
	pthread_mutex_unlock(&availableRowsLock);
	return myRow;	
}
