#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.h"
#include "ray.h"

class Camera {
  public:
	Camera(Point lookFrom, Point lookAt, vec3 vUp, double vfov, double aspectRatio, double aperture, double focusDist) {
	  auto theta = degreesToRadians(vfov);
	  auto h = tan(theta / 2);
	  auto viewportHeight = 2.0 * h;
	  auto viewportWidth = aspectRatio * viewportHeight;

	  w = normalize(lookFrom - lookAt);
	  u = normalize(cross(vUp, w));
	  v = cross(w, u);

	  m_origin = lookFrom;
	  m_horizontal = focusDist * viewportWidth * u;
	  m_vertical = focusDist * viewportHeight * v;
	  m_lowerLeftCorner = m_origin - m_horizontal / 2 - m_vertical / 2 - focusDist * w;

	  m_lensRadius = aperture / 2;
	}

	Ray getRay(double s, double t) const {
	  auto rd = m_lensRadius * randomInUnitDisk();
	  auto offset = u * rd.x() + v * rd.y();
	  return { m_origin + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - m_origin - offset };
	}

  private:
	Point m_origin;
	Point m_lowerLeftCorner;
	vec3 m_horizontal;
	vec3 m_vertical;

	vec3 u, v, w;
	double m_lensRadius;
};

#endif
