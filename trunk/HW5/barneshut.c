/*
	Assignment 5: MPI Barnes-Hut
	Samuel Palmer, Joe Elizondo
*/
#include <stdlib.h>
#include <stdio.h>
#include "inout.h"
#include "barneshut.h"
#include "quadTree.h"


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
		updateVelocities(tree, timestep);
	}
	
	write_output(outputfile, tree);
	freeQuadTree(tree);
	return 0;
}

void updateVelocities(quadTree *tree, double timestep)
{
	if(tree == 0)
		return;
	if(hasChildren(tree))
	{
		updateVelocities(tree->topLeft);
		updateVelocities(tree->topRight);
		updateVelocities(tree->bottomLeft);
		updateVelocities(tree->bottomRight);
	}
	else
	{
		double force = 
	}
}
