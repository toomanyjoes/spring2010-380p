#include <stdlib.h>
#include <stdio.h>
#include <string.h>
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

void read_input(char *filename, quadTree *out_tree)
{
	int i;
	FILE *file = openFile(filename,"r");

	fclose(file);
}

void write_output(char *filename, quadTree *tree)
{
	int i;
	FILE *file = openFile(filename,"w");

	fclose(file);
}

