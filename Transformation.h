#ifndef TRANSFORMATION_H_
#define TRANSFORMATION_H_

#include "Ray.h"

class Transformation {
	public:
		Eigen::Matrix4f m;
		Eigen::Matrix4f minvt;
		int numberOfMatrices;

	Transformation() {
		this->m = Matrix4::createIdentityMatrix();
		this->minvt = Matrix4::createIdentityMatrix();
		numberOfMatrices = 0;
	}

	Transformation(Eigen::Matrix4f transformationMatrix) {
		this->m = transformationMatrix;
		this->minvt = transformationMatrix.inverse().transpose();
		numberOfMatrices = 1;
	}

	void appendTransformation(Eigen::Matrix4f transformationToAppend) {
		this->m *= transformationToAppend;
		this->minvt = this->m.inverse().transpose();
		numberOfMatrices++;
	}

	void resetTransformation() {
		this->m = Matrix4::createIdentityMatrix();
		this->minvt = Matrix4::createIdentityMatrix();
		numberOfMatrices = 0;
	}

	Eigen::Matrix4f returnInverse() {
		return this->m.inverse();
	}

	// TODO: This may not be correct due to homogenizing points and vectors differently.
	Ray transformRay(Ray ray) {
		Ray rayToReturn = Ray();
		rayToReturn.position = Matrix4::multiplyMatrixByPoint(this->m, ray.position);
		rayToReturn.direction = Matrix4::multiplyMatrixByVector(this->m, ray.direction);
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
