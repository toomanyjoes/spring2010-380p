#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
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

	struct timeval begin, end;
	gettimeofday(&begin, NULL);
	for(i = 0; i < matrix->m; i++)
	{
		result.values[i] = 0.0;
		for(k = matrix->rowptr[i]; k < matrix->rowptr[i+1]; k++)
		{
			j = matrix->colidx[k];
			result.values[i] += ((double *)matrix->values)[k] * vec->values[j];
		}
	}
	gettimeofday(&end, NULL);
	double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	printf("Sequential Time: %lf\n",endsec-beginsec);

	int return_result = vectorsEqual(&result, comparison);
	//write_vector("errVector.txt",&result);
	free(result.values);
	return return_result;
}
