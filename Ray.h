/*
 * Ray.h
 *
 *  Created on: Mar 7, 2015
 *      Author: ryanyu
 */

#ifndef RAY_H_
#define RAY_H_

class Ray {
  public:
    Point position;
    Vector4 direction;
    float t_min, t_max;

  Ray(Point position, Vector4 direction, float t_min, float t_max) {
	  this->position = position;
	  this->direction = direction;
	  this->t_min = t_min;
	  this->t_max = t_max;
  }

  // Methods we probably need:
  /*
   *
   */

};

#endif /* RAY_H_ */
