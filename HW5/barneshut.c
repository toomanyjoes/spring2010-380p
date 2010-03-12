/*
	Assignment 5: MPI Barnes-Hut
	Samuel Palmer, Joe Elizondo
*/
#include <stdlib.h>
#include <stdio.h>
#include "inout.h"
#include "barneshut.h"


int main(int argc, char **argv)
{
	if(argc != 5)
	{
		printf("Usage:\n   %s [iterations (double)] [timestep (double)] [input file] [output file]\n",argv[0]);
		exit(1);
	}
	double iterations = atof(argv[1]);
	double timestep = atof(argv[2]);
	quadTree tree;
	tree.xPosition = tree.yPosition = tree.xVelocity = tree.yVelocity = tree.mass = 0.0;
	tree.topLeft = tree.topRight = tree.bottomLeft = tree.bottomRight = 0;
	read_input(argv[3], &tree);
	char *outputfile = argv[4];

	
	
	return 0;
}
