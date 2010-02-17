#include <stdlib.h>
#include <stdio.h>
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

void read_MMEF(char *filename, matrix *out_matrix)
{
	int i;
	FILE *file = openFile(filename,"r");
	fscanf(file, "%d", &out_matrix->rows);
	fscanf(file, "%d", &out_matrix->columns);
	fscanf(file, "%d", &out_matrix->non_zero_entries);
	out_matrix->row_coord = (int *)malloc(sizeof(int)*out_matrix->non_zero_entries);
	out_matrix->col_coord = (int *)malloc(sizeof(int)*out_matrix->non_zero_entries);
	out_matrix->entry = (double *)malloc(sizeof(double)*out_matrix->non_zero_entries);
	for(i=0; i < out_matrix->non_zero_entries; i++)
	{
		fscanf(file, "%d", &out_matrix->row_coord[i]);
		fscanf(file, "%d", &out_matrix->col_coord[i]);
		fscanf(file, "%lf", &out_matrix->entry[i]);
	}
	fclose(file);
}

void read_vector(char *filename, matrix *out_vector)
{
	int i;
	FILE *file = openFile(filename,"r");
	fscanf(file, "%d", &out_vector->rows);
	out_vector->columns = 1;
	out_vector->non_zero_entries = out_vector->rows;
	out_vector->row_coord = (int *)malloc(sizeof(int)*out_vector->non_zero_entries);
	out_vector->col_coord = (int *)malloc(sizeof(int)*out_vector->non_zero_entries);
	out_vector->entry = (double *)malloc(sizeof(double)*out_vector->non_zero_entries);
	for(i=0; i < out_vector->non_zero_entries; i++)
	{
		fscanf(file, "%d", &out_vector->row_coord[i]);
		fscanf(file, "%d", &out_vector->col_coord[i]);
		fscanf(file, "%lf", &out_vector->entry[i]);
	}
	fclose(file);
}

void write_vector(char *filename, matrix *vec)
{
	int i;
	FILE *file = openFile(filename,"w");
	for(i=0; i < vec->columns; i++)
		fprintf(file, "%lf ", vec->entry[i]);
	fclose(file);
}
