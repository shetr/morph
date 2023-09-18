#pragma once

#include "Material.hpp"

// Structure for a ray
struct Ray
{
  vec3 start, dir;
  Ray(const vec3 &_start, const vec3 &_dir)
  {
    start = _start;
    dir = _dir.normalize();
  }
};

class Intersectable;

// Structure to store the result of ray tracing
struct Hit
{
  double t;
  vec3 position;
  vec3 normal;
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
  virtual double pointSampleProb(double totalPower, vec3 dir)
  {
    printf("Point sample on table\n");
    return 0;
  }
};