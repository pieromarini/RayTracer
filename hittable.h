#ifndef HITTABLE_H
#define HITTABLE_H

#include <optional>
#include "ray.h"

struct HitPoint {
  Point p;
  vec3 normal;
  double t;
  bool frontFace;

  inline void setFrontFace(const Ray& r, const vec3& outwardNormal) {
	frontFace = dot(r.direction(), outwardNormal) < 0;
	normal = frontFace ? outwardNormal : -outwardNormal;
  }
};

class Hittable {
  public:
	virtual std::optional<HitPoint> hit(const Ray& r, double tMin, double tMax) const = 0;
};

#endif
