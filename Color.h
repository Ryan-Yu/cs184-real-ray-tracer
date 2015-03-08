#ifndef COLOR_H_
#define COLOR_H_

#include <math.h>

class Color {
  public:
    float r, g, b;

  Color() {

  }

  Color(float r, float g, float b) {
    this->r = r;
    this->g = g;
    this->b = b;
  }

  Color addColor(Color other) {
    return Color(this->r + other.r, this->g + other.g, this->b + other.b);
  }

  Color subtractColor(Color other) {
    return Color(this->r - other.r, this->g - other.g, this->b - other.b);
  }

  Color scaleColor(float c) {
    return Color(c*this->r, c*this->g, c*this->b);
  }

  Color multiplyColor(Color other) {
    return Color(this->r * other.r, this->g * other.g, this->b * other.b);
  }
};

#endif /* COLOR_H_ */
