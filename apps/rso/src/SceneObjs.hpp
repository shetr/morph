#ifndef RSO_SCENE_OBJS_HPP
#define RSO_SCENE_OBJS_HPP

#include "Ray.hpp"

// Rectangle 2D in 3D space
class Rect : public Intersectable
{
  // anchor point, normal,
  dvec3 r0, normal, right, forward;
  double width, height; // size
public:
  Rect(dvec3 _r0, dvec3 _r1, dvec3 _r2, double _width, double _height, Material *mat);

  Rect(double _size, Material *mat);

  // Compute intersection between a ray and the rectangle
  Hit intersect(const Ray &ray);
};

// Sphere used as light source
struct Sphere : public Intersectable
{
  dvec3 center;
  double radius;

  Sphere(const dvec3 &cent, double rad, Material *mat, bool emit = true, const double targetPower = 60);

  virtual Hit intersect(const Ray &r) override;

  virtual void samplePoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal);

  // find a random point with uniform distribution on that half sphere, which can be visible
  void sampleUniformPoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal);

  virtual double pointSampleProb(double totalPower, dvec3 dir) override;
};

#endif // RSO_SCENE_OBJS_HPP