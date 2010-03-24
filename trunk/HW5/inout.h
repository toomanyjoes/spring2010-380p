#ifndef INOUT_H_
#define INOUT_H_

#include "barneshut.h"
#include "quadTree.h"

quadTree *read_input(char *filename, int *num_particles);
void write_output(char *filename, quadTree *tree);

#endif
