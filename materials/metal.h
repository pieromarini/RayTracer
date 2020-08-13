#ifndef METAL_H
#define METAL_H

#include "../hittable.h"
#include "../material.h"

class Metal : public Material {
  public:
	Metal(const Color& a, double f) : albedo{ a }, fuzz{ f } {}

	bool scatter(const Ray& in, const HitPoint& hp, Color& attenuation, Ray& scattered) const override {
	  auto reflected = reflect(normalize(in.direction()), hp.normal);
	  scattered = Ray{ hp.p, reflected + fuzz * randomInUnitSphere() };
	  attenuation = albedo;
	  return (dot(scattered.direction(), hp.normal) > 0);
	}

	Color albedo;
	double fuzz;
};

#endif
