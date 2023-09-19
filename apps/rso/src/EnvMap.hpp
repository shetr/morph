#pragma once

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

  void samplePoint(const vec3 &illuminatedPoint, vec3 &point, vec3 &normal) override;

  double pointSampleProb(double totalPower, vec3 dir) override;
};