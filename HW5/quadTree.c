#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include "quadTree.h"

void buildTreeHelper(quadTree *oldTree, quadTree *newTree);
quadTree *allocNewNode(int q, quadTree *tree, body *particle);

// void swap(body *a, body *b)
// {
// 	body *t = a;
// 	a = b;
// 	b = t;
// }
// void sort(body *arr[], int beg, int end)
// {
// 	if (end > beg + 1)
// 	{
// 		uint64_t piv = arr[beg]->mortonNumber, l = beg + 1, r = end;
// 		while (l < r)
// 		{
// 			if (arr[l]->mortonNumber <= piv)
// 				l++;
// 			else
// 				swap(arr[l], arr[--r]);
// 		}
// 		swap(arr[--l], arr[beg]);
// 		sort(arr, beg, l);
// 		sort(arr, r, end);
// 	}
// }

// // returns true if p < q in the Morton ordering
// int compare(body *p, body *q)
// {
// 	int x = 0;
// 	int dim = 0;
// 	int i;
// 	for(i = 0; i < 2; i++)
// 		int y = XORmsb(p->xPosition, q->xPosition);
// 	if(x < y)
// 		x = y;
// 		dim = i;
// }
// 
// int XORmsb(double a, double b)
// {
// 	int x = extractExponent(a);
// 	int y = extractExponent(b);
// 	if(x == y)
// 	{
// 		int z = msdb(extractMantissa(a), extractMantissa(b));
// 		x = x - extract exponent(z);
// 		return x;
// 	}
// 	if(y < x)
// 		return x;
// 	return y;
// }

// body **mortonOrder(body *particles, int num_particles)
// {
// 	// source: http://www.cs.utk.edu/~vose/c-stuff/bithacks.html#InterleaveTableObvious
// 	int32_t x;
// 	int32_t y;
// 	uint64_t z[num_particles]; // z gets the resulting 32-bit Morton Number.
// 	body **orderedParticles = (body **)malloc(sizeof(body *) * num_particles);
// 
// 	int32_t i;
// 	int j;
// 	for(j = 0; j < num_particles; j++)
// 	{
// 		x = (int32_t)particles[j].xPosition;
// 		y = (int32_t)particles[j].yPosition;
// 		z[j] = 0;
// 		for(i = 0; i < sizeof(x) * 8; i++)
// 		{
// 			z[j] |= (x & 1 << i) << i | (y & 1 << i) << (i + 1);
// 		}
// // 		printf("x: %d y:%d z: %x\n",x,y,(unsigned int)z[j]);
// 		particles[j].mortonNumber = z[j];
// 		orderedParticles[j] = &particles[j];
// 	}
// 	sort(orderedParticles, 0, num_particles-1);
// printf("\n");
// 	return orderedParticles;
// }

void addBody(quadTree *tree, body *particle)
{
	if(tree->mass > 0.0)	// region already has a particle
	{
		if(!hasChildren(tree))	// leaf node, create new node to move it down the tree
		{
			if(tree->xPosition < (tree->xBottomLeft + (tree->size/2.0)))	// left half
			{
				if(tree->yPosition < (tree->yBottomLeft + (tree->size/2.0)))	// bottom half
				{
					tree->bottomLeft = allocNewNode(BOTTOM_LEFT, tree, tree->particle);
				}
				else	// top half
				{
					tree->topLeft = allocNewNode(TOP_LEFT, tree, tree->particle);
				}
			}
			else	// right half
			{
				if(tree->yPosition < (tree->yBottomLeft + (tree->size/2.0)))	// bottom half
				{
					tree->bottomRight = allocNewNode(BOTTOM_RIGHT, tree, tree->particle);
				}
				else	// top half
				{
					tree->topRight = allocNewNode(TOP_RIGHT, tree, tree->particle);
				}
			}
		}
		tree->xPosition = (particle->xPosition * particle->mass + tree->xPosition * tree->mass) / (tree->mass + particle->mass);
		tree->yPosition = (particle->yPosition * particle->mass + tree->yPosition * tree->mass) / (tree->mass + particle->mass);
		tree->mass += particle->mass;
// 		printf("new tree mass: %lf\n",tree->mass);

		double xMid = tree->xBottomLeft + (tree->size/2.0);
		double yMid = tree->yBottomLeft + (tree->size/2.0);
		if(particle->xPosition < xMid)	// left half
		{
			if(particle->yPosition < yMid)	// bottom half
			{
				if(tree->bottomLeft == 0)
				{
					tree->bottomLeft = allocNewNode(BOTTOM_LEFT, tree, particle);
				}
				else
					addBody(tree->bottomLeft, particle);
			}
			else	// top half
			{
				if(tree->topLeft == 0)
				{
					tree->topLeft = allocNewNode(TOP_LEFT, tree, particle);
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
					tree->bottomRight = allocNewNode(BOTTOM_RIGHT, tree, particle);
				}
				else
					addBody(tree->bottomRight, particle);
			}
			else	// top half
			{
				if(tree->topRight == 0)
				{
					tree->topRight = allocNewNode(TOP_RIGHT, tree, particle);
				}
				else
					addBody(tree->topRight, particle);
			}
		}
	}
	else	// the region only has this particle
	{
		tree->xBottomLeft = MIN_GRID;
		tree->yBottomLeft = MIN_GRID;
		tree->size = MAX_GRID - MIN_GRID;
		tree->xPosition = particle->xPosition;
		tree->yPosition = particle->yPosition;
		tree->mass = particle->mass;
		tree->particle = particle;
	}
}

int hasChildren(quadTree *tree)
{
	if(tree->topLeft || tree->topRight || tree->bottomLeft || tree->bottomRight)
		return 1;
	return 0;
}

quadTree *allocNewNode(int q, quadTree *tree, body *particle)
{
	quadTree *newNode = (quadTree *)malloc(sizeof(quadTree));
	memset(newNode,0,sizeof(quadTree));
	double xMid = tree->xBottomLeft + (tree->size/2.0);
	double yMid = tree->yBottomLeft + (tree->size/2.0);
	switch(q)
	{
		case TOP_RIGHT:
			newNode->xBottomLeft = xMid;
	 		newNode->yBottomLeft = yMid;
		break;
		case TOP_LEFT:
			newNode->xBottomLeft = tree->xBottomLeft;
	 		newNode->yBottomLeft = yMid;
		break;
		case BOTTOM_RIGHT:
			newNode->xBottomLeft = xMid;
	 		newNode->yBottomLeft = tree->yBottomLeft;
		break;
		case BOTTOM_LEFT:
			newNode->xBottomLeft = tree->xBottomLeft;
	 		newNode->yBottomLeft = tree->yBottomLeft;
	}
	newNode->size = tree->size / 2.0;
	newNode->xPosition = particle->xPosition;
	newNode->yPosition = particle->yPosition;
	newNode->mass = particle->mass;
// 	printf("newNode->mass: %lf\n",newNode->mass);
	newNode->xVelocity = particle->xVelocity;
	newNode->yVelocity = particle->yVelocity;
	newNode->particle = particle;
	return newNode;
}

quadTree *buildTree(body *particles, int num_particles, int rank, int numtasks)
{
/*	int first, last;
	first = rank * (num_particles/numtasks);
	if(rank != numtasks - 1)
		last = ((rank + 1) * (num_particles/numtasks)) - 1;
	else
		last = num_particles - 1;
*/
	quadTree *newTree = (quadTree *)malloc(sizeof(quadTree));
	newTree->xBottomLeft = MIN_GRID;
	newTree->yBottomLeft = MIN_GRID;
	newTree->size = MAX_GRID - MIN_GRID;
	newTree->mass = 0.0;
	newTree->topLeft = newTree->topRight = newTree->bottomLeft = newTree->bottomRight = 0;
	
	int i;
// 	printf("num_particles: %d\n",num_particles);
//	printf("proc %d  first: %d  last: %d\n",rank, first, last);

	for(i = 0; i < num_particles; i++)
	{
//  		printf("calling addbody %d\n",i);
// 		printf("xPos: %lf yPos: %lf mass: %lf xv: %lf yv: %lf\n",particles[i].xPosition,particles[i].yPosition,particles[i].mass,particles[i].xVelocity,particles[i].yVelocity);
		addBody(newTree, &particles[i]);
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
