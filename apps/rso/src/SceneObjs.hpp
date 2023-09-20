#ifndef RSO_SCENE_OBJS_HPP
#define RSO_SCENE_OBJS_HPP

#include "Ray.hpp"

namespace Morph {

// Rectangle 2D in 3D space
class Rect : public Intersectable
{
  // anchor point, normal,
  dvec3 r0 = dvec3(0);
  dvec3 normal = dvec3(0);
  dvec3 right = dvec3(0);
  dvec3 forward = dvec3(0);
  double width = 0;
  double height = 0; // size
public:
  Rect(dvec3 _r0, dvec3 _r1, dvec3 _r2, double _width, double _height, Material *mat);

  Rect(double _size, Material *mat);

  // Compute intersection between a ray and the rectangle
  virtual Hit intersect(const Ray &ray) override;
};

// Sphere used as light source
struct Sphere : public Intersectable
{
  dvec3 center = dvec3(0);
  double radius = 0;

  Sphere(const dvec3 &cent, double rad, Material *mat, bool emit = true, const double targetPower = 60);

  virtual Hit intersect(const Ray &r) override;

  virtual void samplePoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal, int workerId);

  // find a random point with uniform distribution on that half sphere, which can be visible
  void sampleUniformPoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal, int workerId);

  virtual double pointSampleProb(double totalPower, dvec3 dir) override;
};

}

#endif // RSO_SCENE_OBJS_HPP