/*
 * Intersection.h
 *
 *  Created on: Mar 7, 2015
 *      Author: ryanyu
 */

#ifndef INTERSECTION_H_
#define INTERSECTION_H_

class Primitive;

//This class represents an intersection point of a ray and a primitive.
class Intersection {
	public:
		DifferentialGeometry differentialGeometry;
		Primitive *primitive;
		Intersection() {

		}

		Intersection(DifferentialGeometry differntialGeometry, Primitive *primitive) {
			this->differentialGeometry = differentialGeometry;
			this->primitive = primitive;
		}
};



#endif /* INTERSECTION_H_ */
