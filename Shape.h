#ifndef SHAPE_H_
#define SHAPE_H_

#include "Matrix4.h"

static void printRay(Ray ray);
// Converts float to string
std::string convertFloatToString(float number){
    std::ostringstream buff;
    buff<<number;
    return buff.str();
}

// NOTE: A Primitive object has both a Material and a Shape
class Shape {
	public:
		// Abstract methods that Sphere and Triangle inherit from.
		virtual bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {
			return false;
		}
		virtual bool intersectP(Ray &ray) {
			return false;
		}
		virtual std::string shapeType() {
			return "";
		}
		virtual std::string printShapeInformation() {
			return "";
		}

		virtual ~Shape() {

		}

};

class Sphere : public Shape {
	public:
		float x, y, z, r;

	Sphere(float x, float y, float z, float r) {
		this->x = x;
		this->y = y;
		this->z = z;
		this->r = r;
	}

	// Test if ray intersects with the shape or not (in object space).
	// If so, return intersection point and normal.
	// This method is passed up to the geometric primitive class, which owns a method
	// of similar signature. Only difference is that it takes in an Intersection object
	// instead of a DifferentialGeometry object.
	// NOTE: Intersection object comprises of a DifferentialGeometry and a Primitive.
	// NOTE: This method must populate tHit and differentialGeometry.
	bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {

		// Sphere equation: (x - x0)^2 + (y - y0)^2 + (z - z0)^2 = r^2

		// Ray equations
		// x(t) = ft + g
		float f = ray.direction.x;
		float g = ray.position.x;

		// y(t) = ht + i
		float h = ray.direction.y;
		float i = ray.position.y;

		// z(t) = jt + k
		float j = ray.direction.z;
		float k = ray.position.z;

		// (ft + m))^2 + (ht + n))^2 + (jt + p))^2 - r^2 = 0
		float m = g - this->x;
		float n = i - this->y;
		float p = k - this->z;

		// at^2 + bt + c = 0
		float a = pow(f, 2) + pow(h, 2) + pow(j, 2);
		float b = (2 * f * m) + (2 * h * n) + (2 * j * p);
		float c = pow(m, 2) + pow(n, 2) + pow(p, 2) - pow(this->r, 2);

		float discriminant = pow(b, 2) - (4 * a * c);

		// No intersection, so return false
		if (discriminant < 0) {
			return false;
		}

		// Quadratic equation: (-b +/- sqrt(b^2 - 4ac)) / 2a
		float sqrtTerm = pow(b, 2) - (4 * a * c);

		float intersectionPoint1 = FLT_MAX;
		float intersectionPoint2 = FLT_MAX;
		float tIntersection = FLT_MAX;

		if (sqrtTerm == 0) {
			tIntersection = (b * -1.0) / (2.0 * a);
			// if our ONE intersection point is invalid, then return false
			if (!(tIntersection > ray.t_min && tIntersection < ray.t_max)) {
				return false;
			}
		// We have TWO intersection points
		} else {
			// Calculate both intersection points
			intersectionPoint1 = ((b * -1.0) + pow(sqrtTerm, 0.5)) / (2.0 * a);
			intersectionPoint2 = ((b * -1.0) - pow(sqrtTerm, 0.5)) / (2.0 * a);

			bool intersectionPoint1Valid = false;
			bool intersectionPoint2Valid = false;

			// If intersectionPoint1 is valid, according to our t_min and t_max...
			if (intersectionPoint1 > ray.t_min && intersectionPoint1 < ray.t_max) {
				intersectionPoint1Valid = true;
			}

			// If intersectionPoint1 is valid, according to our t_min and t_max...
			if (intersectionPoint2 > ray.t_min && intersectionPoint2 < ray.t_max) {
				intersectionPoint2Valid = true;
			}

			// Both points are valid
			if (intersectionPoint1Valid && intersectionPoint2Valid) {
				tIntersection = fmin(intersectionPoint1, intersectionPoint2);
			// Only intersection point 1 is valid
			} else if (intersectionPoint1Valid && !intersectionPoint2Valid) {
				tIntersection = intersectionPoint1;
			// Only intersection point 2 is valid
			} else if (!intersectionPoint1Valid && intersectionPoint2Valid) {
				tIntersection = intersectionPoint2;
			// Neither of our points are valid
			} else {
				return false;
			}
		}

		// NOTE: At this point, tIntersection is our closest intersection point

		// Now, find the (xCoor, yCoor, zCoor) that tIntersection corresponds to
		float xCoor = (f * tIntersection) + g;
		float yCoor = (h * tIntersection) + i;
		float zCoor = (j * tIntersection) + k;

		// Compute normal of the sphere and the intersection point in WORLD coordinates
		// This is NOT equal to the position of the intersection point

		differentialGeometry->normal = Normal::normalizeNormal(Normal(xCoor - this->x, yCoor - this->y, zCoor - this->z));

		differentialGeometry->position = Point(xCoor, yCoor, zCoor);

		// IMPORTANT: this used to be:
		// tHit = &tIntersection
		*tHit = tIntersection;
		return true;

	}

	// Test if ray intersects with the shape or not.
	// This method is passed up to the geometric primitive class, which owns a method
	// of the same signature.
	bool intersectP(Ray &ray) {
		// Ray equations
		// x(t) = ft + g
		float f = ray.direction.x;
		float g = ray.position.x;

		// y(t) = ht + i
		float h = ray.direction.y;
		float i = ray.position.y;

		// z(t) = jt + k
		float j = ray.direction.z;
		float k = ray.position.z;

		// (ft + m))^2 + (ht + n))^2 + (jt + p))^2 - r^2 = 0
		float m = g - this->x;
		float n = i - this->y;
		float p = k - this->z;

		// at^2 + bt + c = 0
		float a = pow(f, 2) + pow(h, 2) + pow(j, 2);
		float b = (2 * f * m) + (2 * h * n) + (2 * j * p);
		float c = pow(m, 2) + pow(n, 2) + pow(p, 2) - pow(this->r, 2);

		// discriminant = b^2 - 4ac
		float discriminant = pow(b, 2) - (4 * a * c);

		if (discriminant < 0) {
			return false;
		}

		// ***** Still need to check whether tIntersection is greater than tMin *****
		// Quadratic equation: (-b +/- sqrt(b^2 - 4ac)) / 2a
		float sqrtTerm = pow(b, 2) - (4 * a * c);

		float intersectionPoint1, intersectionPoint2;

		if (sqrtTerm == 0) {
			intersectionPoint1 = (b * -1.0) / (2.0 * a);
		} else {
			// sqrtTerm is positive
			intersectionPoint1 = ((b * -1.0) + pow(sqrtTerm, 0.5)) / (2.0 * a);
			intersectionPoint2 = ((b * -1.0) - pow(sqrtTerm, 0.5)) / (2.0 * a);
		}

		if (intersectionPoint1 >= ray.t_min && intersectionPoint1 < ray.t_max) {
			return true;
		}
		if (intersectionPoint2 >= ray.t_min && intersectionPoint2 < ray.t_max) {
			return true;
		}

		return false;
	}

	std::string shapeType() {
		return "Sphere";
	}

	std::string printShapeInformation() {
		std::string xCoor = convertFloatToString(x);
		std::string yCoor = convertFloatToString(y);
		std::string zCoor = convertFloatToString(z);
		std::string radius = convertFloatToString(r);

		std::string toReturn = "Center is (";
		toReturn.append(xCoor).append(", ").append(yCoor).append(", ").append(zCoor).append(") with radius ").append(radius).append("\n");
		return toReturn;
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

	// Test if ray intersects with the shape or not (in object space).
	// If so, return intersection point and normal.
	// This method is passed up to the geometric primitive class, which owns a method
	// of similar signature. Only difference is that it takes in an Intersection object
	// instead of a DifferentialGeometry object.
	// NOTE: Intersection object comprises of a DifferentialGeometry and a Primitive.
	// NOTE: This method must populate tHit and differentialGeometry.
	bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {
		return false;
	}

	//Test if ray intersects with the shape or not.
	//This method is passed up to the geometric primitive class, which owns a method
	//of the same signature.
	bool intersectP(Ray &ray) {
		return false;
	}

	std::string shapeType() {
		return "Triangle";
	}
};

#endif /* SHAPE_H_ */
