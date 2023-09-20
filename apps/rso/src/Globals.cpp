#include "Globals.hpp"

#include <thread>

namespace Morph {

float Globals::gamma = 2.2;
float Globals::exposure = 1.0;
const double Globals::epsilon = 1e-9;
int Globals::nTotalSamples = 600;
int Globals::samplesPerFrame = 1;
int Globals::currentNumSamples = 1;
Method Globals::method = Method::BRDF;
bool Globals::useMultithreading = true;
std::vector<RandGen> Globals::randomGenerators;
uvec2 Globals::screenSize = uvec2(600, 600);
vector2d<dvec3> Globals::radianceAccumulator;
vector2d<vec3> Globals::hdrImage;
vector2d<vec3> Globals::ldrImage;
float Globals::weight = 0;

void Globals::resize_image(uvec2 _screenSize)
{
    screenSize = _screenSize;
    radianceAccumulator.assign(screenSize, dvec3(0));
    hdrImage.assign(screenSize, vec3(0));
    ldrImage.assign(screenSize, vec3(0));
}

void Globals::clear()
{
    srand(1);
    currentNumSamples = samplesPerFrame;
    radianceAccumulator.assign(screenSize, dvec3(0));
}

void Globals::init()
{
    srand(1);
    randomGenerators.clear();
    for (int i = 0; i < (int)std::thread::hardware_concurrency(); ++i)
      randomGenerators.push_back(RandGen(i + 1));
}

double Globals::drandom(int workerId)
{
    if (useMultithreading) {
        std::thread::id this_id = std::this_thread::get_id();
        return randomGenerators[workerId].Generate();
    } else {
        return (double)rand() / RAND_MAX;
    }
}

}