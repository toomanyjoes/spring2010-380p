#include <stdlib.h>
#include <string.h>
#include "quadTree.h"


void addBody(quadTree *tree, double xPosition, double yPosition, double mass, double xVelocity, double yVelocity)
{
	if(tree->mass > 0.0)	// region already has a particle
	{
		if(!hasChildren(tree))	// leaf node, create new node to move it down the tree
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
					addBody(tree->bottomLeft, xPosition, yPosition, mass, xVelocity, yVelocity);
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
					addBody(tree->topLeft, xPosition, yPosition, mass, xVelocity, yVelocity);
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
					addBody(tree->bottomRight, xPosition, yPosition, mass, xVelocity, yVelocity);
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
					addBody(tree->topRight, xPosition, yPosition, mass, xVelocity, yVelocity);
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
	quadTree *newNode = (quadTree *)malloc(sizeof(quadTree));
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

void freeQuadTree(quadTree *tree)
{
	if(tree == 0)
		return;
	freeQuadTree(tree->topLeft);
	freeQuadTree(tree->topRight);
	freeQuadTree(tree->bottomLeft);
	freeQuadTree(tree->bottomRight);
	free(tree);
}
