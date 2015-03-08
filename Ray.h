#ifndef RAY_H_
#define RAY_H_

#include "Point.h"
#include "Vector4.h"

class Ray {
  public:
    Point position;
    Vector4 direction;
    float t_min, t_max;

  Ray() {

  }

  Ray(Point position, Vector4 direction, float t_min, float t_max) {
	  this->position = position;
	  this->direction = direction;
	  this->t_min = t_min;
	  this->t_max = t_max;
  }

  // TODO: Methods we probably need:
  /*
   * (1) findIntersectionWithPrimitive(Primitive p)
   *     // returns t value of intersection
   */

};

#endif /* RAY_H_ */
