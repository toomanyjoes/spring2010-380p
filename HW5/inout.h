#ifndef INOUT_H_
#define INOUT_H_

#include "barneshut.h"

enum quadrant {TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};

void read_input(char *filename, quadTree *out_tree);
void write_output(char *filename, quadTree *tree);

#endif
