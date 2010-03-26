/*
	Assignment 5: MPI Barnes-Hut
	Samuel Palmer, Joe Elizondo
*/
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <sys/time.h>
#include "mpi.h"
#include "inout.h"
#include "barneshut.h"
#include "quadTree.h"

MPI_Datatype bodyType;

void updateVelocities(quadTree *root, body *particles, int num_particles, double timestep);
void updatePositions(quadTree *root, body *particles, int num_particles, double timestep);
void compute_accln(body *particle, quadTree *tree, two_d_vector *accel);
void acclnFormula(body *particle1, quadTree *particle2, two_d_vector *accel);
void shareData(body *particles, int num_particles, body *myParticles, int myParticleCt, int myRank, int numtasks);
void createBodyType(MPI_Datatype *outtype);

int main(int argc, char **argv)
{
	if(argc != 5)
	{
		printf("Usage:\n   %s [iterations (double)] [timestep (double)] [input file] [output file]\n",argv[0]);
		exit(1);
	}
	int numtasks,rank;
 	MPI_Init(NULL,NULL);
 	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
 	MPI_Comm_size(MPI_COMM_WORLD, &numtasks);
	createBodyType(&bodyType);

	struct timeval begin, end;

	int num_particles;
	double iterations = atof(argv[1]);
	double timestep = atof(argv[2]);
	body *particles = read_input(argv[3], &num_particles);
	quadTree *oldTree;
	char *outputfile = argv[4];

 	MPI_Barrier(MPI_COMM_WORLD); // wait for input to finish
	gettimeofday(&begin, NULL);
// printf("num_particles: %d\n",num_particles);
// 	body **orderedParticles = mortonOrder(particles, num_particles);
	int particles_per_proc = num_particles / numtasks;
	quadTree *tree;
	int i;
	tree = buildTree(particles, num_particles, rank, numtasks);
// 	write_output("stdout",tree);

	for(i=0; i < iterations; i++)
	{
		int sizeMyParticles;
		if(rank != numtasks-1)
			sizeMyParticles = particles_per_proc;
		else
			sizeMyParticles = particles_per_proc + (num_particles%numtasks);

// 		printf("proc %d  startIndex: %d  num_particles: %d\n", rank, (rank*particles_per_proc), sizeMyParticles);
		updateVelocities(tree, particles + (rank*particles_per_proc), sizeMyParticles, timestep);
// 		printf("iteration %d complete vel\n",i);
		updatePositions(tree, particles + (rank*particles_per_proc), sizeMyParticles, timestep);
// 		printf("iteration %d complete pos\n",i);
		shareData(particles, num_particles, particles + (rank*particles_per_proc), sizeMyParticles, rank, numtasks);
// 		printf("iteration %d complete share\n",i);
// 		free(orderedParticles);
// 		orderedParticles = mortonOrder(particles, num_particles);
		oldTree = tree;

// 		if(i == 14)
// 		{
// 			write_output("stdout",tree);
// 			#include <sys/types.h>
// #include <unistd.h>
// printf("pid: %d\n",getpid());
// 			int m = 0;
// 			int n;
// 			while(m == 0)
// 				n = 0;
// 		}
		
		tree = buildTree(particles, num_particles, rank, numtasks);
// 		printf("iteration %d complete build\n",i);
		freeQuadTree(oldTree);
// 		printf("completed iteration %d\n\n",i);
	}
	MPI_Type_free(&bodyType);
	MPI_Finalize();
	if(rank != 0)
		exit(0);
	gettimeofday(&end, NULL);
	double beginsec = (double)begin.tv_sec + ((double)begin.tv_usec/1000000.0);
	double endsec = (double)end.tv_sec + ((double)end.tv_usec/1000000.0);
	printf("Time: %lf\n",endsec-beginsec);
	
	write_output(outputfile, tree);
	freeQuadTree(tree);
	free(particles);
// 	free(orderedParticles);
	return 0;
}

void updateVelocities(quadTree *root, body *particles, int num_particles, double timestep)
{
// 	if(root == 0)
// 		return;
// 	if(hasChildren(tree))
// 	{
// 		updateVelocities(tree->topLeft, root, timestep);
// 		updateVelocities(tree->topRight, root, timestep);
// 		updateVelocities(tree->bottomLeft, root, timestep);
// 		updateVelocities(tree->bottomRight, root, timestep);
// 	}
// 	else
// 	{
	int i;
	two_d_vector accel;
	for(i = 0; i < num_particles; i++)
	{
		accel.xMagnitude = accel.yMagnitude = 0.0;
		compute_accln(&particles[i], root, &accel);
		//if((int)particles[i]->mass == 100000)
// 			printf("accel x: %lf y: %lf\n", accel.xMagnitude, accel.yMagnitude);

		particles[i].xVelocity += accel.xMagnitude * timestep * DAMPING;
		particles[i].yVelocity += accel.yMagnitude * timestep * DAMPING;
	}
// 	}
}

void updatePositions(quadTree *root, body *particles, int num_particles, double timestep)
{
// 	if(tree == 0) return;
// 	if(hasChildren(tree))
// 	{
// 		updatePositions(tree->topLeft, timestep);
// 		updatePositions(tree->topRight, timestep);
// 		updatePositions(tree->bottomLeft, timestep);
// 		updatePositions(tree->bottomRight, timestep);
// 	}
// 	else
// 	{
	int i;
	for(i = 0; i < num_particles; i++)
	{
		particles[i].xPosition += particles[i].xVelocity * timestep;
		particles[i].yPosition += particles[i].yVelocity * timestep;
	}
// 	}
}

void compute_accln(body *particle, quadTree *tree, two_d_vector *accel)
{
	if(tree == 0) return;
	if(!hasChildren(tree))
	{
		acclnFormula(particle, tree, accel);
// 		printf("xPos: %lf  yPos: %lf\n", particle->xPosition, particle->yPosition);
	}
	else
	{
		double r = sqrt(  (tree->xPosition - particle->xPosition) * (tree->xPosition - particle->xPosition)
				+ (tree->yPosition - particle->yPosition) * (tree->yPosition - particle->yPosition));
		double D = tree->size;		// size of bounding region
		if(D/r < THETA)
		{
// 			printf("theta  xPos: %lf  yPos: %lf\n", particle->xPosition, particle->yPosition);
			acclnFormula(particle, tree, accel);
		}
		else
		{
			compute_accln(particle, tree->topLeft, accel);
 			compute_accln(particle, tree->topRight, accel);
 			compute_accln(particle, tree->bottomLeft, accel);
 			compute_accln(particle, tree->bottomRight, accel);
		}
	}
}

void acclnFormula(body *particle1, quadTree *particle2, two_d_vector *accel)
{
	double x = particle2->xPosition;
	double y = particle2->yPosition;
        x -= particle1->xPosition;
        y -= particle1->yPosition;

        double dist = x*x + y*y;

        dist += SOFTEN; //softening factor helps in controlling the impact of one body over the other. assume a value of 18.

        dist = pow(dist, 3.0/2.0); //i.e dist to the power of 1.5
        double mag = particle2->mass / dist;

        accel->xMagnitude += mag*x;//G * mag*x;
        accel->yMagnitude += mag*y;//G * mag*y;
// 	printf("1xpos: %lf 2xpos: %lf 1ypos: %lf 2ypos: %lf xmag: %lf ymag: %lf x: %lf y: %lf dist: %lf mag: %lf part2Mass: %lf\n",particle1->xPosition, particle2->xPosition, particle1->yPosition, particle2->yPosition, accel->xMagnitude, accel->yMagnitude,x,y,dist,mag,particle2->mass);
        return;
}

void shareData(body *particles, int num_particles, body *myParticles, int myParticleCt, int myRank, int numtasks)
{
	int i, j;
	for(i = 0; i < numtasks-1; i++)
	{
		for(j = 0; j < num_particles/numtasks; j++)
		{
// printf("proc %d i: %d j: %d   xPos: %lf  yPos: %lf\n",myRank,i,j,particles[i*(num_particles/numtasks)+j]->xPosition, particles[i*(num_particles/numtasks)+j]->yPosition);
// 			printf("proc %d before recv particle %d from %d\n", myRank, j, i);
			MPI_Bcast(&particles[i*(num_particles/numtasks)+j],1,bodyType,i,MPI_COMM_WORLD);
// 			if(i == myRank)
// 				printf("proc: %d sent x: %lf y: %lf xv: %lf yv:%lf\n",myRank, particles[i*(num_particles/numtasks)+j]->xPosition, particles[i*(num_particles/numtasks)+j]->yPosition, particles[i*(num_particles/numtasks)+j]->xVelocity, particles[i*(num_particles/numtasks)+j]->yVelocity);
// 			else
// 				printf("proc: %d recv x: %lf y: %lf xv: %lf yv:%lf\n",myRank, particles[i*(num_particles/numtasks)+j]->xPosition, particles[i*(num_particles/numtasks)+j]->yPosition, particles[i*(num_particles/numtasks)+j]->xVelocity, particles[i*(num_particles/numtasks)+j]->yVelocity);
// 			printf("proc %d after recv particle %d from %d\n", myRank, j, i);
// printf("proc %d i: %d j: %d   xPos: %lf  yPos: %lf\n",myRank,i,j,particles[i*(num_particles/numtasks)+j]->xPosition, particles[i*(num_particles/numtasks)+j]->yPosition);
		}
	}
	for(j = 0; j < num_particles/numtasks + (num_particles%numtasks); j++)
	{
// 		printf("proc %d before recv particle %d from %d\n", myRank, j, numtasks-1);
		MPI_Bcast(&particles[(numtasks-1)*(num_particles/numtasks)+j],1,bodyType,numtasks-1,MPI_COMM_WORLD);
// 		printf("proc %d after recv particle %d from %d\n", myRank, j, numtasks-1);
// 		if(numtasks-1 == myRank)
// 			printf("proc: %d sent x: %lf y: %lf xv: %lf yv:%lf\n",myRank, particles[(numtasks-1)*(num_particles/numtasks)+j]->xPosition, particles[(numtasks-1)*(num_particles/numtasks)+j]->yPosition, particles[(numtasks-1)*(num_particles/numtasks)+j]->xVelocity, particles[(numtasks-1)*(num_particles/numtasks)+j]->yVelocity);
// 		else
// 			printf("proc: %d recv x: %lf y: %lf xv: %lf yv:%lf\n",myRank, particles[(numtasks-1)*(num_particles/numtasks)+j]->xPosition, particles[(numtasks-1)*(num_particles/numtasks)+j]->yPosition, particles[(numtasks-1)*(num_particles/numtasks)+j]->xVelocity, particles[(numtasks-1)*(num_particles/numtasks)+j]->yVelocity);
	}
}

void createBodyType(MPI_Datatype *outtype)
{
	body b;
	int count[] = { 1,1,1,1,1,1 };
	MPI_Datatype type[] = { MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE, MPI_DOUBLE };
	int bAddr, xAddr, yAddr, massAddr, xvAddr, yvAddr;//, mnAddr;
	int disp[6];
	MPI_Get_address(&b, &bAddr);
	MPI_Get_address(&b.xPosition, &xAddr);
	MPI_Get_address(&b.yPosition, &yAddr);
	MPI_Get_address(&b.mass, &massAddr);
	MPI_Get_address(&b.xVelocity, &xvAddr);
	MPI_Get_address(&b.yVelocity, &yvAddr);
// 	MPI_Get_address(&b.mortonNumber, &mnAddr);
	disp[0] = xAddr - bAddr;
	disp[1] = yAddr - bAddr;
	disp[2] = massAddr - bAddr;
	disp[3] = xvAddr - bAddr;
	disp[4] = yvAddr - bAddr;
// 	disp[5] = mnAddr - bAddr;
	MPI_Type_struct(5,count,disp,type,outtype);
	MPI_Type_commit(outtype);
}

