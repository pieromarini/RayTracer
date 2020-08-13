#ifndef DIELECTRIC_H
#define DIELECTRIC_H

#include "../hittable.h"
#include "../material.h"
#include "../utils.h"

class Dielectric : public Material {
  public:
	Dielectric(double ri) : refractionIndex{ ri } {}

	bool scatter(const Ray& in, const HitPoint& hp, Color& attenuation, Ray& scattered) const override {
	  attenuation = Color{ 1.0, 1.0, 1.0 };
	  double etaiOverEtat = hp.frontFace ? (1.0 / refractionIndex) : refractionIndex;

	  vec3 unitDir = normalize(in.direction());

	  double cosTheta = fmin(dot(-unitDir, hp.normal), 1.0);
	  double sinTheta = sqrt(1.0 - cosTheta * cosTheta); 

	  if (etaiOverEtat * sinTheta > 1.0) {
		auto reflected = reflect(unitDir, hp.normal);
		scattered = Ray{ hp.p, reflected };
		return true;
	  }

	  double reflectionProb = schlick(cosTheta, etaiOverEtat);
	  if (randomDouble() < reflectionProb) {
		auto reflected = reflect(unitDir, hp.normal);
		scattered = Ray{ hp.p, reflected };
		return true;
	  }

	  auto refracted = refract(unitDir, hp.normal, etaiOverEtat);
	  scattered = Ray{ hp.p, refracted };
	  return true;
	}

	double refractionIndex;
};

#endif

