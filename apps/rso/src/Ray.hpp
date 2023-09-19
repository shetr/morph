#ifndef RSO_RAY_HPP
#define RSO_RAY_HPP

#include "Material.hpp"

// Structure for a ray
struct Ray
{
  dvec3 start;
  dvec3 dir;
  Ray(const dvec3 &_start, const dvec3 &_dir)
  {
    start = _start;
    dir = normalize(_dir);
  }
};

class Intersectable;

// Structure to store the result of ray tracing
struct Hit
{
  double t;
  dvec3 position;
  dvec3 normal;
  Material *material;
  Intersectable *object;
  Hit() { t = -1; }
};

// Abstract 3D object
struct Intersectable
{
  Material *material;
  double power = 0;
  virtual Hit intersect(const Ray &ray) = 0;
  virtual double pointSampleProb(double totalPower, dvec3 dir)
  {
    printf("Point sample on table\n");
    return 0;
  }
};

#endif // RSO_RAY_HPP