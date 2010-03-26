#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <stdint.h>

#define MIN_GRID -10000.0
#define MAX_GRID 10000.0

enum quadrant {TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};


typedef struct body
{
	double xPosition;
	double yPosition;
	double mass;
	double xVelocity;
	double yVelocity;
	uint64_t mortonNumber;
}body;

typedef struct qTree
{
	double xPosition;
	double yPosition;

	double xVelocity;
	double yVelocity;

	double mass;

	body *particle;

	// tracks the area that this node is responsible for
 	double xBottomLeft;
 	double yBottomLeft;
 	double size;

	// children
	struct qTree *topLeft;
	struct qTree *topRight;
	struct qTree *bottomLeft;
	struct qTree *bottomRight;
} quadTree;

typedef struct body_list
{
	body b;
	struct body_list *next;
}body_list;

void addBody(quadTree *tree, body *particle);
quadTree *buildTree(body *particles, int num_particles, int rank, int numtasks);
int hasChildren(quadTree *tree);
body **mortonOrder(body *particles, int num_particles);
void freeQuadTree(quadTree *tree);

#endif
