#ifndef RAYTRACER_H
#define RAYTRACER_H

#include <cfloat>
#include <cstdlib>
#include <iostream>
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "ray.h"
#include "sphere.h"
#include "vec3.h"

vec3 rayColor(const Ray &r, Hittable *world, int depth) {
  HitRecord rec;
  if (world->hit(r, 0.001f, MAXFLOAT, rec)) {
    Ray scattered;
    vec3 attenuation;
    if (depth < 50 && rec.mat->scatter(r, rec, attenuation, scattered))
      return attenuation * rayColor(scattered, world, depth + 1);
    else
      return vec3(0, 0, 0);
  }

  vec3 dir = normalize(r.direction());
  float t = 0.5f * (dir.y + 1.0f);
  return (1.0f - t) * vec3(1.0f, 1.0f, 1.0f) + t * vec3(0.5f, 0.7f, 1.0f);
}

Hittable* randomScene() {
  const int n = 500;
  Hittable** list = new Hittable *[n + 1];
  list[0] = new Sphere(vec3(0, -1000, 0), 1000, new Lambertian(vec3(0.5f, 0.5f, 0.5f)));
  int i = 1;
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      float choose_mat = drand48();
      vec3 center(a + 0.9f * drand48(), 0.2f, b + 0.9f * drand48());
      if ((center - vec3(4.f, 0.2f, 0.f)).magnitude() > 0.9) {
        if (choose_mat < 0.7) {  // diffuse
          list[i++] = new Sphere( center, 0.2f, new Lambertian(vec3(drand48() * drand48(), drand48() * drand48(), drand48() * drand48())));
        } else if (choose_mat < 0.9) {  // metal
          list[i++] = new Sphere(
              center, 0.2f,
              new Metal(vec3(0.5f * (1.f + drand48()), 0.5f * (1.f + drand48()), 0.5f * (1.f + drand48())), drand48() * drand48() * drand48()));
        } else {  // Dielectric
          list[i++] =
              new Sphere(center, 0.2f, new Dielectric(1.f + 3.f * drand48()));
        }
      }
    }
  }

  list[i++] = new Sphere(vec3(0.f, 1.f, 0.f), 1.f, new Dielectric(1.5f));
  list[i++] = new Sphere(vec3(-4.f, 1.f, 0.f), 1.f,
                         new Lambertian(vec3(0.4f, 0.2f, 0.1f)));
  list[i++] = new Sphere(vec3(4.f, 1.f, 0.f), 1.f,
                         new Metal(vec3(0.7f, 0.6f, 0.5f), 0.f));

  return new HittableList(list, i);
}

#endif
