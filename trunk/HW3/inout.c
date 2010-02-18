#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bebop/smc/read_mm.h>
#include "inout.h"

FILE *openFile(char *fname, char *mode)
{
	FILE *file = fopen(fname, mode);
	if(!file) {
		fprintf(stderr,"Error opening file: %s\n", fname);
		exit(1);
	}
	return file;
}

void read_MMEF(char *filename, coo_matrix_t *out_matrix)
{
	if(read_matrix_market_real_sparse(filename, out_matrix) != 0)
	{
		fprintf(stderr, "Error reading MMEF file\n");
		exit(1);
	}
}

void read_vector(char *filename, vector *out_vector)
{
	int i;
	FILE *file = openFile(filename,"r");
	fscanf(file, "%d", &out_vector->rows);
	out_vector->entry = (double *)malloc(sizeof(double)*out_vector->rows);
	for(i=0; i < out_vector->rows; i++)
	{
		fscanf(file, "%lf", &out_vector->entry[i]);
	}
	fclose(file);
}

void write_vector(char *filename, vector *vec)
{
	int i;
	FILE *file = openFile(filename,"w");
	fprintf(file, "%d\n", vec->rows);
	for(i=0; i < vec->rows; i++)
		fprintf(file, "%lf ", vec->entry[i]);
	fclose(file);
}


