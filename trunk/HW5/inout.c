#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "quadTree.h"
#include "inout.h"

// void write_bodies(FILE *file, quadTree *tree);
// int count_bodies(quadTree *tree);


FILE *openFile(char *fname, char *mode)
{
	FILE *file = fopen(fname, mode);
	if(!file) {
		fprintf(stderr,"Error opening file: %s\n", fname);
		exit(1);
	}
	return file;
}

body *read_input(char *filename, int *num_particles)
{
	int i;
	FILE *file = openFile(filename,"r");
	fscanf(file,"%d", num_particles);	// number of bodies
	body *particles = (body *)malloc(sizeof(body) * *num_particles);

	for(i = 0; i < *num_particles; i++)
	{
		fscanf(file, "%lf %lf %lf %lf %lf", &particles[i].xPosition, &particles[i].yPosition, &particles[i].mass, &particles[i].xVelocity, &particles[i].yVelocity);
	}

	fclose(file);
	return particles;
}

void write_output(char *filename, body *particles, int num_particles)
{
	FILE *file = openFile(filename,"w");
	fprintf(file, "%d\n", num_particles);
	int i;
	for(i = 0; i < num_particles; i++)
		fprintf(file, "%lf %lf %lf %lf %lf\n", particles[i].xPosition, particles[i].yPosition, particles[i].mass, particles[i].xVelocity, particles[i].yVelocity);
	fclose(file);
}


// void write_bodies(FILE *file, body *particles, int num_particles)
// {
// 	int i;
// 
// 		
// 		return;
// 	}
// 	write_bodies(file, tree->bottomRight);
// 	write_bodies(file, tree->bottomLeft);
// 	write_bodies(file, tree->topRight);
// 	write_bodies(file, tree->topLeft);
// }

