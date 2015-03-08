#ifndef POINT_H_
#define POINT_H_

class Point {
  public:
    float x, y, z;

  Point() {

  }

  Point(float x, float y, float z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  Point addPoint(Point other) {
    return Point(this->x + other.x, this->y + other.y, this->z + other.z);
  }

  Point subtractPoint(Point other) {
    return Point(this->x - other.x, this->y - other.y, this->z - other.z);
  }
};

#endif /* POINT_H_ */
