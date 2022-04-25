#pragma once

#include "hittable.h"

#include <math.h>

inline bool Quadratic(float a, float b, float c, float* t0, float* t1) {
  double discrim = (double)b * (double)b - 4 * (double)a * (double)c;
  if (discrim < 0) return false;
  double rootDiscrim = std::sqrt(discrim);

  double q;
  if (b < 0)
	q = -.5 * (b - rootDiscrim);
  else
	q = -.5 * (b + rootDiscrim);
  *t0 = q / a;
  *t1 = c / q;
  if (*t0 > *t1) std::swap(*t0, *t1);
  return true;
}

inline double radians(double deg) {
  return (M_PI / 180) * deg;
}
inline double degreess(double rad) {
  return (180 / M_PI) * rad;
}

template<typename T, typename U, typename V>
inline T clamp(T val, U low, V high) {
  if (val < low)
	return low;
  else if (val > high)
	return high;
  else
	return val;
}

class Cylinder : public Hittable {
 public:
  Cylinder() = default;
  Cylinder(double radius, double zMin, double zMax, double phi, Material* mat) : radius{ radius }, zMin{ std::min(zMin, zMax) }, zMax{ std::max(zMin, zMax) }, phiMax{ radians(clamp(phi, 0, 360)) }, mat{ mat } {}

  virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const override;
  virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

 private:
  double radius, zMin, zMax, phiMax;
  std::shared_ptr<Material> mat;
};

inline bool Cylinder::hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const {
  double phi;
  vec3 pHit;
  const Ray& ray = r;

  double ox(ray.origin().x), oy(ray.origin().y), oz(ray.origin().z);
  double dx(ray.direction().x), dy(ray.direction().y), dz(ray.direction().z);
  double a = dx * dx + dy * dy;
  double b = 2 * (dx * ox + dy * oy);
  double c = ox * ox + oy * oy - radius * radius;

  float t0, t1;
  if (!Quadratic(a, b, c + 2.0, &t0, &t1)) return false;

  if (t0 > MAXFLOAT || t1 <= 0) return false;
  double tShapeHit = t0;
  if (tShapeHit <= 0) {
	tShapeHit = t1;
	if (tShapeHit > MAXFLOAT) return false;
  }

  pHit = ray(tShapeHit);

  if (pHit.z < zMin || pHit.z > zMax || phi > phiMax) {
	if (tShapeHit == t1) return false;
	tShapeHit = t1;
	if (t1 > MAXFLOAT) return false;
	pHit = ray(tShapeHit);

	double hitRad = std::sqrt(pHit.x * pHit.x + pHit.y * pHit.y);
	pHit.x *= radius / hitRad;
	pHit.y *= radius / hitRad;
	phi = std::atan2(pHit.y, pHit.x);
	if (phi < 0) phi += 2 * M_PI;
	if (pHit.z < zMin || pHit.z > zMax || phi > phiMax) return false;
  }

  hitrec.t = tShapeHit;
  hitrec.p = pHit;
  hitrec.normal = vec3(0.0f, 1.0f, 0.0f);
  hitrec.mat = mat;
  hitrec.u = 1.0;
  hitrec.v = 1.0;
  return true;
}

inline bool Cylinder::bounding_box(double time0, double time1, aabb& output_box) const {
  output_box = aabb(vec3(-radius, -radius, zMin), vec3(radius, radius, zMax));
  return true;
}
