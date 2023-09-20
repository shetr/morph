#include "Globals.hpp"

#include <thread>

namespace Morph {

float Globals::gamma = 2.2;
float Globals::exposure = 1.0;
const double Globals::epsilon = 1e-9;
const int Globals::rainbowPSC = 0;
const int Globals::showBargraph = 1;
int Globals::nTotalSamples = 600;
int Globals::samplesPerFrame = 1;
int Globals::currentNumSamples = 1;
Method Globals::method = Method::BRDF;
bool Globals::useMultithreading = true;
std::vector<RandGen> Globals::randomGenerators;
const float Globals::pscols[4 * 33] = { // 33 colors RGB
    0, 0.2298057, 0.298717966, 0.753683153, 0.03125, 0.26623388, 0.353094838, 0.801466763,
    0.0625, 0.30386891, 0.406535296, 0.84495867, 0.09375, 0.342804478, 0.458757618, 0.883725899,
    0.125, 0.38301334, 0.50941904, 0.917387822, 0.15625, 0.424369608, 0.558148092, 0.945619588,
    0.1875, 0.46666708, 0.604562568, 0.968154911, 0.21875, 0.509635204, 0.648280772, 0.98478814,
    0.25, 0.552953156, 0.688929332, 0.995375608, 0.28125, 0.596262162, 0.726149107, 0.999836203,
    0.3125, 0.639176211, 0.759599947, 0.998151185, 0.34375, 0.681291281, 0.788964712, 0.990363227,
    0.375, 0.722193294, 0.813952739, 0.976574709, 0.40625, 0.761464949, 0.834302879, 0.956945269,
    0.4375, 0.798691636, 0.849786142, 0.931688648, 0.46875, 0.833466556, 0.860207984, 0.901068838,
    0.5, 0.865395197, 0.86541021, 0.865395561, 0.53125, 0.897787179, 0.848937047, 0.820880546,
    0.5625, 0.924127593, 0.827384882, 0.774508472, 0.59375, 0.944468518, 0.800927443, 0.726736146,
    0.625, 0.958852946, 0.769767752, 0.678007945, 0.65625, 0.96732803, 0.734132809, 0.628751763,
    0.6875, 0.969954137, 0.694266682, 0.579375448, 0.71875, 0.966811177, 0.650421156, 0.530263762,
    0.75, 0.958003065, 0.602842431, 0.481775914, 0.78125, 0.943660866, 0.551750968, 0.434243684,
    0.8125, 0.923944917, 0.49730856, 0.387970225, 0.84375, 0.89904617, 0.439559467, 0.343229596,
    0.875, 0.869186849, 0.378313092, 0.300267182, 0.90625, 0.834620542, 0.312874446, 0.259301199,
    0.9375, 0.795631745, 0.24128379, 0.220525627, 0.96875, 0.752534934, 0.157246067, 0.184115123,
    1.0, 0.705673158, 0.01555616, 0.150232812
};
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