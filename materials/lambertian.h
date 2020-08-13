#ifndef LAMBERTIAN_H
#define LAMBERTIAN_H

#include "../hittable.h"
#include "../material.h"

class Lambertian : public Material {
  public:
	Lambertian(const Color& a) : albedo{ a } {}

	bool scatter(const Ray& in, const HitPoint& hp, Color& attenuation, Ray& scattered) const override {
	  auto scatterDirection = hp.normal + randomUnitVector();
	  scattered = Ray{ hp.p, scatterDirection };
	  attenuation = albedo;
	  return true;
	}

	// Radiation a surface reflects
	Color albedo;
};

#endif
