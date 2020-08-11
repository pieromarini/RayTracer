#ifndef RAY_H
#define RAY_H

/*
 * P(t) = A + tb
 * A -> origin
 * b -> direction
 */

#include "vec3.h"

class Hittable;

class Ray {

  public:
	Ray() {}
	Ray(const Point& origin, const vec3& direction) : m_origin{ origin }, m_direction{ direction } {}

	Point origin() const { return m_origin; }
	vec3 direction() const { return m_direction; }

	Point at(double t) const {
	  return m_origin + t * m_direction;
	}

  private:
	Point m_origin;
	vec3 m_direction;
};

#endif
