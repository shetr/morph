#pragma once

#include <random>

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
    static bool useMultithreading;
    static std::vector<RandGen> randomGenerators;

    static double drandom();
};