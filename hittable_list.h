#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"
#include <vector>

class HittableList : public Hittable {
 public:
  HittableList() = default;
  HittableList(std::vector<Hittable*>& l);
  bool hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const override;
  bool bounding_box(
	double time0,
	double time1,
	aabb& output_box) const override;

  std::vector<Hittable*> list;
};

inline HittableList::HittableList(std::vector<Hittable*>& l) : list(l) {}

inline bool HittableList::hit(const Ray& r, double t_min, double t_max, HitRecord& hitrec) const {
  HitRecord temp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;

  for (int i = 0; i < list.size(); i++) {
	if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
	  hit_anything = true;
	  closest_so_far = temp_rec.t;
	  hitrec = temp_rec;
	}
  }

  return hit_anything;
}
inline aabb surrounding_box(aabb box0, aabb box1) {
  vec3 small(fmin(box0.min().x, box1.min().x),
	fmin(box0.min().y, box1.min().y),
	fmin(box0.min().z, box1.min().z));

  vec3 big(fmax(box0.max().x, box1.max().x),
	fmax(box0.max().y, box1.max().y),
	fmax(box0.max().z, box1.max().z));

  return aabb(small, big);
}

inline bool HittableList::bounding_box(double time0, double time1, aabb& output_box) const {
  if (list.empty()) return false;

  aabb temp_box;
  bool first_box = true;

  for (const auto& object : list) {
	if (!object->bounding_box(time0, time1, temp_box)) return false;
	output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
	first_box = false;
  }

  return true;
}

#endif
