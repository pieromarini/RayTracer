#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
  public:
	Sphere() {}
	Sphere(Point center, double radius, std::shared_ptr<Material> m) : m_center{ center }, m_radius{ radius }, m_material{ m } {}

	/*
	 * Sphere + Ray equation solving
	 * (P - C) * (P - C) = r^2
	 * (A + tb - C) * (A + tb - C) = r^2
	 * Evaluate discriminant > 0 (2 real solutions) to know if the sphere intersects.
	 */
	virtual std::optional<HitPoint> hit(const Ray& r, double tMin, double tMax) const override {
	  auto oc = r.origin() - m_center;
	  auto a = r.direction().squaredMagnitude();
	  auto bHalf = dot(oc, r.direction());
	  auto c = oc.squaredMagnitude() - m_radius * m_radius;
	  auto discriminant = bHalf * bHalf - a * c;

	  if (discriminant > 0) {
		HitPoint hp;
		auto root = std::sqrt(discriminant);

		auto tmp = (-bHalf - root) / a;
		if (tmp < tMax && tmp > tMin) {
		  hp.t = tmp;
		  hp.p = r.at(hp.t);
		  auto outwardNormal = (hp.p - m_center) / m_radius;
		  hp.setFrontFace(r, outwardNormal);
		  hp.material = m_material;
		  return { hp };
		}

		tmp = (-bHalf + root) / a;
		if (tmp < tMax && tmp > tMin) {
		  hp.t = tmp;
		  hp.p = r.at(hp.t);
		  auto outwardNormal = (hp.p - m_center) / m_radius;
		  hp.setFrontFace(r, outwardNormal);
		  hp.material = m_material;
		  return { hp };
		}
	  }

	  return {};
	}

	Point m_center;
	double m_radius;
	std::shared_ptr<Material> m_material;
};

#endif
