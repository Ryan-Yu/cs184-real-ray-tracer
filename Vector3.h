#ifndef VECTOR3_H_
#define VECTOR3_H_

class Vector3 {
  public:
    float x, y, z;

  Vector3(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  float dotProduct(Vector3 other) {
    return this->x * other.x + this->y * other.y + this->z * other.z;
  }

  Vector3 addVector(Vector3 other) {
    return Vector3(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Vector3 subtractVector(Vector3 other) {
    return Vector3(this->x - other.x, this->y - other.y, this->z - other.z);
  }

  Vector3 scaleVector(float c) {
    return Vector3(c*this->x, c*this->y, c*this->z);
  }

  Vector3 multiplyVector(Vector3 other) {
    return Vector3(this->x * other.x, this->y * other.y, this->z * other.z);
  }
};

#endif /* VECTOR3_H_ */
