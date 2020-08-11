#include <iostream>
#include <memory>

#include "renderer.h"
#include "hittable_list.h"
#include "sphere.h"

int main() {

  // Image size
  const auto aspectRatio = 16.0 / 9.0;
  const int imageWidth = 400;
  const auto imageHeight = static_cast<int>(imageWidth / aspectRatio);
  const int samplesPerPixel = 100;
  const int maxDepth = 50;

  HittableList world;
  world.add(std::make_shared<Sphere>(Point{ 0, 0, -1}, 0.5));
  world.add(std::make_shared<Sphere>(Point{ 0, -100.5, -1}, 100));

  Camera camera;

  // Render
  Renderer::render("output.ppm", imageWidth, imageHeight, world, camera, samplesPerPixel, maxDepth);
}
