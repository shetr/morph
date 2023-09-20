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

Image ReadHDR(const char* filename);

void SaveHDR(const char* filename, const vector2d<vec3>& inImage);

void SaveTGA(const char* filename, const vector2d<vec3>& inImage);

}

#endif // RSO_IMAGE_HPP