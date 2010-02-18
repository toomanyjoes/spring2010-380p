#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <bebop/smc/read_mm.h>
#include "inout.h"
#include "mmio.h"

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
//	MM_typecode matcode;
//	int i;
//	FILE *file = openFile(filename,"r");
// 	if(mm_read_banner(file, &matcode) != 0 ||
// 	   mm_read_mtx_crd_size(file, &out_matrix->rows, &out_matrix->columns, &out_matrix->non_zero_entries) != 0)
// 	{
// 		fprintf(stderr, "Error reading MMEF file\n");
// 		if(file) fclose(file);
// 		exit(1);
// 	}
// 	
// // 	fscanf(file, "%d", &out_matrix->rows);
// // 	fscanf(file, "%d", &out_matrix->columns);
// // 	fscanf(file, "%d", &out_matrix->non_zero_entries);
// 	out_matrix->row_coord = (int *)malloc(sizeof(int)*out_matrix->non_zero_entries);
// 	out_matrix->col_coord = (int *)malloc(sizeof(int)*out_matrix->non_zero_entries);
// 	out_matrix->entry = (double *)malloc(sizeof(double)*out_matrix->non_zero_entries);
// 	for(i=0; i < out_matrix->non_zero_entries; i++)
// 	{
// 		fscanf(file, "%d", &out_matrix->row_coord[i]);
// 		fscanf(file, "%d", &out_matrix->col_coord[i]);
// 		fscanf(file, "%lf", &out_matrix->entry[i]);
// 	}
//	fclose(file);
	if(read_matrix_market_real_sparse(filename, out_matrix) != 0)
	{
		fprintf(stderr, "Error reading MMEF file\n");
		exit(1);
	}
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
		out_vector->row_coord[i] = i+1;
		out_vector->col_coord[i] = 1;
		fscanf(file, "%lf", &out_vector->entry[i]);
	}
	fclose(file);
}

void write_vector(char *filename, matrix *vec)
{
	int i;
	FILE *file = openFile(filename,"w");
	fprintf(file, "%d\n", vec->rows);
	for(i=0; i < vec->rows; i++)
		fprintf(file, "%lf ", vec->entry[i]);
	fclose(file);
}

void print_matrix(matrix *m)
{
	int i;
	printf("rows: %d columns: %d\n",m->rows,m->columns);
	for(i=0;i<m->non_zero_entries;i++)
		printf("(%d, %d): %lf\n",m->row_coord[i],m->col_coord[i],m->entry[i]);
}
