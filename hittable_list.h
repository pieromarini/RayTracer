#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

class HittableList : public Hittable {
 public:
  HittableList() = default;
  HittableList(Hittable** l, int n);
  virtual bool hit(const Ray& r, float t_min, float t_max, HitRecord& hitrec) const;

  Hittable** list;
  int size;
};

HittableList::HittableList(Hittable** l, int n) : list(l), size(n) {}

bool HittableList::hit(const Ray& r, float t_min, float t_max, HitRecord& hitrec) const {
  HitRecord temp_rec;
  bool hit_anything = false;
  double closest_so_far = t_max;

  for (int i = 0; i < size; i++) {
    if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
      hit_anything = true;
      closest_so_far = temp_rec.t;
      hitrec = temp_rec;
    }
  }

  return hit_anything;
}

#endif
