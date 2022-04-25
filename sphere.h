#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"
#include "material.h"

constexpr double pi = 3.1415;

class Sphere : public Hittable {
 public:
  Sphere() = default;
  Sphere(vec3 c, float r, Material* mat);
  virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const override;

  virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

  vec3 center;
  float radius;
  std::shared_ptr<Material> mat;

 private:
  static void getSphereUV(const vec3& p, double& u, double& v) {
	// p: a given point on the sphere of radius one, centered at the origin.
	// u: returned value [0,1] of angle around the Y axis from X=-1.
	// v: returned value [0,1] of angle from Y=-1 to Y=+1.
	//     <1 0 0> yields <0.50 0.50>       <-1  0  0> yields <0.00 0.50>
	//     <0 1 0> yields <0.50 1.00>       < 0 -1  0> yields <0.50 0.00>
	//     <0 0 1> yields <0.25 0.50>       < 0  0 -1> yields <0.75 0.50>

	auto theta = acos(-p.y);
	auto phi = atan2(-p.z, p.x) + pi;

	u = phi / (2 * pi);
	v = theta / pi;
  }
};

inline Sphere::Sphere(vec3 c, float r, Material* mat)
  : center(c), radius(r), mat(mat) {}

inline bool Sphere::hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const {
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

	  double u, v;
	  getSphereUV(hitrec.normal, u, v);
	  hitrec.u = u;
	  hitrec.v = v;
	  return true;
	}
	tmp = (-b + sqrt(discriminant)) / (2.0f * a);
	if (tmp < t_max && tmp > t_min) {
	  hitrec.t = tmp;
	  hitrec.p = r(tmp);
	  hitrec.normal = (hitrec.p - center) / radius;
	  hitrec.mat = mat;

	  double u, v;
	  getSphereUV(hitrec.normal, u, v);
	  hitrec.u = u;
	  hitrec.v = v;
	  return true;
	}
  }
  return false;
}


inline bool Sphere::bounding_box(double time0, double time1, aabb& output_box) const {
  output_box = aabb(
	center - vec3(radius, radius, radius),
	center + vec3(radius, radius, radius));
  return true;
}

#endif
