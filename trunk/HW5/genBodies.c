#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char **argv)
{
	srand(time(NULL));
	int bodyCount = atoi(argv[1]);
	printf("%d\n",bodyCount);

	double x, y, m;

	int i;
	for(i = 0; i < bodyCount; i++)
	{
		x = -8000.0 + (16000.0 * (rand() / (RAND_MAX + 1.0)));
		y = -8000.0 + (16000.0 * (rand() / (RAND_MAX + 1.0)));
		m = 1.0 + (3.0 * (rand() / (RAND_MAX + 1.0)));
		// xPosition yPosition mass xVelocity yVelocity
		printf("%lf %lf %lf %d %d\n",x,y,m,0,0);
	}
	return 0;
}
