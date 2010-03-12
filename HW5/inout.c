#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "inout.h"

void addBody(quadTree *tree, quadTree *root, double xPosition, double yPosition, double mass, double xVelocity, double yVelocity);
quadTree *allocNewNode(int q, quadTree *tree);
int hasChildren(quadTree *tree);
void write_bodies(FILE *file, quadTree *tree);
int count_bodies(FILE *file, quadTree *tree);


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

	int xMin = INT_MAX;
	int xMax = INT_MIN;
	int yMin = INT_MAX;
	int yMax = INT_MIN;
	body_list *list;
	list = malloc(sizeof(body_list));
	body_list *current = list;
	for(i = 0; i < n; i++)
	{
		fscanf(file, "%lf %lf %lf %lf %lf", &current->b.xPosition, &current->b.yPosition, &current->b.mass, &current->b.xVelocity, &current->b.yVelocity);
		if(current->b.xPosition < xMin)
			xMin = current->b.xPosition;
		else if(current->b.xPosition > xMax)
			xMax = current->b.xPosition;
		if(current->b.yPosition < yMin)
			yMin = current->b.yPosition;
		else if(current->b.yPosition > yMax)
			yMax = current->b.yPosition;
		if(i+1 < n)
		{
			current->next = malloc(sizeof(body_list));
			current = current->next;
		}
		else
			current->next = 0;
	}
	out_tree->xBottomLeft = xMin;
	out_tree->yBottomLeft = yMin;
	out_tree->xTopRight = xMax;
	out_tree->yTopRight = yMax;
	current = list;
	for(i = 0; i < n; i++)
	{
		addBody(out_tree, out_tree, current->b.xPosition, current->b.yPosition, current->b.mass, current->b.xVelocity, current->b.yVelocity);
		current = current->next;
		free(list);
		list = current;
	}
	fclose(file);
}

void addBody(quadTree *tree, quadTree *root, double xPosition, double yPosition, double mass, double xVelocity, double yVelocity)
{
	if(tree->mass > 0.0)	// region already has a particle
	{
		if(!hasChildren(tree))
		{
			if(tree->xPosition < ((tree->xTopRight + tree->xBottomLeft) / 2.0))	// left half
			{
				if(tree->yPosition < ((tree->yTopRight + tree->yBottomLeft) / 2.0))	// bottom half
				{
					tree->bottomLeft = allocNewNode(BOTTOM_LEFT, tree);
					tree->bottomLeft->xPosition = tree->xPosition;
					tree->bottomLeft->yPosition = tree->yPosition;
					tree->bottomLeft->mass = tree->mass;
					tree->bottomLeft->xVelocity = tree->xVelocity;
					tree->bottomLeft->yVelocity = tree->yVelocity;
				}
				else	// top half
				{
					tree->topLeft = allocNewNode(TOP_LEFT, tree);
					tree->topLeft->xPosition = tree->xPosition;
					tree->topLeft->yPosition = tree->yPosition;
					tree->topLeft->mass = tree->mass;
					tree->topLeft->xVelocity = tree->xVelocity;
					tree->topLeft->yVelocity = tree->yVelocity;
				}
			}
			else	// right half
			{
				if(tree->yPosition < ((tree->yTopRight + tree->yBottomLeft) / 2.0))	// bottom half
				{
					tree->bottomRight = allocNewNode(BOTTOM_RIGHT, tree);
					tree->bottomRight->xPosition = tree->xPosition;
					tree->bottomRight->yPosition = tree->yPosition;
					tree->bottomRight->mass = tree->mass;
					tree->bottomRight->xVelocity = tree->xVelocity;
					tree->bottomRight->yVelocity = tree->yVelocity;
				}
				else	// top half
				{
					tree->topRight = allocNewNode(TOP_RIGHT, tree);
					tree->topRight->xPosition = tree->xPosition;
					tree->topRight->yPosition = tree->yPosition;
					tree->topRight->mass = tree->mass;
					tree->topRight->xVelocity = tree->xVelocity;
					tree->topRight->yVelocity = tree->yVelocity;
				}
			}
		}
		tree->xPosition = (xPosition * mass + tree->xPosition * tree->mass) / (tree->mass + mass);
		tree->yPosition = (yPosition * mass + tree->yPosition * tree->mass) / (tree->mass + mass);
		tree->mass += mass;		

		if(xPosition < ((tree->xTopRight + tree->xBottomLeft) / 2.0))	// left half
		{
			if(yPosition < ((tree->yTopRight + tree->yBottomLeft) / 2.0))	// bottom half
			{
				if(tree->bottomLeft == 0)
				{
					tree->bottomLeft = allocNewNode(BOTTOM_LEFT, tree);
					tree->bottomLeft->xPosition = xPosition;
					tree->bottomLeft->yPosition = yPosition;
					tree->bottomLeft->mass = mass;
					tree->bottomLeft->xVelocity = xVelocity;
					tree->bottomLeft->yVelocity = yVelocity;
				}
				else
					addBody(tree->bottomLeft, root, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
			else	// top half
			{
				if(tree->topLeft == 0)
				{
					tree->topLeft = allocNewNode(TOP_LEFT, tree);
					tree->topLeft->xPosition = xPosition;
					tree->topLeft->yPosition = yPosition;
					tree->topLeft->mass = mass;
					tree->topLeft->xVelocity = xVelocity;
					tree->topLeft->yVelocity = yVelocity;
				}
				else
					addBody(tree->topLeft, root,  xPosition, yPosition, mass, xVelocity, yVelocity);
			}
		}
		else	// right half
		{
			if(yPosition < ((tree->yTopRight + tree->yBottomLeft) / 2.0))	// bottom half
			{
				if(tree->bottomRight == 0)
				{
					tree->bottomRight = allocNewNode(BOTTOM_RIGHT, tree);
					tree->bottomRight->xPosition = xPosition;
					tree->bottomRight->yPosition = yPosition;
					tree->bottomRight->mass = mass;
					tree->bottomRight->xVelocity = xVelocity;
					tree->bottomRight->yVelocity = yVelocity;
				}
				else
					addBody(tree->bottomRight, root, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
			else	// top half
			{
				if(tree->topRight == 0)
				{
					tree->topRight = allocNewNode(TOP_RIGHT, tree);
					tree->topRight->xPosition = xPosition;
					tree->topRight->yPosition = yPosition;
					tree->topRight->mass = mass;
					tree->topRight->xVelocity = xVelocity;
					tree->topRight->yVelocity = yVelocity;
				}
				else
					addBody(tree->topRight, root, xPosition, yPosition, mass, xVelocity, yVelocity);
			}
		}
	}
	else	// the region only has this particle
	{
		tree->mass += mass;
		tree->xPosition = xPosition;
		tree->yPosition = yPosition;
		tree->xVelocity = xVelocity;
		tree->yVelocity = yVelocity;
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
	int n = count_bodies(file, tree);
	fprintf(file, "%d\n", n);
	write_bodies(file, tree);
	fclose(file);
}

int count_bodies(FILE *file, quadTree *tree)
{
	if(!tree)
		return 0;
	if(!hasChildren(tree))
		return 1;
	return count_bodies(file, tree->bottomRight) + count_bodies(file, tree->bottomLeft) + count_bodies(file, tree->topRight) + count_bodies(file, tree->topLeft);
}

void write_bodies(FILE *file, quadTree *tree)
{
	if(!tree)
		return;
	if(!hasChildren(tree))
	{
		fprintf(file, "%lf %lf %lf %lf %lf\n", tree->xPosition, tree->yPosition, tree->mass, tree->xVelocity, tree->yVelocity);
		return;
	}
	write_bodies(file, tree->bottomRight);
	write_bodies(file, tree->bottomLeft);
	write_bodies(file, tree->topRight);
	write_bodies(file, tree->topLeft);
}

