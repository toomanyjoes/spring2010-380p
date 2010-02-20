#ifndef INOUT_H_
#define INOUT_H_

#include "matrixmult.h"

#define BUFSIZE 1024

void read_MMEF(char *filename, coo_matrix_t *out_matrix);
void read_vector(char *filename, vector *out_vector);
void write_vector(char *filename, vector *vec);

#endif
