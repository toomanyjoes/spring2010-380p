/*
	Assignment 5: MPI Barnes-Hut
	Samuel Palmer, Joe Elizondo
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "inout.h"
#include "barneshut.h"
#include "quadTree.h"

void updateVelocities(quadTree *tree, quadTree *root, double timestep);
void updatePositions(quadTree *tree, double timestep);
void compute_accln(quadTree *particle, quadTree *tree, two_d_vector *accel);
void acclnFormula(quadTree *particle1, quadTree *particle2, two_d_vector *accel);

int main(int argc, char **argv)
{
	if(argc != 5)
	{
		printf("Usage:\n   %s [iterations (double)] [timestep (double)] [input file] [output file]\n",argv[0]);
		exit(1);
	}
	int numtasks,rank;
	MPI_Init(NULL,NULL);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

	struct timeval begin, end;

	double iterations = atof(argv[1]);
	double timestep = atof(argv[2]);
	quadTree *particles = read_input(argv[3]);

	MPI_Barrier(); // wait for input to finish
	gettimeofday(&begin, NULL);

	quadTree *oldTree;
	char *outputfile = argv[4];

	int i;
	for(i=0; i < iterations; i++)
	{
		updateVelocities(tree, tree, timestep);
		updatePositions(tree, timestep);
		oldTree = tree;
		tree = buildTree(tree);
		freeQuadTree(oldTree);
	}
	if(rank != 0)
		exit(0);
	gettimeofday(&end, NULL);
	double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	printf("Time: %lf\n",endsec-beginsec);
	
	write_output(outputfile, tree);
	freeQuadTree(tree);
	return 0;
}

void updateVelocities(quadTree *tree, quadTree *root, double timestep)
{
	if(tree == 0)
		return;
	if(hasChildren(tree))
	{
		updateVelocities(tree->topLeft, root, timestep);
		updateVelocities(tree->topRight, root, timestep);
		updateVelocities(tree->bottomLeft, root, timestep);
		updateVelocities(tree->bottomRight, root, timestep);
	}
	else
	{
		two_d_vector accel;
		accel.xMagnitude = accel.yMagnitude = 0.0;
		compute_accln(tree, root, &accel);

		tree->xVelocity += accel.xMagnitude * timestep * DAMPING;
		tree->yVelocity += accel.yMagnitude * timestep * DAMPING;
	}
}

void updatePositions(quadTree *tree, double timestep)
{
	if(tree == 0) return;
	if(hasChildren(tree))
	{
		updatePositions(tree->topLeft, timestep);
		updatePositions(tree->topRight, timestep);
		updatePositions(tree->bottomLeft, timestep);
		updatePositions(tree->bottomRight, timestep);
	}
	else
	{
		tree->xPosition += tree->xVelocity * timestep;
		tree->yPosition += tree->yVelocity * timestep;
	}
}

void compute_accln(quadTree *particle, quadTree *tree, two_d_vector *accel)
{
	if(tree == 0 || tree == particle) return;
	if(!hasChildren(tree))
	{
		acclnFormula(particle, tree, accel);
	}
	else
	{
		double r = sqrt(  (tree->xPosition - particle->xPosition) * (tree->xPosition - particle->xPosition)
				+ (tree->yPosition - particle->yPosition) * (tree->yPosition - particle->yPosition));
		double D = tree->xTopRight - tree->xBottomLeft;		// size of bounding region
		if(D/r < THETA)
		{
			acclnFormula(particle, tree, accel);
		}
		else
		{
			compute_accln(particle, tree->topLeft, accel);
 			compute_accln(particle, tree->topRight, accel);
 			compute_accln(particle, tree->bottomLeft, accel);
 			compute_accln(particle, tree->bottomRight, accel);
		}
	}
}

void acclnFormula(quadTree *particle1, quadTree *particle2, two_d_vector *accel)
{
	double x = particle2->xPosition;
	double y = particle2->yPosition;
        x -= particle1->xPosition;
        y -= particle1->yPosition;

        double dist = x*x + y*y;

        dist += SOFTEN; //softening factor helps in controlling the impact of one body over the other. assume a value of 18.

        dist = pow(dist, 3.0/2.0); //i.e dist to the power of 1.5
        double mag = particle2->mass / dist;

        accel->xMagnitude += mag*x;//G * mag*x;
        accel->yMagnitude += mag*y;//G * mag*y;
        return;
}

