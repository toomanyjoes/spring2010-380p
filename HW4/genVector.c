#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	// 1: size   2: min   3: max
	int size = atoi(argv[1]);
	double min = atof(argv[2]);
	double max = atof(argv[3]);
	srand(time(NULL));
	printf("%d\n",size);
	int i;
	for(i=0;i<size;i++)
	{
		double num = min + ( (max-min) * rand() / ( RAND_MAX + 1.0 ) );
		printf("%lf ",num);
	}
	printf("\n");
	return 0;
}
