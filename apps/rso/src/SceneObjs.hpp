#pragma once

#include "Ray.hpp"

// Rectangle 2D in 3D space
class Rect : public Intersectable
{
  // anchor point, normal,
  vec3 r0, normal, right, forward;
  double width, height; // size
public:
  Rect(vec3 _r0, vec3 _r1, vec3 _r2, double _width, double _height, Material *mat);

  Rect(double _size, Material *mat);

  // Compute intersection between a ray and the rectangle
  Hit intersect(const Ray &ray);
};

// Sphere used as light source
struct Sphere : public Intersectable
{
  vec3 center;
  double radius;

  Sphere(const vec3 &cent, double rad, Material *mat, bool emit = true, const double targetPower = 60);

  virtual Hit intersect(const Ray &r) override;

  virtual void samplePoint(const vec3 &illuminatedPoint, vec3 &point, vec3 &normal);

  // find a random point with uniform distribution on that half sphere, which can be visible
  void sampleUniformPoint(const vec3 &illuminatedPoint, vec3 &point, vec3 &normal);

  virtual double pointSampleProb(double totalPower, vec3 dir) override;
};