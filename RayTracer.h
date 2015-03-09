/*
 * RayTracer.h
 *
 *  Created on: Mar 8, 2015
 *      Author: ryanyu
 */

#ifndef RAYTRACER_H_
#define RAYTRACER_H_

class RayTracer {
	public:

		void trace(Ray& ray, int depth, Color* color) {
			if (depth > recursionDepth) {
				color->r = 0;
				color->g = 0;
				color->b =0;
				return;
			}
			float tHit;
			Intersection intersection;
			// This method will populate tHit and intersection if there is an intersection with this ray and any primitive.
			if (!aggregatePrimitive.intersect(ray, &tHit, &intersection) ) {
				color->r = 0;
				color->g = 0;
				color->b =0;
				return;
			}
			BRDFCoefficients brdf;
			// This method will populate the brdf variable with the brdf values of the intersection primitive.
			intersection.primitive->getBRDF(intersection.differentialGeometry, &brdf);

			//There is an intersection, so we have to loop through all the light sources

		}
};




#endif /* RAYTRACER_H_ */
