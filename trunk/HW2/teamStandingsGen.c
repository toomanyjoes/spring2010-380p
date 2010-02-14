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
	int numTeams = atoi(argv[1]);
	int numGames = atoi(argv[2]);
	FILE *infile = openFile(argv[3]); // input file
	fprintf(infile,"%d\n%d\n",numTeams,numGames);
	FILE *outfile = openFile(argv[4]); // expected output
	int *array = (int *)malloc(sizeof(int)*numGames);
	int i;
	for(i=0;i<numGames;i++)
		array[i] = 0;
	srand(time(NULL));
	
	int ahead = 0;
	int aheadIndex = 0;
	for(i=0;i<numGames;i++) {
		int num = (int)( (double)numTeams * rand() / ( RAND_MAX + 1.0 ) );
		fprintf(infile,"%d ",num);
		array[num]++;
		if(array[num] == ahead)
			aheadIndex = 0; // tie. output 0
		else if(array[num] > ahead) {
			ahead = array[num];  // team "num" goes ahead
			aheadIndex = num;
		}
		fprintf(outfile,"%d ",aheadIndex);
	}
	fprintf(outfile,"\n");
	fclose(infile);
	fclose(outfile);
	return 0;
}

