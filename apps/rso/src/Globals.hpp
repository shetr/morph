#pragma once

#include <Morph.hpp>
#include <random>

using namespace glm;

class RandGen {
public:
  RandGen(int seed) : eng(seed), distr(0.0, 1.0) {}
  double Generate() { return distr(eng); }
private:
  std::default_random_engine eng;
  std::uniform_real_distribution<double> distr;
};

class Globals
{
public:
    static const double epsilon;
    static const int rainbowPSC;    // 0 .. use CoolWarm mapping, 1 .. use rainbow color mapping
    static const int showBargraph;  // 0/1 .. dont use/use bargraph on the right for color mapping

    static bool useMultithreading;
    static std::vector<RandGen> randomGenerators;
    static const float pscols[4 * 33];
    static int screenWidth;
    static int screenHeight;
    static std::vector<dvec3> image;
    static std::vector<dvec3> reference;
    static std::vector<double> weight;

    static void resize_image(int _screenWidth, int _screenHeight);

    static double drandom();
};