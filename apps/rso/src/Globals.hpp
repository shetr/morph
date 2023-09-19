#ifndef RSO_GLOBALS_HPP
#define RSO_GLOBALS_HPP

#include <glm/glm.hpp> 
#include <random>

//using namespace glm;
using dvec3 = glm::dvec3;
using glm::clamp;

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
    static const double epsilon;
    static const int rainbowPSC;    // 0 .. use CoolWarm mapping, 1 .. use rainbow color mapping
    static const int showBargraph;  // 0/1 .. dont use/use bargraph on the right for color mapping

    static int nIterations;    // how many iterations to render
    static int nTotalSamples; // samples in one render iteration - should be even number

    // The cost of sampling - should be measured and set
    static double costBRDF;
    static double costLight;
    static double referenceEfficiency;

    static Method method;

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

#endif // RSO_GLOBALS_HPP