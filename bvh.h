#pragma once

#include <algorithm>
#include "hittable.h"
#include "hittable_list.h"


class BVHNode : public Hittable {
 public:
  BVHNode();

  BVHNode(const HittableList& list, double time0, double time1)
	: BVHNode(list.list, 0, list.list.size(), time0, time1) {}

  BVHNode(
	const std::vector<Hittable*>& src_objects,
	size_t start,
	size_t end,
	double time0,
	double time1);

  virtual bool hit(
	const Ray& r,
	double t_min,
	double t_max,
	HitRecord& rec) const override;

  virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;

 public:
  Hittable* left;
  Hittable* right;
  aabb box;
};

inline bool BVHNode::bounding_box(double time0, double time1, aabb& output_box) const {
  output_box = box;
  return true;
}

inline bool BVHNode::hit(const Ray& r, double t_min, double t_max, HitRecord& rec) const {
  if (!box.hit(r, t_min, t_max))
	return false;

  bool hit_left = left->hit(r, t_min, t_max, rec);
  bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

  return hit_left || hit_right;
}

inline double random_double() {
  // Returns a random real in [0,1).
  return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
  // Returns a random real in [min,max).
  return min + (max - min) * random_double();
}

inline int random_int(int min, int max) {
  // Returns a random integer in [min,max].
  return static_cast<int>(random_double(min, max + 1));
}

inline bool box_x_compare(const Hittable* a, const Hittable* b) {
  aabb box_a;
  aabb box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
	std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().x < box_b.min().x;
}

inline bool box_y_compare(const Hittable* a, const Hittable* b) {
  aabb box_a;
  aabb box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
	std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().y < box_b.min().y;
}

inline bool box_z_compare(const Hittable* a, const Hittable* b) {
  aabb box_a;
  aabb box_b;

  if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
	std::cerr << "No bounding box in bvh_node constructor.\n";

  return box_a.min().z < box_b.min().z;
}

inline BVHNode::BVHNode(
  const std::vector<Hittable*>& src_objects,
  size_t start,
  size_t end,
  double time0,
  double time1) {
  auto objects = src_objects;// Create a modifiable array of the source scene objects

  int axis = random_int(0, 2);
  auto comparator = (axis == 0) ? box_x_compare : (axis == 1) ? box_y_compare :
																  box_z_compare;

  size_t object_span = end - start;

  if (object_span == 1) {
	left = right = objects[start];
  } else if (object_span == 2) {
	if (comparator(objects[start], objects[start + 1])) {
	  left = objects[start];
	  right = objects[start + 1];
	} else {
	  left = objects[start + 1];
	  right = objects[start];
	}
  } else {
	std::sort(objects.begin() + start, objects.begin() + end, comparator);

	auto mid = start + object_span / 2;
	left = new BVHNode(objects, start, mid, time0, time1);
	right = new BVHNode(objects, mid, end, time0, time1);
  }

  aabb box_left, box_right;

  if (!left->bounding_box(time0, time1, box_left)
	  || !right->bounding_box(time0, time1, box_right))
	std::cerr << "No bounding box in bvh_node constructor.\n";

  box = surrounding_box(box_left, box_right);
}
