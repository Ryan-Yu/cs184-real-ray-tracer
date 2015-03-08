#ifndef SHAPE_H_
#define SHAPE_H_

// NOTE: A Primitive object has both a Material and a Shape
class Shape {
	public:
		// Abstract methods that Sphere and Triangle inherit from.
		bool intersect(Ray &ray, float* tHit, DifferentialGeometry* differentialGeometry) {
			return false;
		}
		bool intersectP(Ray &ray) {
			return false;
		}
};

// TODO: Fill out the intersect and intersectP methods.
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

		float tIntersection;

		if (sqrtTerm == 0) {
			tIntersection = (b * -1.0) / (2.0 * a);
		} else {
			// sqrtTerm is positive
			tIntersection = ((b * -1.0) + sqrtTerm) / (2.0 * a);
			if ((((b * -1.0) - sqrtTerm) / (2.0 * a)) < tIntersection) {
				tIntersection = ((b * -1.0) + sqrtTerm) / (2.0 * a);
			}
		}

		// at this point, tIntersection is our closest intersection point

		// now, find the (xCoor, yCoor, zCoor) that tIntersection corresponds to
		float xCoor = (f * tIntersection) + g;
		float yCoor = (h * tIntersection) + i;
		float zCoor = (j * tIntersection) + k;

		// (?) Because we are on a sphere, the Normal is precisely the normalizecd version of the intersection point
		// (in primitive coordinates)
		differentialGeometry->position = Point(xCoor, yCoor, zCoor);
		differentialGeometry->normal = Normal::normalizeNormal(Normal(xCoor, yCoor, zCoor));
		tHit = *tIntersection;

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
		return true;
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
};

#endif /* SHAPE_H_ */
