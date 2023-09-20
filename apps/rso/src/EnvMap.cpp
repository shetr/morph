#include "EnvMap.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Morph {

Distribution1D::Distribution1D(const std::vector<double>& f, int n)
{
    func.resize(n, 0);
    cdf.resize(n + 1, 0);
    count = n;
    std::copy(f.begin(), f.begin() + n, func.begin());
    ComputeStep1dCDF();
    invFuncInt = 1. / funcInt;
    invCount = 1. / count;
}

double Distribution1D::Sample(double u, double *pdf) {
    // Find surrounding cdf segments
    double *ptr = std::lower_bound(cdf.data(), cdf.data()+count+1, u);
    int offset = std::max(0, (int) (ptr-cdf.data()-1));
    // Return offset along current cdf segment
    u = (u - cdf[offset]) / (cdf[offset+1] - cdf[offset]);
    *pdf = func[offset] * invFuncInt;
    return offset + u;
}

void Distribution1D::ComputeStep1dCDF()
{
    for (int i = 0; i < count; ++i) {
        cdf[i + 1] = cdf[i] + (func[i] / (double)count);
    }
    funcInt = std::max(cdf[count], Globals::epsilon);
    for (int i = 0; i < count; ++i) {
        cdf[i + 1] /= funcInt;
    }
}

EnvMap::EnvMap(const char* hdrFilename) : Sphere(dvec3(), 1, new EnvMapMaterial(hdrFilename), true, 1)
{
    EnvMapMaterial* mat = (EnvMapMaterial*)material;
    const Image& hdrImage = mat->hdrImage;
    width = hdrImage.width;
    height = hdrImage.height;
    int size = hdrImage.width * hdrImage.height;
    intensities.resize(size, 0);
    for (int y = 0; y < height; ++y) {
        // compensate the poles
        double sinTheta = sin(M_PI * (double) y / (double)height);
        for (int x = 0; x < width; ++x) {
        int i = x + y * width;
        intensities[i] = std::max(dot(dvec3(0.2989, 0.5870, 0.1140), hdrImage.data[i]), Globals::epsilon);
        }
    }

    const double sigma = 1;
    int gauss_size = 7;
    std::vector<double> gauss;
    gauss.resize(gauss_size, 0);
    for (int g = 0; g  < gauss_size; ++g) {
        int dg = g - (gauss_size/2);
        gauss[g] = exp(-(dg * dg) / (2.0 * sigma * sigma)) / sqrt(2.0 * M_PI * sigma * sigma);
    }

    std::vector<double> tmpIntensities;
    tmpIntensities.resize(size, 0);

    // horizontal convolution
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
        int i = x + y * width;
        for (int g = 0; g  < gauss_size; ++g) {
            int dx = g - (gauss_size/2);
            int px = (x + dx + width) % width;
            int di = px + y * width;
            tmpIntensities[i] += gauss[g] * intensities[di];
        }
        }
    }
    // vertical convolution
    intensities.assign(size, 0);
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
        int i = x + y * width;
        for (int g = 0; g  < gauss_size; ++g) {
            int dy = g - (gauss_size/2);
            int py = y + dy;
            if (py < 0)
            py = -py;
            else if (py >= height)
            py = 2 * height - py - 2;
            int di = x + py * width;
            intensities[i] += gauss[g] * tmpIntensities[di];
        }
        }
    }

    std::vector<double> func;
    func.resize(std::max(width, height), 0);

    std::vector<double> sinVals;
    sinVals.resize(height, 0);
    for (int y = 0; y < height; ++y)
        sinVals[y] = sin(M_PI * ((double)y + 0.5) / (double)height);

    vDistribs.resize(width, Distribution1D());

    for (int x = 0; x < width; ++x) {
        // Compute sampling distribution for column x
        for (int y = 0; y < height; ++y) {
        int i = x + y * width;
        func[y] = intensities[i] * sinVals[y];
        }
        vDistribs[x] = Distribution1D(func, height);
    }

    // Compute sampling distribution for columns of image
    for (int x = 0; x < width; ++x) {
        func[x] = vDistribs[x].funcInt;
    }
    uDistrib = Distribution1D(func, width);

    // testing code
    #if 0
        g_testImage = mat->hdrImage;
        for (int i = 0; i < 200000; ++i)
        {
        dvec3 dir;
        dvec3 normal;
        samplePoint(dvec3(0), dir, normal);
        mat->getLeRef(dir) = dvec3(1,0,0);
        }
        std::swap(g_testImage, mat->hdrImage);
    #endif
}

Hit EnvMap::intersect(const Ray &r)
{
    Hit hit;
    hit.t = std::numeric_limits<double>::infinity();
    //hit.t = 1000;
    //hit.t = 1;
    hit.position = r.start + r.dir;
    hit.normal = -r.dir;
    hit.material = material;
    hit.object = this;
    return hit;
}

void EnvMap::samplePoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal, int workerId)
{
    double e1 = Globals::drandom(workerId);
    double pdfs[2];
    double fx = uDistrib.Sample(e1, &pdfs[0]);
    int x = glm::clamp((int)fx, 0, uDistrib.count-1);
    double e2 = Globals::drandom(workerId);
    double fy = vDistribs[x].Sample(e2, &pdfs[1]);

    double theta = fy * vDistribs[x].invCount * M_PI;
    double phi = fx * uDistrib.invCount * 2. * M_PI;

    dvec3 dir;
    dir.x = cos(phi) * sin(theta);
    dir.y = sin(phi) * sin(theta);
    dir.z = cos(theta);
    point = illuminatedPoint + dir;
    normal = -dir;
}

double EnvMap::pointSampleProb(double totalPower, dvec3 dir)
{
    double theta = acos(dir.z);
    double phi = atan2(dir.y, dir.x);
    if (phi < 0) {
        phi += 2 * M_PI;
    }

    int x = glm::clamp((int)(width * phi / (2 * M_PI)), 0, (width - 1));
    int y = glm::clamp((int)(height * theta / M_PI), 0, (height - 1));
    double puv = (uDistrib.func[x] * vDistribs[x].func[y]) / (uDistrib.funcInt * vDistribs[x].funcInt);
    //return puv * nu * nv / (2.0 * M_PI * M_PI * sinTheta);
    return (power / totalPower) * puv / (2.0 * M_PI * M_PI * sin(theta));
}

}