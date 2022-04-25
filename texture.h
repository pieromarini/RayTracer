#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.h"

class Texture {
 public:
  virtual vec3 value(double u, double v, const vec3& p) const = 0;
};

class SolidColor : public Texture {
 public:
  SolidColor() {}
  SolidColor(vec3 c) : colorValue(c) {}

  SolidColor(double red, double green, double blue)
	: SolidColor(vec3(red, green, blue)) {}

  virtual vec3 value(double u, double v, const vec3& p) const override {
	return colorValue;
  }

 private:
  vec3 colorValue;
};

#endif
