#ifndef RAY_H
#define RAY_H

#include "vec3.h"

class Ray {
 public:
  Ray() = default;
  Ray(const vec3& a, const vec3& b);
  vec3 origin() const;
  vec3 direction() const;
  vec3 at(float t) const;
  vec3 operator()(float t) const;

  vec3 A;
  vec3 B;
};

Ray::Ray(const vec3& a, const vec3& b) : A(a), B(b) {}

vec3 Ray::origin() const { return A; }

vec3 Ray::direction() const { return B; }

vec3 Ray::at(float t) const { return (*this)(t); }

vec3 Ray::operator()(float t) const { return A + t * B; }

#endif
