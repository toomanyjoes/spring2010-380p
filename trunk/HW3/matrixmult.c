/*
 * matrixmult.c -- Assignment 3 Sparse Matrix-Vector Multiplication using Pthreads
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#include <bebop/smc/coo_matrix.h>
#include <bebop/smc/csr_matrix.h>
#include <bebop/smc/read_mm.h>
#include "inout.h"
#include "verify.h"
#define MAX_THREAD 1000

void multiply(int num_threads, csr_matrix_t *inMatrix, vector *inVector, vector *out_result);
int getRowPtrIndex(csr_matrix_t *matrix);
void *thread_main(void *arg);

int *availableRows;
int nextRow, rows_per_iter;
pthread_mutex_t availableRowsLock;

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
	int num_threads;
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

	gettimeofday(&begin, NULL);
	multiply(num_threads, csrMatrix, &invector, &result);
	gettimeofday(&end, NULL);
	double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	printf("Time: %lf\n",endsec-beginsec);
	
	write_vector(outfile, &result);

	// Verify against a sequential multiplication calculation
	// This works if you want to run a sequential version of this algoirthm
	/*if(verifyMult(csrMatrix, &invector, &result) == 0)
	{
		printf("\nFailed to verify result!\n\n");
	}*/

	return 0;
}

void multiply(int num_threads, csr_matrix_t *inMatrix, vector *inVector, vector *out_result)
{
	int i;
	pthread_attr_t attr;
	pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t)*num_threads);
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);

	out_result->rows = inMatrix->m;
	out_result->values = (double *)malloc(sizeof(double)*out_result->rows);
	memset(out_result->values,0,sizeof(double)*out_result->rows);
	rows_per_iter = (inMatrix->rowptrsize / (4*num_threads))+1;
	nextRow = num_threads * rows_per_iter;
	thread_arg args[num_threads];
	for(i=0; i<num_threads; i++)
	{
		args[i].id = i;
		args[i].matrix = inMatrix;
		args[i].vec = inVector;
		args[i].result = out_result;
		pthread_create(&threads[i], &attr, thread_main, &args[i]);
	}
	pthread_attr_destroy(&attr);
	for(i=0; i<num_threads; i++)
		pthread_join(threads[i],NULL);
}

void *thread_main(void *arg)
{
	thread_arg *myArg = (thread_arg *)arg;
	csr_matrix_t *matrix = myArg->matrix;
	int rowIndex = myArg->id*rows_per_iter;
	int j,k;

	int *rowptr = matrix->rowptr;
	int *colidx = matrix->colidx;
	int rowptrsize = matrix->rowptrsize;
	double *mvalues = (double *)matrix->values;
	double *rvalues = myArg->result->values;
	double *vvalues = myArg->vec->values;
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
		rowIndex = getRowPtrIndex(matrix);	// get some more rows
	}
		
	return (void *)0;
}

int getRowPtrIndex(csr_matrix_t *matrix)
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
