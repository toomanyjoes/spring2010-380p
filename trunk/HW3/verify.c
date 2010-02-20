#include <stdlib.h>
#include <math.h>
#include <bebop/smc/csr_matrix.h>
#include "matrixmult.h"
#include "inout.h"

int vectorsEqual(vector *A, vector *B)
{
	if(A->rows != B->rows) return 0;
	int i;
	for(i=0;i<A->rows;i++)
		if(fabs(A->values[i] - B->values[i]) > 0.000001)
			return 0;
	return 1;
}

int verifyMult(csr_matrix_t *matrix, vector *vec, vector *comparison)
{
	int i,j,k;
	vector result;
	result.rows = matrix->m;
	result.values = (double *)malloc(sizeof(double)*result.rows);
	for(i = 0; i < matrix->m; i++)
	{
		result.values[i] = 0.0;
		for(k = matrix->rowptr[i]; k < matrix->rowptr[i+1]; k++)
		{
			j = matrix->colidx[k];
			result.values[i] += ((double *)matrix->values)[k] * vec->values[j];
		}
	}
	int return_result = vectorsEqual(&result, comparison);
	//write_vector("errVector.txt",&result);
	free(result.values);
	return return_result;
}

// csr_matrix_t *vector_to_csr(vector *v)
// {
// 	int i;
// 	int *colidx = (int *)malloc(sizeof(int)*v->rows);
// 	int *rowptr = (int *)malloc(sizeof(int)*v->rows);
// 	for(i=0; i<v->rows;i++)
// 	{
// 		colidx[i] = 0;
// 		rowptr[i] = i;
// 	}
// 	return create_csr_matrix(
// 		v->rows,	// n
// 		1,		// m
// 		v->rows,	// nnz
// 		v->values,	// values
// 		colidx,
// 		rowptr,
// 		UNSYMMETRIC,	// symmetry_type
// 		UPPER_TRIANGLE,	// symmetric_storage_location  ignore
// 		REAL,		// value_type,
// 		USER_DEALLOCATES, // ownership,
// 		NULL,		// void(*)(void *) deallocator,
// 		NO_COPY		// copy_mode	 
// 	);
// }
// 
// vector *csr_to_vector(csr_matrix_t *matrix)
// {
// 	int i;
// 	if(matrix->n != 1)
// 	{
// 		printf("vector has more than one column\n");
// 		return NULL;
// 	}
// 	vector *result = (vector *)malloc(sizeof(vector));
// 	result->values = (double *)malloc(sizeof(double)*matrix->m);
// 	result->rows = matrix->m;
// 	for(i = 0; i < result->rows; i++)
// 	{
// 		if(matrix->rowptr[i] != i)
// 			result->values[i] = 0.0;
// 		else
// 			result->values[i] = ((double *)matrix->values)[i];
// 	}
// 	return result;
// }
