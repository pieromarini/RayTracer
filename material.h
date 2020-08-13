#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"

struct HitPoint;

class Material {
  public:
	virtual bool scatter(const Ray& in, const HitPoint& hp, Color& attenuation, Ray& scattered) const = 0;
};

#endif
