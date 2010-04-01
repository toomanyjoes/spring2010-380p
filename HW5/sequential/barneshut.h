#ifndef BARNESHUT_H_
#define BARNESHUT_H_

#include "quadTree.h"

#define G 0.000000000066742
#define DAMPING 0.1
#define SOFTEN 18.0
#define THETA 0.5

typedef struct two_d_vector
{
	double xMagnitude;
	double yMagnitude;
} two_d_vector;

typedef struct {
	int id;
	body *particles;
	int num_particles;
} thread_arg;

#endif
