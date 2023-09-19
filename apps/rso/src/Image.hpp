#pragma once

#include "Globals.hpp"

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

void SaveHDR(const char* filename, const dvec3* inImage, int width, int height, bool psf = false);