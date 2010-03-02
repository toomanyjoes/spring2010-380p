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
#include "mpi.h"

void multiply(int numtasks, int rank, csr_matrix_t *matrix, vector *vec, vector *out_result);
void runController(csr_matrix_t *matrix, vector *result, int numtasks);

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("Usage:\n   %s [MMEF in file] [vector in file] [out file]\n",argv[0]);
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
	out_result->rows = matrix->m;
	out_result->values = (double *)malloc(sizeof(double)*out_result->rows);
	if(rank == 0)
	{
		if(numtasks > 1)
			runController(matrix, out_result, numtasks);
		else
			seqMult(matrix, vec, out_result);	// only one process so run the sequential version
		return;
	}
	memset(out_result->values,0,sizeof(double)*out_result->rows);
	MPI_Status status;
	int i, j, k;
	int rows_per_iter;
	rows_per_iter = (matrix->rowptrsize / (4*(numtasks-1)))+1;

	int rowIndex = (rank-1)*rows_per_iter;
	int *rowptr = matrix->rowptr;
	int *colidx = matrix->colidx;
	int rowptrsize = matrix->rowptrsize;
	double *mvalues = (double *)matrix->values;
	double *rvalues = out_result->values;
	double *vvalues = vec->values;
	int testval;
	int startRow = rowIndex;
	int rowsCompleted = 0;
	
	while(rowIndex != -1)
	{
		for(j = 0; j < rows_per_iter && rowIndex < rowptrsize; j++,rowIndex++)	// for each row
		{
			rowsCompleted++;
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
		MPI_Sendrecv(&rvalues[startRow], rowsCompleted, MPI_DOUBLE, 0, 0,
			&startRow, 1, MPI_INT, 0, MPI_ANY_TAG,
                 	MPI_COMM_WORLD, &status);
		rowsCompleted = 0;
		rowIndex = startRow;
	}
	
	if(rank != 0)
		exit(0);	// kill everyone except root
}


void runController(csr_matrix_t *matrix, vector *result, int numtasks)
{
	MPI_Request requests[numtasks-1];
	MPI_Status status;
	int rowptrsize = matrix->rowptrsize;
	int rows_per_iter = (rowptrsize / (4*(numtasks-1)))+1;
	int nextRow = (numtasks-1)*rows_per_iter;
	int requestRank, index, i;
	for(i = 1; i < numtasks; i++)	// receive initial data sets
	{
		MPI_Irecv(&result->values[(i-1)*rows_per_iter], rows_per_iter, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &requests[i-1]);
	}
	while(nextRow != -1)
	{
		MPI_Waitany(numtasks-1, requests, &requestRank, &status);
		requestRank++;
		MPI_Send(&nextRow, 1, MPI_INT, requestRank, 0, MPI_COMM_WORLD);
		if(rowptrsize - nextRow < rows_per_iter)
			MPI_Irecv(&result->values[nextRow], rows_per_iter, MPI_DOUBLE, requestRank, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[requestRank-1]);
		else
			MPI_Irecv(&result->values[nextRow], rowptrsize - nextRow, MPI_DOUBLE, requestRank, MPI_ANY_TAG, MPI_COMM_WORLD, &requests[requestRank-1]);
		nextRow = (nextRow+rows_per_iter < rowptrsize ? nextRow + rows_per_iter : -1);
	}
	for(i = 1; i < numtasks; i++)	// send final -1 value
		MPI_Send(&nextRow, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	MPI_Waitall(numtasks-1, requests, MPI_STATUSES_IGNORE);	// wait for all the processes to finish
}
