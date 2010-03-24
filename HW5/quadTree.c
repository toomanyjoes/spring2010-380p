#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include "quadTree.h"

void buildTreeHelper(quadTree *oldTree, quadTree *newTree);
quadTree *allocNewNode(int q, quadTree *tree);

void swap(quadTree *a, quadTree *b)
{
	quadTree *t = a;
	a = b;
	b = t;
}
void sort(quadTree *arr[], int beg, int end)
{
	if (end > beg + 1)
	{
		uint64_t piv = arr[beg]->mortonNumber, l = beg + 1, r = end;
		while (l < r)
		{
			if (arr[l]->mortonNumber <= piv)
				l++;
			else
				swap(arr[l], arr[--r]);
		}
		swap(arr[--l], arr[beg]);
		sort(arr, beg, l);
		sort(arr, r, end);
	}
}

quadTree **mortonOrder(quadTree *particles, int num_particles)
{
	// source: http://www.cs.utk.edu/~vose/c-stuff/bithacks.html#InterleaveTableObvious
	int32_t x;
	int32_t y;
	uint64_t z[num_particles]; // z gets the resulting 32-bit Morton Number.
	quadTree **orderedParticles = (quadTree **)malloc(sizeof(quadTree *) * num_particles);

	int32_t i;
	int j;
	for(j = 0; j < num_particles; j++)
	{
		x = (int32_t)particles[j].xPosition;
		y = (int32_t)particles[j].yPosition;
		z[j] = 0;
		for(i = 0; i < sizeof(x) * 8; i++)
		{
			z[j] |= (x & 1 << i) << i | (y & 1 << i) << (i + 1);
		}
		particles[j].mortonNumber = z[j];
		orderedParticles[j] = &particles[j];
	}
	sort(orderedParticles, 0, num_particles-1);

	return orderedParticles;
}

void addBody(quadTree *tree, quadTree *particle)
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
		tree->xPosition = (particle->xPosition * particle->mass + tree->xPosition * tree->mass) / (tree->mass + particle->mass);
		tree->yPosition = (particle->yPosition * particle->mass + tree->yPosition * tree->mass) / (tree->mass + particle->mass);
		tree->mass += particle->mass;

		double xMid = (tree->xTopRight + tree->xBottomLeft) / 2.0;
		double yMid = (tree->yTopRight + tree->yBottomLeft) / 2.0;
		if(particle->xPosition < xMid)	// left half
		{
			if(particle->yPosition < yMid)	// bottom half
			{
				if(tree->bottomLeft == 0)
				{
					particle->xBottomLeft = tree->xBottomLeft;
	 				particle->yBottomLeft = tree->yBottomLeft;
					particle->xTopRight = xMid;
		 			particle->yTopRight = yMid;
					tree->bottomLeft = particle;
				}
				else
					addBody(tree->bottomLeft, particle);
			}
			else	// top half
			{
				if(tree->topLeft == 0)
				{
					particle->xBottomLeft = tree->xBottomLeft;
					particle->yBottomLeft = yMid;
					particle->xTopRight = xMid;
					particle->yTopRight = tree->yTopRight;
					tree->topLeft = particle;
				}
				else
					addBody(tree->topLeft, particle);
			}
		}
		else	// right half
		{
			if(particle->yPosition < yMid)	// bottom half
			{
				if(tree->bottomRight == 0)
				{
					particle->xBottomLeft = xMid;
					particle->yBottomLeft = tree->yBottomLeft;
					particle->xTopRight = tree->xTopRight;
					particle->yTopRight = yMid;
					tree->bottomRight = particle;
				}
				else
					addBody(tree->bottomRight, particle);
			}
			else	// top half
			{
				if(tree->topRight == 0)
				{
					particle->xBottomLeft = xMid;
					particle->yBottomLeft = yMid;
					particle->xTopRight = tree->xTopRight;
					particle->yTopRight = tree->yTopRight;
					tree->topRight = particle;
				}
				else
					addBody(tree->topRight, particle);
			}
		}
	}
	else	// the region only has this particle
	{
		particle->xBottomLeft = MIN_GRID;
		particle->yBottomLeft = MIN_GRID;
		particle->xTopRight = MAX_GRID;
		particle->yTopRight = MAX_GRID;
		tree = particle;
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

quadTree *buildTree(quadTree **particles, int num_particles, int rank, int numtasks, int subdomain, int maxSubdomain)
{
	int first, last;
	first = rank * (num_particles/numtasks) * subdomain;
	if(rank != numtasks - 1 && subdomain != maxSubdomain)
		last = ((rank + 1) * (num_particles/numtasks) * subdomain) - 1;
	else
		last = num_particles - 1;

	int size = (int) MAX_GRID / sqrt(SUBDOMAINS);

	quadTree *newTree = (quadTree *)malloc(sizeof(quadTree));
	newTree->xBottomLeft = MIN_GRID * rank * size;
	newTree->yBottomLeft = MAX_GRID - subdomain * size;
	newTree->xTopRight = newTree->xBottomLeft + size;
	newTree->yTopRight = newTree->yBottomLeft + size;
	newTree->mass = 0.0;
	newTree->topLeft = newTree->topRight = newTree->bottomLeft = newTree->bottomRight = 0;
	
	int i;
	for(i = first; i < last; i++)
	{
		addBody(newTree, particles[i]);
	}
	return newTree;
}


// quadTree *buildTree(quadTree *oldTree)
// {
// 	quadTree *newTree = (quadTree *)malloc(sizeof(quadTree));
// 
// 	newTree->xBottomLeft = oldTree->xBottomLeft;
// 	newTree->yBottomLeft = oldTree->yBottomLeft;
// 	newTree->xTopRight = oldTree->xTopRight;
// 	newTree->yTopRight = oldTree->yTopRight;
// 	newTree->mass = 0.0;
// 	newTree->topLeft = newTree->topRight = newTree->bottomLeft = newTree->bottomRight = 0;
// 	buildTreeHelper(oldTree, newTree);
// 	return newTree;
// }
// 
// void buildTreeHelper(quadTree *oldTree, quadTree *newTree)
// {
// 	if(oldTree == 0)
// 		return;
// 	if(hasChildren(oldTree))
// 	{
// 		buildTreeHelper(oldTree->topLeft, newTree);
// 		buildTreeHelper(oldTree->topRight, newTree);
// 		buildTreeHelper(oldTree->bottomLeft, newTree);
// 		buildTreeHelper(oldTree->bottomRight, newTree);
// 	}
// 	else
// 		addBody(newTree, oldTree->xPosition, oldTree->yPosition, oldTree->mass, oldTree->xVelocity, oldTree->yVelocity);
// }

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
