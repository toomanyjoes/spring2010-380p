#ifndef QUADTREE_H_
#define QUADTREE_H_

enum quadrant {TOP_RIGHT, TOP_LEFT, BOTTOM_RIGHT, BOTTOM_LEFT};

typedef struct qTree
{
	double xPosition;
	double yPosition;

	double xVelocity;
	double yVelocity;

	double mass;

	// tracks the area that this node is responsible for
 	double xBottomLeft;
 	double yBottomLeft;
 	double xTopRight;
 	double yTopRight;

	struct qTree *topLeft;
	struct qTree *topRight;
	struct qTree *bottomLeft;
	struct qTree *bottomRight;
} quadTree;

typedef struct body
{
	double xPosition;
	double yPosition;
	double mass;
	double xVelocity;
	double yVelocity;
}body;

typedef struct body_list
{
	body b;
	struct body_list *next;
}body_list;


void addBody(quadTree *tree, double xPosition, double yPosition, double mass, double xVelocity, double yVelocity);
quadTree *allocNewNode(int q, quadTree *tree);
int hasChildren(quadTree *tree);
void freeQuadTree(quadTree *tree);

#endif