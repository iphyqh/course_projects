#include <cmath>
#include <assert.h>

#include "Box.h"

// Relying on: 1. ComputeLocalBoundingBox(), gives local bounds for the box;
//			   2. BoundingBox::intersect(), which implements ray-box intersection for us and gives correspondng tNear and tFar intersection points;
bool Box::intersectLocal(const ray& r, isect& i) const
{
	// YOUR CODE HERE:
	// Add box intersection code here.
	// it currently ignores all boxes and just returns false.

	double tNear;
	double tFar;

	BoundingBox localbounds = ComputeLocalBoundingBox();

	bool intersect = localbounds.intersect(r, tNear, tFar);

	if (!intersect)
		return false;

	i.obj = this;


	i.t = tNear;
	i.N = vec3f(0.0, 0.0, 0.0);
	vec3f tP = r.at(tNear);
	for (int currentAxis = 0; currentAxis < 3;  currentAxis++) {
		for (int j = 1; j < 3; j++)
			if (abs(tP[currentAxis] - 0.5 * pow(-1, j)) < NORMAL_EPSILON)
				i.N[currentAxis] = pow(-1, j);
	}


	return intersect;
}
