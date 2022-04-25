#pragma once

#include "texture.h"
#include "material.h"

class DiffuseLight : public Material {
 public:
  DiffuseLight(std::shared_ptr<Texture> a) : emit(a) {}
  DiffuseLight(vec3 c) : emit(std::make_shared<SolidColor>(c)) {}

  virtual bool scatter(
	const Ray& r_in,
	const HitRecord& rec,
	vec3& attenuation,
	Ray& scattered) const override {
	return false;
  }

  virtual vec3 emitted(double u, double v, const vec3& p) const override {
	return emit->value(u, v, p);
  }

 public:
  std::shared_ptr<Texture> emit;
};
