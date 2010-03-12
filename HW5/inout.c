#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "inout.h"

void addBody(quadTree *tree, double xPosition, double yPosition, double mass, double xVelocity, double yVelocity);
quadTree *allocNewNode(int q, quadTree *tree);
int hasChildren(quadTree *tree);


FILE *openFile(char *fname, char *mode)
{
	FILE *file = fopen(fname, mode);
	if(!file) {
		fprintf(stderr,"Error opening file: %s\n", fname);
		exit(1);
	}
	return file;
}

void read_input(char *filename, quadTree *out_tree)
{
	int i;
	FILE *file = openFile(filename,"r");
	int n;
	fscanf(file,"%d", &n);	// n = number of bodies

	double xPosition, yPosition, xVelocity, yVelocity, mass;
	for(i = 0; i < n; i++)
	{
		fscanf(file, "%lf %lf %lf %lf %lf", &xPosition, &yPosition, &mass, &xVelocity, &yVelocity);
		addBody(out_tree, xPosition, yPosition, mass, xVelocity, yVelocity);
	}

	fclose(file);
}

void addBody(quadTree *tree, double xPosition, double yPosition, double mass, double xVelocity, double yVelocity)
{
	if(mass > 0.0)	// region already has a particle
	{
		tree->mass += mass;
		tree->xPosition = (xPosition * mass + tree->xPosition * tree->mass) / tree->mass;
		tree->yPosition = (yPosition * mass + tree->yPosition * tree->mass) / tree->mass;
		if(xPosition < ((tree->xTopRight + tree->xBottomLeft) / 2.0))	// left half
		{
			if(yPosition < ((tree->yTopRight + tree->yBottomLeft) / 2.0))	// bottom half
			{
				if(tree->bottomLeft == 0)
					tree->bottomLeft = allocNewNode(BOTTOM_LEFT, tree);
				addBody(tree->bottomLeft, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
			else	// top half
			{
				if(tree->topLeft == 0)
					tree->topLeft = allocNewNode(TOP_LEFT, tree);
				addBody(tree->topLeft, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
		}
		else	// right half
		{
			if(yPosition < ((tree->yTopRight + tree->yBottomLeft) / 2.0))	// bottom half
			{
				if(tree->bottomRight == 0)
					tree->bottomRight = allocNewNode(BOTTOM_RIGHT, tree);
				addBody(tree->bottomRight, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
			else	// top half
			{
				if(tree->topRight == 0)
					tree->topRight = allocNewNode(TOP_RIGHT, tree);
				addBody(tree->topRight, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
		}
	}
	else	// the region only has this particle
	{
		tree->mass += mass;
		tree->xPosition = xPosition;
		tree->yPosition = yPosition;
	}
	
	
}

int hasChildren(quadTree *tree)
{
	if(tree->topLeft || tree->topRight || tree->bottomLeft || tree->bottomRight)
		return 1;
	return 0;
}

quadTree *allocNewNode(int q, quadTree *tree)
{
	quadTree *newNode = malloc(sizeof(quadTree));
	memset(newNode,0,sizeof(quadTree));
	double xMid = (tree->xTopRight + tree->xBottomLeft) / 2.0;
	double yMid = (tree->yTopRight + tree->yBottomLeft) / 2.0;
	switch(q)
	{
		case TOP_RIGHT:
			newNode->xBottomLeft = xMid;
	 		newNode->yBottomLeft = yMid;
 			newNode->xTopRight = tree->xTopRight;
		 	newNode->yTopRight = tree->yTopRight;
		break;
		case TOP_LEFT:
			newNode->xBottomLeft = tree->xBottomLeft;
	 		newNode->yBottomLeft = yMid;
 			newNode->xTopRight = xMid;
		 	newNode->yTopRight = tree->yTopRight;
		break;
		case BOTTOM_RIGHT:
			newNode->xBottomLeft = xMid;
	 		newNode->yBottomLeft = tree->yBottomLeft;
 			newNode->xTopRight = tree->xTopRight;
		 	newNode->yTopRight = yMid;
		break;
		case BOTTOM_LEFT:
			newNode->xBottomLeft = tree->xBottomLeft;
	 		newNode->yBottomLeft = tree->yBottomLeft;
			newNode->xTopRight = xMid;
		 	newNode->yTopRight = yMid;
	}
	return newNode;
}

void write_output(char *filename, quadTree *tree)
{
	FILE *file = openFile(filename,"w");

	fclose(file);
}

