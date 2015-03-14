#ifndef AGGREGATEPRIMITIVE_H_
#define AGGREGATEPRIMITIVE_H_

class AggregatePrimitive {
	public:
		std::vector<GeometricPrimitive*> listOfPrimitives;

	AggregatePrimitive() {

	}

	AggregatePrimitive(std::vector<GeometricPrimitive*> listOfPrimitives) {
		this->listOfPrimitives = listOfPrimitives;
	}

	void addPrimitive(GeometricPrimitive* primitive) {
		listOfPrimitives.push_back(primitive);
	}

	// Just loops through all the primitives in the list and calls the intersect routine
	// Compare tHit of all the intersections and return that of the nearest one (we want the first hit)
	// NOTE: in->primitive should be set to the pointer to that primitive
	bool intersect(Ray& ray, float* tHit, Intersection* in) {
		bool hit = false;
		float smallestTSeenSoFar = FLT_MAX;
		float* pointerOfSmallestTSeenSoFar;
		Intersection* pointerOfClosestIntersectionSoFar;
		// For each primitive...
		for (std::vector<GeometricPrimitive*>::size_type i = 0; i < listOfPrimitives.size(); i++) {
			// Find whether the primitive hits the ray...
			if (listOfPrimitives[i]->intersect(ray, tHit, in)) {
				hit = true;
				// ... if it does, then update points for "lowest" t-value and corresponding intersection object
				if (*tHit < smallestTSeenSoFar) {
					smallestTSeenSoFar = *tHit;
					pointerOfSmallestTSeenSoFar = tHit;
					pointerOfClosestIntersectionSoFar = in;
				}
			}

		}

		// After we've checked all of our primitives, update our 'tHit' and 'in' to the nearest combination
		tHit = pointerOfSmallestTSeenSoFar;
		in = pointerOfClosestIntersectionSoFar;

		// Return whether our ray hit any of our primitives
		return hit;
	}

	// Just loops through all the primitives in the list and calls the intersectP routine
	// Returns true if our input ray hits ANY primitive, and false otherwise
	bool intersectP(Ray& ray) {
		for (std::vector<GeometricPrimitive*>::size_type i = 0; i < listOfPrimitives.size(); i++) {
			if (listOfPrimitives[i]->intersectP(ray)) {
				return true;
			}
		}
		return false;
	}

	// Just loops through all the primitives in the list and calls the intersectP routine
	// Returns true if our input ray hits ANY primitive, and false otherwise
	// NOTE: Does not consider primitives in our aggregatePrimitive if they match 'sourcePrimitive'
	bool intersectP(Ray& ray, GeometricPrimitive *sourcePrimitive) {
		for (std::vector<GeometricPrimitive*>::size_type i = 0; i < listOfPrimitives.size(); i++) {
			if (listOfPrimitives[i] == sourcePrimitive) {
				continue;
			}
			if (listOfPrimitives[i]->intersectP(ray)) {
				return true;
			}
		}
		return false;
	}

	// NOTE: This will never get called
	void getBRDF(DifferentialGeometry& differentialGeometry, BRDFCoefficients* brdf) {
		exit(1);
	}
};

#endif /* AGGREGATEPRIMITIVE_H_ */
