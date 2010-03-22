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

void updateVelocitiesAndPositions(quadTree *tree, quadTree *root, double timestep);
void computeForce(quadTree *particle, quadTree *tree, forceVector *force);
void forceFormula(quadTree *particle1, quadTree *particle2, forceVector *result);

int main(int argc, char **argv)
{
	if(argc != 5)
	{
		printf("Usage:\n   %s [iterations (double)] [timestep (double)] [input file] [output file]\n",argv[0]);
		exit(1);
	}
	double iterations = atof(argv[1]);
	double timestep = atof(argv[2]);
	quadTree *tree = read_input(argv[3]);
	char *outputfile = argv[4];

	int i;
	for(i=0; i < iterations; i++)
	{
		updateVelocitiesAndPositions(tree, tree, timestep);
//		tree = buildTree(tree);
//		freeQuadTree(tree);
	}
	
	write_output(outputfile, tree);
	freeQuadTree(tree);
	return 0;
}

void updateVelocitiesAndPositions(quadTree *tree, quadTree *root, double timestep)
{
	if(tree == 0)
		return;
	if(hasChildren(tree))
	{
		updateVelocitiesAndPositions(tree->topLeft, root, timestep);
		updateVelocitiesAndPositions(tree->topRight, root, timestep);
		updateVelocitiesAndPositions(tree->bottomLeft, root, timestep);
		updateVelocitiesAndPositions(tree->bottomRight, root, timestep);
	}
	else
	{
		forceVector force;
		force.xMagnitude = force.yMagnitude = 0.0;
		computeForce(tree, root, &force);
		double oldX = tree->xVelocity;
		double oldY = tree->yVelocity;
		tree->xVelocity += (force.xMagnitude / tree->mass) * timestep;	// change in velocity = acceleration * change in time
		tree->yVelocity += (force.yMagnitude / tree->mass) * timestep;
		tree->xPosition += (oldX + tree->xVelocity) / 2.0 * timestep;
		tree->yPosition += (oldY + tree->yVelocity) / 2.0 * timestep;
	}
}

void computeForce(quadTree *particle, quadTree *tree, forceVector *force)
{
	if(tree == 0 || tree == particle) return;
	if(!hasChildren(tree))
	{
		forceFormula(particle, tree, force);
	}
	else
	{
		double r = sqrt(  (tree->xPosition - particle->xPosition) * (tree->xPosition - particle->xPosition)
				+ (tree->yPosition - particle->yPosition) * (tree->yPosition - particle->yPosition));
		double D = tree->xTopRight - tree->xBottomLeft;		// size of bounding region
		if(D/r < THETA)
			forceFormula(particle, tree, force);
		else
		{
			computeForce(particle, tree->topLeft, force);
			computeForce(particle, tree->topRight, force);
			computeForce(particle, tree->bottomLeft, force);
			computeForce(particle, tree->bottomRight, force);
		}
	}
}

void forceFormula(quadTree *particle1, quadTree *particle2, forceVector *result)
{
	if(particle2 == 0) return;
	double r_sqrd = (particle2->xPosition - particle1->xPosition) * (particle2->xPosition - particle1->xPosition)
			+ (particle2->yPosition - particle1->yPosition) * (particle2->yPosition - particle1->yPosition);
	double r = sqrt(r_sqrd);
	double magnitude = (G * particle1->mass * particle2->mass) / r_sqrd;
	result->xMagnitude += (particle2->xPosition - particle1->xPosition) / r * magnitude;
	result->yMagnitude += (particle2->yPosition - particle1->yPosition) / r * magnitude;
}
