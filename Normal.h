#ifndef NORMAL_H_
#define NORMAL_H_

#include <math.h>

// Class that represents a normal vector
class Normal {
  public:
    float x, y, z;

  Normal() {

  }

  Normal(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  Normal addNormal(Normal other) {
    return Normal(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Normal subtractNormal(Normal other) {
    return Normal(this->x - other.x, this->y - other.y, this->z - other.z);
  }


  static Normal normalizeNormal(Normal other) {
    float normalizedMagnitude = sqrt(pow(this->x, 2.0) + pow(this->y, 2.0) + pow(this->z, 2.0));
    return Normal(this->x / normalizedMagnitude, this->y / normalizedMagnitude, this->z / normalizedMagnitude);
  }
};

#endif /* NORMAL_H_ */
