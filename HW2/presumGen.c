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
//	int numGames = atoi(argv[2]);
	FILE *infile = openFile(argv[2]); // input file
	fprintf(infile,"%d\n",numValues);
	FILE *outfile = openFile(argv[3]); // expected output
	int *array = (int *)malloc(sizeof(int)*numValues);
	int i;
	for(i=0;i<numValues;i++)
		array[i] = 0;
	srand(time(NULL));
	
	int sum = 0;
	for(i=0;i<numValues;i++)
	{
		int num = 1 + (int)( 30.0 * rand() / ( RAND_MAX + 1.0 ) );
		fprintf(infile,"%d ",num);
		if(i == 0)
			array[i] = num;
		else
			array[i] = array[i-1] + num;
		fprintf(outfile,"%d ",array[i]);
	}
	fprintf(outfile,"\n");
	fclose(infile);
	fclose(outfile);
	return 0;
}
