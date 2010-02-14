#include <stdio.h>
#include <stdlib.h>
#include <time.h>


FILE *openFile(char *fname)
{
	FILE *file = fopen(fname, "w");
	if(!file) {
		fprintf(stderr,"Error opening file: %s\n", fname);
		exit(1);
	}
	return file;
}

int main(int argc, char *argv[])
{
	int numValues = atoi(argv[1]);
	int stringLength = atoi(argv[2]);
	FILE *infile = openFile(argv[3]); // input file
	fprintf(infile,"%d\n%d\n",numValues,stringLength);
	FILE *outfile = openFile(argv[4]); // expected output
	int *array = (int *)malloc(sizeof(int)*numValues);
	int i;
	for(i=0;i<numValues;i++)
		array[i] = 0;
	srand(time(NULL));
	
	int index = 0;
	for(i=0;i<stringLength;i++) {
		int num = 1 + (int)( (double)numValues * rand() / ( RAND_MAX + 1.0 ) );
		fprintf(infile,"%d ",num);
		fprintf(outfile,"%d ",array[num-1]);
		array[num-1] = index++;
	}
	fprintf(outfile,"\n");
	fclose(infile);
	fclose(outfile);
	return 0;
}

