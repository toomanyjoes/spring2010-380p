#ifndef INOUT_H_
#define INOUT_H_

#include "barneshut.h"
#include "quadTree.h"

body *read_input(char *filename, int *num_particles);
void write_output(char *filename, body *particles, int num_particles);

#endif