#ifndef MATRIXMULT_H_
#define MATRIXMULT_H_

typedef struct coo_matrix_t coo_matrix_t;
typedef struct csr_matrix_t csr_matrix_t;

typedef struct {
	int rows;		// number of vector rows
	double *values;		// array storing entries
} vector;

#endif
