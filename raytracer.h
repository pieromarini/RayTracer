#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cfloat>
#include <cstdlib>
#include <iostream>
#include "camera.h"
#include "cylinder.h"
#include "hittable.h"
#include "hittable_list.h"
#include "light.h"
#include "ray.h"
#include "sphere.h"
#include "rect.h"
#include "vec3.h"

inline vec3 rayColor(const Ray& r, const vec3& background, Hittable* world, int depth) {
  HitRecord rec;

  // If we've exceeded the ray bounce limit, no more light is gathered.
  if (depth <= 0)
	return vec3(0, 0, 0);

  // If the ray hits nothing, return the background color.
  if (!world->hit(r, 0.001, MAXFLOAT, rec))
	return background;

  Ray scattered;
  vec3 attenuation;
  vec3 emitted = rec.mat->emitted(rec.u, rec.v, rec.p);

  if (!rec.mat->scatter(r, rec, attenuation, scattered))
	return emitted;

  return emitted + attenuation * rayColor(scattered, background, world, depth - 1);
}

inline Hittable* randomScene() {
  const int n = 10;
  auto list = std::vector<Hittable*>();
  list.push_back(new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5f, 0.5f, 0.5f))));


  list.push_back(new Sphere(vec3(0.0f, 2.1f, 0.0f), -2.1f, new Dielectric(1.0f)));
  list.push_back(new Sphere(vec3(3.5f, 1.5f, 0.0f), 0.7f, new Lambertian(vec3{ 0.2f, 0.0f, 0.0f })));
  list.push_back(new Sphere(vec3(4.5f, 0.6f, 1.0f), 0.5f, new Metal(vec3(0.7f, 0.6f, 0.5f), 0.f)));

  const vec3 fireflyColor = { 191.0 / 255.0, 139.0 / 255.0, 50.0 / 255.0 };
  list.push_back(new Sphere(vec3(0.5f, 1.5f, 0.0f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(-0.5f, 1.5f, 0.0f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(0.7f, 1.58f, 0.3f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(-0.7f, 1.2f, 0.0f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(0.4f, 1.4f, 0.2f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(0.3f, 1.0f, 0.2f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(0.4f, 2.3f, 0.2f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(-0.5f, 2.4f, 0.1f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(-1.5f, 1.9f, 0.1f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(1.7f, 2.2f, 0.1f), 0.05f, new DiffuseLight(fireflyColor)));
  list.push_back(new Sphere(vec3(1.9f, 1.7f, 0.1f), 0.05f, new DiffuseLight(fireflyColor)));

  list.push_back(new xz_rect(1.0f, 3.0f, 1.0f, 3.0f, 2.5f, new DiffuseLight(vec3{ 4, 4, 4 })));
  // list.push_back(new Cylinder(-1.5f, 2.0f, 6.0f, 360, new Dielectric(1.0f)));

  return new HittableList(list);
}

#endif
