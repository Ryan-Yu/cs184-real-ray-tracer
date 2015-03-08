#ifndef DIFFERENTIALGEOMETRY_H_
#define DIFFERENTIALGEOMETRY_H_

#include "point.h"
#include "normal.h"

// Class that represents a point on a Primitive object, such as a sphere.
// We will primarily use this class for intersection points of primitives and rays
class DifferentialGeometry {
	public:
		Point position;
		Normal normal;

	DifferentialGeometry() {

	}

	DifferentialGeometry(Point position, Normal normal) {
		this->position = position;
		this->normal = normal;
	}
};

#endif /* DIFFERENTIALGEOMETRY_H_ */
