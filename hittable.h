#ifndef HITTABLE_H
#define HITTABLE_H

#include "ray.h"
#include "vec3.h"

class Material;

struct HitRecord {
  float t;
  vec3 p;
  vec3 normal;
  Material* mat;
};

class Hittable {
 public:
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& hitrec) const = 0;
};

#endif
