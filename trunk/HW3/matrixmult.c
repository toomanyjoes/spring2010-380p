/*
 * matrixmult.c -- Assignment 3 Sparse Matrix-Vector Multiplication using Pthreads
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <pthread.h>
#include <bebop/smc/coo_matrix.h>
#include <bebop/smc/csr_matrix.h>
#include <bebop/smc/read_mm.h>
#include "inout.h"
#define MAX_THREAD 1000

int num_threads;
csr_matrix_t *csrMatrix;

int main(int argc, char* argv[])
{
	if(argc != 4 && argc != 5)
	{
		printf("Usage:\n   %s [MMEF in file] [vector in file] [out file] <[num threads]>\n",argv[0]);
		exit(1);
	}
	matrix invector;
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
	
	//write_vector(outfile, &invector);


	return 0;
}

