#ifndef BARNESHUT_H_
#define BARNESHUT_H_

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

#endif
