#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

class Camera {
  public:
	Camera() {
	  const auto aspectRatio = 16.0 / 9.0;
	  auto viewportHeight = 2.0;
	  auto viewportWidth = aspectRatio * viewportHeight;
	  auto focalLength = 1.0;
	  m_origin = Point{ 0, 0, 0 };
	  m_horizontal = vec3{ viewportWidth, 0, 0 };
	  m_vertical = vec3{ 0, viewportHeight, 0 };
	  m_lowerLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - vec3{ 0, 0, focalLength };
	}

	Ray getRay(double u, double v) const {
	  return { m_origin, m_lowerLeftCorner + u * m_horizontal + v * m_vertical - m_origin };
	}

  private:
	Point m_origin;
	Point m_lowerLeftCorner;
	vec3 m_horizontal;
	vec3 m_vertical;
};

#endif
