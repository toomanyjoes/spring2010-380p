#include <stdio.h>
#include <stdlib.h>
#include "array_funcs.h"


intArray *readArray(char *fname)
{
	FILE *file = fopen(fname, "r");
	if(!file) {
		fprintf(stderr,"Error opening file: %s\n", fname);
		exit(1);
	}
	intArray *array = (intArray*)malloc(sizeof(intArray));
	// first int is the size of the array
	array->size = 0;
	if(!fscanf(file,"%d",&(array->size))) {
		fprintf(stderr,"Error reading size of array\n");
		exit(1);
	}
	array->data = (int*)malloc(sizeof(int)*array->size);
	int i;
	for(i=0; i<array->size; i++) {
		if(!fscanf(file,"%d",array->data+i)) {
			fprintf(stderr,"Error reading element %d",i);
			exit(1);
		}
	}
	fclose(file);
	return array;
}
