#ifndef VEC3_H
#define VEC3_H

#include <iostream>
#include <math.h>

#include "utils.h"

struct vec3 {
  vec3() : data{ 0, 0, 0 } {}
  vec3(double d1, double d2, double d3) : data{ d1, d2, d3 } {}

  double x() const { return data[0]; }
  double y() const { return data[1]; }
  double z() const { return data[2]; }

  inline static vec3 random() {
	return { randomDouble(), randomDouble(), randomDouble() };
  }

  inline static vec3 random(double min, double max) {
	return { randomDouble(min, max), randomDouble(min, max), randomDouble(min, max) };
  }

  vec3 operator-() const { return { -data[0], -data[1], -data[2] }; }

  double operator[](int index) const { return data[index]; }
  double& operator[](int index) { return data[index]; }

  vec3& operator+=(const vec3& rhs) {
	data[0] += rhs.data[0];
	data[1] += rhs.data[1];
	data[2] += rhs.data[2];
	return *this;
  }

  vec3& operator*=(const double scalar) {
	data[0] *= scalar;
	data[1] *= scalar;
	data[2] *= scalar;
	return *this;
  }

  vec3& operator/=(const double scalar) {
	return *this *= 1 / scalar;
  }

  double magnitude() {
	return std::sqrt(squaredMagnitude());
  }

  double squaredMagnitude() {
	return data[0] * data[0] + data[1] * data[1] + data[2] * data[2];
  }

  double data[3];
};

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
  return out << v.data[0] << ' ' << v.data[1] << ' ' << v.data[2];
}

inline vec3 operator+(const vec3& lhs, const vec3& rhs) {
  return { lhs.data[0] + rhs.data[0], lhs.data[1] + rhs.data[1], lhs.data[2] + rhs.data[2] };
}

inline vec3 operator-(const vec3& lhs, const vec3& rhs) {
  return { lhs.data[0] - rhs.data[0], lhs.data[1] - rhs.data[1], lhs.data[2] - rhs.data[2] };
}

inline vec3 operator*(const vec3& lhs, const vec3& rhs) {
  return { lhs.data[0] * rhs.data[0], lhs.data[1] * rhs.data[1], lhs.data[2] * rhs.data[2] };
}

inline vec3 operator*(double scalar, const vec3& rhs) {
  return { scalar * rhs.data[0], scalar * rhs.data[1], scalar * rhs.data[2] };
}

inline vec3 operator*(const vec3& lhs, double scalar) {
  return scalar * lhs;
}

inline vec3 operator/(vec3 lhs, double scalar) {
  return (1 / scalar) * lhs;
}

inline double dot(const vec3& lhs, const vec3& rhs) {
  return lhs.data[0] * rhs.data[0] + lhs.data[1] * rhs.data[1] + lhs.data[2] * rhs.data[2];
}

inline vec3 cross(const vec3& lhs, const vec3& rhs) {
  return {
	lhs.data[1] * rhs.data[2] - lhs.data[2] * rhs.data[1],
	lhs.data[2] * rhs.data[0] - lhs.data[0] * rhs.data[2],
	lhs.data[0] * rhs.data[1] - lhs.data[1] * rhs.data[0]
  };
}

vec3 normalize(vec3 v) {
  return v / v.magnitude();
}

vec3 reflect(const vec3& v, const vec3& n) {
  return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, double etaiOverEtat) {
  auto cosTheta = dot(-uv, n);
  auto rOutOrth = etaiOverEtat * (uv + cosTheta * n);
  auto  rOutParallel = -std::sqrt(fabs(1.0 - rOutOrth.squaredMagnitude())) * n;
  return rOutOrth * rOutParallel;
}

vec3 randomInUnitSphere() {
  while(true) {
	auto p = vec3::random(-1, 1);
	if (p.squaredMagnitude() >= 1)
	  continue;

	return p;
  }
}

vec3 randomInUnitDisk() {
  while(true) {
	auto p = vec3{ randomDouble(-1, 1), randomDouble(-1, 1), 0 };
	if (p.squaredMagnitude() >= 1)
	  continue;
	return p;
  }
}

vec3 randomUnitVector() {
  auto a  = randomDouble(0, 2 * pi);
  auto z  = randomDouble(-1, 1);
  auto r = std::sqrt(1 - z * z);
  return { r * std::cos(a), r * std::sin(a), z };
}

using Point = vec3;
using Color = vec3;

#endif
