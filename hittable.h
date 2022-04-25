#ifndef HITTABLE_H
#define HITTABLE_H

#include <memory>
#include "ray.h"
#include "aabb.h"
#include "vec3.h"

class Material;

struct HitRecord {
  double t;
  double u, v;
  vec3 p;
  vec3 normal;
  std::shared_ptr<Material> mat;
  bool frontFace;
  inline void set_face_normal(const Ray& r, const vec3& outward_normal) {
	frontFace = dot(r.direction(), outward_normal) < 0;
	normal = frontFace ? outward_normal : -outward_normal;
  }
};

class Hittable {
 public:
  Hittable() = default;
  Hittable(Hittable&&) = default;
  virtual ~Hittable() = default;

  Hittable(const Hittable&) = delete;
  Hittable& operator=(const Hittable&) = delete;
  Hittable& operator=(Hittable&&) = delete;

  virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const = 0;

  virtual bool bounding_box(double time0, double time1, aabb& output_box) const = 0;
};

#endif
