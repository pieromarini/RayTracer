#ifndef RENDERER_H
#define RENDERER_H

#include <fstream>
#include <string>
#include <ostream>
#include <string_view>
#include <vector>

#include "vec3.h"
#include "camera.h"
#include "ray.h"
#include "hittable.h"


Color rayColor(const Ray& r, const Hittable& world, int depth) {
  if (depth <= 0)
	return { 0, 0, 0 };

  if (auto hit = world.hit(r, 0.001, infinity); hit) {
	Ray scattered;
	Color attenuation;
	if (hit->material->scatter(r, hit.value(), attenuation, scattered))
	  return attenuation * rayColor(scattered, world, depth - 1);
	return { 0, 0, 0 };
  }

  auto unitDirection = normalize(r.direction());
  auto t = 0.5 * (unitDirection.y() + 1.0);
  return (1.0 - t) * Color{ 1.0, 1.0, 1.0 } + t * Color{ 0.5, 0.7, 1.0 };
}


class Renderer {
  public:
	static void render(std::string filepath, const int width, const int height, Hittable& world, Camera& camera, int samplesPerPixel, int maxDepth) {

	  std::ofstream output{ filepath };

	  output << "P3\n" << width << ' ' << height << "\n255\n";
	  for (int j = height - 1; j >= 0; --j) {
		for (int i = 0; i < width; ++i) {
		  Color pixel{ 0, 0, 0 };
		  for (int s = 0; s < samplesPerPixel; ++s) {
			auto u = (i + randomDouble()) / (width - 1);
			auto v = (j + randomDouble()) / (height - 1);
			auto ray = camera.getRay(u, v);
			pixel += rayColor(ray, world, maxDepth);
		  }
		  writeToStream(output, pixel, samplesPerPixel);
		}
	  }
	}

  private:
	static void writeToStream(std::ostream& out, Color pixel, int samplesPerPixel) {
	  auto r = pixel.x();
	  auto g = pixel.y();
	  auto b = pixel.z();

	  auto scale = 1.0 / samplesPerPixel;
	  r = std::sqrt(scale * r);
	  g = std::sqrt(scale * g);
	  b = std::sqrt(scale * b);

	  out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		  << static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		  << static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
	}
};

#endif
