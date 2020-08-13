#include <iostream>
#include <memory>

#include "renderer.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"
#include "materials/lambertian.h"
#include "materials/metal.h"
#include "materials/dielectric.h"

HittableList randomScene() {
  HittableList world;

  auto ground_material = std::make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
  world.add(std::make_shared<Sphere>(Point(0, -1000, 0), 1000, ground_material));

  for (int a = -11; a < 11; a++) {
	for (int b = -11; b < 11; b++) {
	  auto choose_mat = randomDouble();
	  Point center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());

	  if ((center - Point(4, 0.2, 0)).magnitude() > 0.9) {
		std::shared_ptr<Material> sphereMaterial;

		if (choose_mat < 0.8) {
		  // diffuse
		  auto albedo = Color::random() * Color::random();
		  sphereMaterial = std::make_shared<Lambertian>(albedo);
		  world.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
		} else if (choose_mat < 0.95) {
		  // Metal
		  auto albedo = Color::random(0.5, 1);
		  auto fuzz = randomDouble(0, 0.5);
		  sphereMaterial = std::make_shared<Metal>(albedo, fuzz);
		  world.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
		} else {
		  // glass
		  sphereMaterial = std::make_shared<Dielectric>(1.5);
		  world.add(std::make_shared<Sphere>(center, 0.2, sphereMaterial));
		}
	  }
	}
  }

  auto material1 = std::make_shared<Dielectric>(1.5);
  world.add(std::make_shared<Sphere>(Point(0, 1, 0), 1.0, material1));

  auto material2 = std::make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
  world.add(std::make_shared<Sphere>(Point(-4, 1, 0), 1.0, material2));

  auto material3 = std::make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
  world.add(std::make_shared<Sphere>(Point(4, 1, 0), 1.0, material3));

  return world;
}

int main() {

  // Image size
  const auto aspectRatio = 3.0 / 2.0;
  const int imageWidth = 400;
  const auto imageHeight = static_cast<int>(imageWidth / aspectRatio);
  const int samplesPerPixel = 10;
  const int maxDepth = 50;

  auto world = randomScene();

  vec3 lookfrom(13, 2, 3);
  vec3 lookat(0, 0, 0);
  vec3 vup(0, 1, 0);
  auto distToFocus = 10;
  auto aperture = 0.1;

  Camera camera(lookfrom, lookat, vup, 20, aspectRatio, aperture, distToFocus);

  // Render
  Renderer::render("output.ppm", imageWidth, imageHeight, world, camera, samplesPerPixel, maxDepth);
}
