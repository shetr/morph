#ifndef RSO_GLOBALS_HPP
#define RSO_GLOBALS_HPP

#include <Morph.hpp>
#include <Core/Arrays.hpp>

#include <random>

//#include "vec.hpp"

namespace Morph {

// Which sampling method should be used
enum Method
{
  BRDF,
  LIGHT_SOURCE,
  HALF_WEIGHT,
  MULTIPLE_IMPORTANCE,
  PATH_TRACING
};

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
    static float gamma;
    static float exposure;
    static const double epsilon;
    static const int rainbowPSC;    // 0 .. use CoolWarm mapping, 1 .. use rainbow color mapping
    static const int showBargraph;  // 0/1 .. dont use/use bargraph on the right for color mapping

    static int nTotalSamples; // samples in one render iteration - should be even number
    static int samplesPerFrame;
    static int currentNumSamples;

    static Method method;

    static bool useMultithreading;
    static std::vector<RandGen> randomGenerators;
    static const float pscols[4 * 33];
    static uvec2 screenSize;
    static vector2d<dvec3> radianceAccumulator;
    static vector2d<vec3> hdrImage;
    static vector2d<vec3> ldrImage;
    static float weight;

    static void resize_image(uvec2 _screenSize);
    static void clear();

    static double drandom();
};

}

#endif // RSO_GLOBALS_HPP