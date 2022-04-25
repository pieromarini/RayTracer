#ifndef RECT_H
#define RECT_H

#include "hittable.h"
#include "aabb.h"

class xy_rect : public Hittable {
 public:
  xy_rect() {}

  xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<Material> mat)
	: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat){};

  virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

  virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
	// The bounding box must have non-zero width in each dimension, so pad the Z
	// dimension a small amount.
	output_box = aabb(vec3(x0, y0, k - 0.0001), vec3(x1, y1, k + 0.0001));
	return true;
  }

 public:
  std::shared_ptr<Material> mp;
  double x0, x1, y0, y1, k;
};

inline bool xy_rect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
  auto t = (k - r.origin().z) / r.direction().z;
  if (t < t_min || t > t_max)
	return false;
  auto x = r.origin().x + t * r.direction().x;
  auto y = r.origin().y + t * r.direction().y;
  if (x < x0 || x > x1 || y < y0 || y > y1)
	return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (y - y0) / (y1 - y0);
  rec.t = t;
  auto outward_normal = vec3(0, 0, 1);
  rec.set_face_normal(r, outward_normal);
  rec.mat = mp;
  rec.p = r.at(t);
  return true;
}

class xz_rect : public Hittable {
 public:
  xz_rect() {}

  xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, Material* mat)
	: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mp(mat){};

  virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

  virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
	// The bounding box must have non-zero width in each dimension, so pad the Y
	// dimension a small amount.
	output_box = aabb(vec3(x0, k - 0.0001, z0), vec3(x1, k + 0.0001, z1));
	return true;
  }

 public:
  std::shared_ptr<Material> mp;
  double x0, x1, z0, z1, k;
};

class yz_rect : public Hittable {
 public:
  yz_rect() {}

  yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<Material> mat)
	: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mp(mat){};

  virtual bool hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const override;

  virtual bool bounding_box(double time0, double time1, aabb& output_box) const override {
	// The bounding box must have non-zero width in each dimension, so pad the X
	// dimension a small amount.
	output_box = aabb(vec3(k - 0.0001, y0, z0), vec3(k + 0.0001, y1, z1));
	return true;
  }

 public:
  std::shared_ptr<Material> mp;
  double y0, y1, z0, z1, k;
};

inline bool xz_rect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
  auto t = (k - r.origin().y) / r.direction().y;
  if (t < t_min || t > t_max)
	return false;
  auto x = r.origin().x + t * r.direction().x;
  auto z = r.origin().z + t * r.direction().z;
  if (x < x0 || x > x1 || z < z0 || z > z1)
	return false;
  rec.u = (x - x0) / (x1 - x0);
  rec.v = (z - z0) / (z1 - z0);
  rec.t = t;
  auto outward_normal = vec3(0, 1, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat = mp;
  rec.p = r.at(t);
  return true;
}

inline bool yz_rect::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
  auto t = (k - r.origin().x) / r.direction().x;
  if (t < t_min || t > t_max)
	return false;
  auto y = r.origin().y + t * r.direction().y;
  auto z = r.origin().z + t * r.direction().z;
  if (y < y0 || y > y1 || z < z0 || z > z1)
	return false;
  rec.u = (y - y0) / (y1 - y0);
  rec.v = (z - z0) / (z1 - z0);
  rec.t = t;
  auto outward_normal = vec3(1, 0, 0);
  rec.set_face_normal(r, outward_normal);
  rec.mat = mp;
  rec.p = r.at(t);
  return true;
}

#endif
