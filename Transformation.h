#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include "Ray.h"

class Transformation {
	public:
		Matrix4 m;
		Matrix4 minvt;

	Transformation() {

	}

	Transformation(Matrix4 transformationMatrix) {
		this->m = transformationMatrix;
	}



	Ray transformRay(Ray ray) {
		Ray rayToReturn = Ray();
		rayToReturn.position = Matrix4::multiplyMatrixByPoint(this->m, ray.position);
		rayToReturn.direction = ray.direction;
		rayToReturn.t_min = ray.t_min;
		//TODO: Determine what t_max equals for different lights.
		rayToReturn.t_max = ray.t_max;
		return rayToReturn;
	}

	Point transformPoint(Point point) {
		return Matrix4::multiplyMatrixByPoint(this->m, point);
	}
};

#endif /* TRANSFORMATION_H_ */
