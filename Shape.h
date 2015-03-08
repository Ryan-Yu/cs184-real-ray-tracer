/*
 * Shape.h
 *
 *  Created on: Mar 7, 2015
 *      Author: ryanyu
 */

#ifndef SHAPE_H_
#define SHAPE_H_


class Shape {
public:
	//Abstract methods that Sphere and Triangle inherit from.
	bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {
		return false;
	}
	bool intersectP(Ray &ray) {
		return false;
	}
};

//TODO: Fill out the intersect and intersectP methods.
class Sphere : public Shape {
	public:
		float x, y, z, r;
	Sphere(float x, float y, float z, float r) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
	}
	//Test if ray intersects with the shape or not (in object space), if so, return
	//intersection point and normal.
	//This method is passed up to the geometric primitive class, which owns a method
	//of similar signature. Only difference is that it takes in an Intersection object
	//instead of a DifferentialGeoemetry object. Intersection object comprises of a
	//DifferentialGeometry and a Primitive.
	//Must populate tHit and differentialGeometry.
	bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {
		return false;
	}

	//Test if ray intersects with the shape or not.
	//This method is passed up to the geometric primitive class, which owns a method
	//of the same signature.
	bool intersectP(Ray &ray) {
		return false;
	}
};

class Triangle : public Shape {
	public:
		Point v1, v2, v3;
	Triangle(Point v1, Point v2, Point v3) {
		this->v1 = v1;
		this->v2 = v2;
		this->v3 = v3;
	}
	//Test if ray intersects with the shape or not (in object space), if so, return
	//intersection point and normal.
	//This method is passed up to the geometric primitive class, which owns a method
	//of similar signature. Only difference is that it takes in an Intersection object
	//instead of a DifferentialGeoemetry object. Intersection object comprises of a
	//DifferentialGeometry and a Primitive.
	//Must populate tHit and differentialGeometry.
	bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {
		return false;
	}

	//Test if ray intersects with the shape or not.
	//This method is passed up to the geometric primitive class, which owns a method
	//of the same signature.
	bool intersectP(Ray &ray) {
		return false;
	}
};

#endif /* SHAPE_H_ */
