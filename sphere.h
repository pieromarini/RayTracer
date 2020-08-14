#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"

class Sphere : public Hittable {
 public:
  Sphere() = default;
  Sphere(vec3 c, float r, Material* mat);
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& hitrec) const;

  vec3 center;
  float radius;
  Material* mat;
};

Sphere::Sphere(vec3 c, float r, Material* mat)
    : center(c), radius(r), mat(mat) {}

bool Sphere::hit(const Ray& r, float t_min, float t_max, HitRecord& hitrec) const {
  const vec3 dir = r.direction();
  const vec3 oc = r.origin() - center;
  const float a = dot(dir, dir);
  const float b = 2.0f * dot(dir, oc);
  const float c = dot(oc, oc) - radius * radius;
  const float discriminant = b * b - 4.0f * a * c;

  if (discriminant > 0.0f) {
    float tmp = (-b - sqrt(discriminant)) / (2.0f * a);
    if (tmp < t_max && tmp > t_min) {
      hitrec.t = tmp;
      hitrec.p = r(tmp);
      hitrec.normal = (hitrec.p - center) / radius;
      hitrec.mat = mat;
      return true;
    }
    tmp = (-b + sqrt(discriminant)) / (2.0f * a);
    if (tmp < t_max && tmp > t_min) {
      hitrec.t = tmp;
      hitrec.p = r(tmp);
      hitrec.normal = (hitrec.p - center) / radius;
      hitrec.mat = mat;
      return true;
    }
  }
  return false;
}

#endif
