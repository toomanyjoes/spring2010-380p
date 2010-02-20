/*
 * matrixmult.c -- Assignment 3 Sparse Matrix-Vector Multiplication using Pthreads
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/types.h>
#include <pthread.h>
#include <bebop/smc/coo_matrix.h>
#include <bebop/smc/csr_matrix.h>
#include <bebop/smc/read_mm.h>
#include "inout.h"
#include "verify.h"
#define MAX_THREAD 1000

void multiply(int num_threads, csr_matrix_t *inMatrix, vector *inVector, vector *out_result);
int getRowPtrIndex(csr_matrix_t *matrix, int *lastrow);
void *thread_main(void *arg);

int *availableRows;
int nextRow;
pthread_mutex_t availableRowsLock = PTHREAD_MUTEX_INITIALIZER;

int main(int argc, char* argv[])
{
	if(argc != 4 && argc != 5)
	{
		printf("Usage:\n   %s [MMEF in file] [vector in file] [out file] <[num threads]>\n",argv[0]);
		exit(1);
	}
	vector invector,result;
	csr_matrix_t *csrMatrix;
	int num_threads;
	struct coo_matrix_t cooMatrix;
	read_MMEF(argv[1], &cooMatrix);
	//save_coo_matrix_in_matrix_market_format(argv[3],&inMatrix);
	csrMatrix = coo_to_csr(&cooMatrix);
	//print_csr_matrix_in_matrix_market_format(stdout,csrMatrix);
	read_vector(argv[2], &invector);
	//print_matrix(&invector);
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

	multiply(num_threads, csrMatrix, &invector, &result);
	
	write_vector(outfile, &result);

	// Verify against a sequential multiplication calculation
	if(verifyMult(csrMatrix, &invector, &result) == 0)
	{
		printf("\nFailed to verify result!\n\n");
	}

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
	thread_arg args[num_threads];
	for(i=0; i<num_threads; i++)
	{
		args[i].id = i;
		args[i].matrix = inMatrix;
		args[i].vec = inVector;
		args[i].result = out_result;
		args[i].num_threads = num_threads;
		pthread_create(&threads[i], &attr, thread_main, &args[i]);
	}
	for(i=0; i<num_threads; i++)
		pthread_join(threads[i],NULL);
}

void *thread_main(void *arg)
{
	thread_arg *myArg = (thread_arg *)arg;
	int id = myArg->id;
	csr_matrix_t *matrix = myArg->matrix;
	vector *vec = myArg->vec;
	vector *result = myArg->result;
	int num_threads = myArg->num_threads;
	int lastrow;
	int rowIndex = getRowPtrIndex(matrix, &lastrow);
	int i,j,k;
//	for(i = 0; i < vec->rows; i++)
//	{

// 	if(lastrow)
// 		for(k = matrix->rowptr[rowIndex]; k < vec->rows; k++)
// 		{
// 			j = matrix->colidx[k];
// 			result[rowIndex] += matrix->values[k]*vec->values[j];
// 		}
// 	else
	
	while(rowIndex != -1)
	{
		for(k = matrix->rowptr[rowIndex]; k < matrix->rowptr[rowIndex+1]; k++)
		{
			j = matrix->colidx[k];
			result->values[rowIndex] += ((double *)matrix->values)[k] * ((double *)vec->values)[j];
		}
		rowIndex = getRowPtrIndex(matrix, &lastrow);
	}
		
//	}
	return (void *)0;
}

int getRowPtrIndex(csr_matrix_t *matrix, int *lastrow)
{
	int myRow;
	pthread_mutex_lock(&availableRowsLock);
	{
		myRow = nextRow;
		if(nextRow != -1)
		{
			if(matrix->rowptr[nextRow+1] == matrix->nnz)  // Bebop fills unused rowptr entries with nnz
			{
				nextRow = -1;
				*lastrow = 1;
			}
			else
				nextRow++;
		}
	}
	pthread_mutex_unlock(&availableRowsLock);
	return myRow;	
}
