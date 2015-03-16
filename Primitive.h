#ifndef PRIMITIVE_H_
#define PRIMITIVE_H_

// Must #include -- forward declaration doesn't tell the compiler what the structure of Intersection is
#include "Intersection.h"

class Primitive {
	public:
		virtual bool intersect(Ray& ray, float* tHit, Intersection* in) {
			return false;
		}
		virtual bool intersectP(Ray& ray) {
			return false;
		}
		BRDFCoefficients getBRDF(DifferentialGeometry& differentialGeometry, BRDFCoefficients* brdf) {
			//TODO: Pointer logic may be wrong.
			return *brdf;
		}
};

class GeometricPrimitive : public Primitive {
	public:
		Shape* shape;
		Material* material;
		// TODO: Transformation objToWorld, worldToObj;

		// This populates method an empty Intersection object "in" and an empty time
		// "tHit" if the given ray intersects "this" geometric primitive.
		bool intersect(Ray& ray, float* tHit, Intersection* in) {
			// When we initially generate samples relative to our eye and to our view plane
			// we are generating the rays with respect to the "global coordinates", i.e. the eye
			// is at (0, 0, 0) relative to the global origin. However, when we want to calculate
			// the intersection of one of these global rays to a primitive, the problem is that
			// the primitive's coordinates are relative to its own origin. We must convert the
			// incoming ray to transform into coordinates that are relative to the primitive.
			// Then, when we are done calculating the intersection of the ray and the primitive,
			// we can transform the intersection's local geometry back into global coordinates.


			// TODO: Ray THIS-IS-OUR-REAL-RAY = worldToObj(*ray);

			DifferentialGeometry differentialGeometry; //creates new DifferentialGeometry to populate
			// This method call populates the variable "differentialGeometry".
			if (!shape->intersect(ray, tHit, &differentialGeometry)) {
				return false;
			}

			// If we find an intersection point, populate our "in" attributes with
			// intersection information.
			in->primitive = this;
			in->differentialGeometry = differentialGeometry;

			// TODO: Instead of the previous line, we want:
			// in->differentialGeometry = objToWorld(*differentialGeometry);
			return true;
		}

		// This method is identical to the method above, except it ONLY populates the intersection 'in'
		// if the tHit discovered is less than maxT
		bool intersectWithMaxT(Ray& ray, float* tHit, Intersection* in, float maxT) {

			// TODO: Ray THIS-IS-OUR-REAL-RAY = worldToObj(*ray);
			DifferentialGeometry differentialGeometry; //creates new DifferentialGeometry to populate
			// This method call populates the variable "differentialGeometry".
			if (!shape->intersect(ray, tHit, &differentialGeometry)) {
				return false;
			}

			// Only populate 'in' if the tHit we found is less than the maxT given
			if (*tHit < maxT) {
				in->primitive = this;
				in->differentialGeometry = differentialGeometry;
			}

			// TODO: Instead of the previous line, we want:
			// in->differentialGeometry = objToWorld(*differentialGeometry);
			return true;
		}

		bool intersectP(Ray& ray) {
			return shape->intersectP(ray);
		}

		// Simply returns the BRDFCoefficients object of 'this' Primitive
		BRDFCoefficients getBRDF(DifferentialGeometry& differentialGeometry, BRDFCoefficients* brdf) {
			return material->getBRDF(differentialGeometry, brdf);
		}

};

#endif /* PRIMITIVE_H_ */
