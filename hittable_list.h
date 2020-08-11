#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include <memory>
#include <optional>
#include <type_traits>
#include <vector>
#include "hittable.h"

class HittableList : public Hittable {
  public:
	HittableList() {}
	HittableList(std::shared_ptr<Hittable> obj) {
	  add (obj);
	}

	void clear() { m_objects.clear(); }
	void add(std::shared_ptr<Hittable> obj) {
	  m_objects.push_back(obj);
	}

	virtual std::optional<HitPoint> hit(const Ray& r, double tMin, double tMax) const override {
	  HitPoint hp;
	  auto hitAnything = false;
	  auto closest = tMax;

	  for (const auto& object : m_objects) {
		if (auto hit = object->hit(r, tMin, closest); hit) {
		  closest = hit->t;
		  hp = hit.value();
		  hitAnything = true;
		}
	  }

	  return hitAnything ? std::optional<HitPoint>{ hp } : std::nullopt;
	}

  private:
	std::vector<std::shared_ptr<Hittable>> m_objects;
};

#endif
