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
csr_matrix_t myMatrix;

int main(int argc, char* argv[])
{
	if(argc != 4 && argc != 5)
	{
		printf("Usage:\n   %s [MMEF in file] [vector in file] [out file] <[num threads]>\n",argv[0]);
		exit(1);
	}
	matrix invector;
	//read_MMEF(argv[1], &inmatrix);
	//print_matrix(&inmatrix);
	coo_matrix_t inmatrix;
	read_MMEF(argv[1], &inmatrix);
	coo_to_csr (&myMatrix);
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
	
	write_vector(outfile, &invector);
	
//	freeMatrix(&inmatrix);
//	freeMatrix(&invector);
	return 0;
}

// void freeMatrix(matrix *m)
// {
// 	free(m->row_coord);
// 	free(m->col_coord);
// 	free(m->entry);
// }
