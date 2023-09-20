#ifndef RSO_ENVMAP_HPP
#define RSO_ENVMAP_HPP

#include "SceneObjs.hpp"

namespace Morph {

struct Distribution1D
{
  std::vector<double> func;
  std::vector<double> cdf;
  double funcInt;
  double invFuncInt;
  double invCount;
  int count;

  Distribution1D(){}
  
  Distribution1D(const std::vector<double>& f, int n);

  double Sample(double u, double *pdf);

private:
  void ComputeStep1dCDF();
};

struct EnvMap : public Sphere
{
  int width;
  int height;
  std::vector<double> intensities;
  
  Distribution1D uDistrib;
  std::vector<Distribution1D> vDistribs;

  EnvMap(const char* hdrFilename);

  Hit intersect(const Ray &r) override;

  void samplePoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal) override;

  double pointSampleProb(double totalPower, dvec3 dir) override;
};

}

#endif // RSO_ENVMAP_HPP