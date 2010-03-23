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

quadTree *read_input(char *filename, int *num_particles)
{
	int i;
	FILE *file = openFile(filename,"r");
	fscanf(file,"%d", num_particles);	// number of bodies
	quadTree *particles = (quadTree *)malloc(sizeof(quadTree) * *num_particles);

	double xPosition, yPosition, mass, xVelocity, yVelocity;

	for(i = 0; i < n; i++)
	{
		fscanf(file, "%lf %lf %lf %lf %lf", &particles[i].xPosition, &particles[i].yPosition, &particles[i].mass, &particles[i].xVelocity, &particles[i].yVelocity);
	}

	fclose(file);
	return particles;
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

