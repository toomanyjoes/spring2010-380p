#include "matrixmult.h"

#define BUFSIZE 1024

void read_MMEF(char *filename, matrix *out_matrix);
void read_vector(char *filename, matrix *out_vector);
void write_vector(char *filename, matrix *vec);
void print_matrix(matrix *m);
