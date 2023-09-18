#include "Globals.hpp"

#include <thread>

const double Globals::epsilon = 1e-9;
bool Globals::useMultithreading = false;
std::vector<RandGen> Globals::randomGenerators;

double Globals::drandom()
{
    if (useMultithreading) {
        return randomGenerators[(int)std::thread::hardware_concurrency()].Generate();
    } else {
        return (double)rand() / RAND_MAX;
    }
}
