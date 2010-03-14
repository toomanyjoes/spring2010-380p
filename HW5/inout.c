#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "quadTree.h"
#include "inout.h"

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

quadTree *read_input(char *filename)
{
	int i;
	FILE *file = openFile(filename,"r");
	int n;
	fscanf(file,"%d", &n);	// n = number of bodies
	quadTree *out_tree = (quadTree *)malloc(sizeof(quadTree));

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
		addBody(out_tree, current->b.xPosition, current->b.yPosition, current->b.mass, current->b.xVelocity, current->b.yVelocity);
		current = current->next;
		free(list);
		list = current;
	}
	fclose(file);
	return out_tree;
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

