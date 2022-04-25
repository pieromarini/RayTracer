#ifndef MATERIAL_H
#define MATERIAL_H

#include "hittable.h"
#include "vec3.h"

vec3 randomInUnitSphere();

class Material {
 public:
  virtual vec3 emitted(double u, double v, const vec3& p) const {
	return vec3(0, 0, 0);
  }
  virtual bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const = 0;
};

// materials
class Lambertian;
class Metal;

class Lambertian : public Material {
 public:
  Lambertian(const vec3& a);
  virtual bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;

  vec3 albedo;
};

inline Lambertian::Lambertian(const vec3& a) : albedo(a) {}

inline bool Lambertian::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
  vec3 target = rec.p + rec.normal + randomInUnitSphere();
  scattered = Ray(rec.p, target - rec.p);
  attenuation = albedo;
  return true;
}

inline vec3 reflect(const vec3& v, const vec3& n) { return v - 2.0f * dot(v, n) * n; }

class Metal : public Material {
 public:
  Metal(const vec3& a, float f);
  virtual bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;

  vec3 albedo;
  float fuzz;
};

inline Metal::Metal(const vec3& a, float f) : albedo(a) {
  if (f < 1.0f)
	fuzz = f;
  else
	fuzz = 1.0f;
}

inline bool Metal::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
  vec3 reflected = reflect(normalize(rayIn.direction()), rec.normal);
  scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere());
  attenuation = albedo;
  return dot(scattered.direction(), rec.normal) > 0;
}

inline bool refract(const vec3& v, const vec3& n, float NIoverNT, vec3& refracted) {
  vec3 uv = normalize(v);
  float dt = dot(uv, n);
  float discriminant = 1.f - NIoverNT * NIoverNT * (1.f - dt * dt);
  if (discriminant > 0) {
	refracted = NIoverNT * (uv - dt * n) - n * std::sqrt(discriminant);
	return true;
  }
  return false;
}

inline float schlick(float cosine, float ref_idx) {
  float r0 = (1.f - ref_idx) / (1.f + ref_idx);
  r0 = r0 * r0;
  return r0 + (1.f - r0) * pow((1.f - cosine), 5.f);
}

class Dielectric : public Material {
 public:
  Dielectric(float ri);
  virtual bool scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const;

  float ref_idx;
};

inline Dielectric::Dielectric(float ri) : ref_idx{ ri } {}

inline bool Dielectric::scatter(const Ray& rayIn, const HitRecord& rec, vec3& attenuation, Ray& scattered) const {
  vec3 outward_normal;
  vec3 reflected = reflect(rayIn.direction(), rec.normal);
  float NIoverNT;
  attenuation = vec3(1.0, 1.0, 1.0);
  vec3 refracted;
  float reflect_prob;
  float cosine;
  if (dot(rayIn.direction(), rec.normal) > 0) {
	outward_normal = -rec.normal;
	NIoverNT = ref_idx;
	cosine = dot(rayIn.direction(), rec.normal) / rayIn.direction().magnitude();
	cosine = sqrt(1 - ref_idx * ref_idx * (1 - cosine * cosine));
  } else {
	outward_normal = rec.normal;
	NIoverNT = 1.0 / ref_idx;
	cosine = -dot(rayIn.direction(), rec.normal) / rayIn.direction().magnitude();
  }
  if (refract(rayIn.direction(), outward_normal, NIoverNT, refracted))
	reflect_prob = schlick(cosine, ref_idx);
  else
	reflect_prob = 1.0;
  if (drand48() < reflect_prob)
	scattered = Ray(rec.p, reflected);
  else
	scattered = Ray(rec.p, refracted);
  return true;
}

inline vec3 randomInUnitSphere() {
  vec3 p;
  do {
	p = 2.0f * vec3(drand48(), drand48(), drand48()) - vec3(1, 1, 1);
  } while (p.squareMagnitude() >= 1.0f);
  return p;
}

#endif
