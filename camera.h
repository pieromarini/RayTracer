#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

class Camera {
 public:
  Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focusDist);
  Ray getRay(float u, float v) const;

  vec3 m_origin;
  vec3 m_lowerLeftCorner;
  vec3 m_horizontal;
  vec3 m_vertical;
  vec3 u, v, w;
  float lensRadius;
};

vec3 randomInUnitDisk() {
  vec3 p;
  do {
    p = 2.f * vec3(drand48(), drand48(), 0.f) - vec3(1.f, 1.f, 0.f);
  } while (dot(p, p) >= 1.f);
  return p;
}

Camera::Camera(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect, float aperture, float focusDist) {
  lensRadius = aperture / 2.f;
  float theta = vfov * M_PI / 180.f;
  float halfHeight = tan(theta / 2);
  float halfWidth = aspect * halfHeight;
  m_origin = lookfrom;
  w = normalize(lookfrom - lookat);
  u = normalize(cross(vup, w));
  v = cross(w, u);
  m_lowerLeftCorner = m_origin - halfWidth * focusDist * u - halfHeight * focusDist * v - focusDist * w;
  m_horizontal = 2.f * halfWidth * focusDist * u;
  m_vertical = 2.f * halfHeight * focusDist * v;
}

Ray Camera::getRay(float s, float t) const {
  vec3 rd = lensRadius * randomInUnitDisk();
  vec3 offset = u * rd.x + v * rd.y;
  return Ray(m_origin + offset, m_lowerLeftCorner + s * m_horizontal + t * m_vertical - offset);
}

#endif
