/*
 * DifferentialGeometry.h
 *
 *  Created on: Mar 7, 2015
 *      Author: ryanyu
 */

#ifndef DIFFERENTIALGEOMETRY_H_
#define DIFFERENTIALGEOMETRY_H_

#include "point.h"
#include "normal.h"

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
