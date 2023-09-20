#ifndef RSO_SCENE_HPP
#define RSO_SCENE_HPP

#include "EnvMap.hpp"

// The light source represented by a sphere
struct LightSource
{
  Sphere *sphere;
  dvec3 point;
  dvec3 normal;
  LightSource(Sphere *_sphere, dvec3 _point, dvec3 _normal)
  {
    sphere = _sphere, point = _point;
    normal = _normal;
  }
};

// Definition of the camera
class Camera
{
  // center of projection and orthogonal basis of the camera
  dvec3 eye, lookat, right, up;

public:
  void set(const dvec3 &_eye, const dvec3 &_lookat, const dvec3 &_vup, double fov);
  Ray getRay(int X, int Y);
};

// The scene definition with main rendering method
class Scene
{
  std::vector<Intersectable *> objects;
  double totalPower;
  int nLightSamples, nBRDFSamples;

public:
  Camera camera;

  void buildHw1_3Test();

  void buildHw2Test(const char* hdrFilename);

  void buildHw4Test(const char* hdrFilename);

  void build(const char* hdrFilename = "raw013.hdr");

  // Render the scene
  void render();

  // Compute intersection between a rady and primitive
  Hit firstIntersect(const Ray &ray, Intersectable *skip);

  // Sample the light source from all the light sources in the scene
  LightSource sampleLightSource(const dvec3 &illuminatedPoint);

  dvec3 pathTrace(const Ray &primaryRay);

  // Trace a primary ray towards the scene
  dvec3 trace(const Ray &r);

  // Only testing routine for debugging
  void testRay(int X, int Y);
};

#endif // RSO_SCENE_HPP