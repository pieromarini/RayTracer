#include <random>

const double pi = 3.1415926535897932385;
const double infinity = std::numeric_limits<double>::infinity();

inline double randomDouble() {
  static std::uniform_real_distribution<double> dist(0.0, 1.0);
  static std::mt19937 gen;
  return dist(gen);
}

inline double randomDouble(double min, double max) {
  return min + (max - min) * randomDouble();
}

inline double clamp(double x, double min, double max) {
  if (x < min) return min;
  if (x > max) return max;
  return x;
}
