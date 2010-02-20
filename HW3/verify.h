#ifndef VERIFY_H_
#define VERIFY_H_

int verifyMult(csr_matrix_t *matrix, vector *vec, vector *comparison);
int vectorsEqual(vector *A, vector *B);
//csr_matrix_t *vector_to_csr(vector *v);
//vector *csr_to_vector(csr_matrix_t *lib_result);

#endif
