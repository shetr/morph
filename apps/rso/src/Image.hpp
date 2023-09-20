#ifndef RSO_IMAGE_HPP
#define RSO_IMAGE_HPP

#include "Globals.hpp"

namespace Morph {

double average(dvec3 v);

struct RGBE
{
  unsigned char r, g, b, e;
};

struct Image
{
  std::vector<dvec3> data;
  int width;
  int height;
};

void getPseudocolorRainbow(double val, double minVal, double maxVal, double &r, double &g, double &b);

void getPseudocolorCoolWarm(double val, double minVal, double maxVal, double &r, double &g, double &b);

Image ReadHDR(const char* filename);

void SaveHDR(const char* filename, const vec3* inImage, int width, int height, bool psf = false);

void SaveTGA();

}

#endif // RSO_IMAGE_HPP