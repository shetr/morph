#include "Material.hpp"

#include "Globals.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

dvec3 Material::BRDF(const dvec3 &N, const dvec3 &V, const dvec3 &L)
{
    dvec3 brdf(0, 0, 0);
    double cosThetaL = dot(N, L);
    double cosThetaV = dot(N, V);
    if (cosThetaL <= Globals::epsilon || cosThetaV <= Globals::epsilon)
        return brdf;
    brdf = diffuseAlbedo / M_PI; // diffuse part
    dvec3 R = N * (dot(N, L) * 2.0) - L;
    double cosPhi = dot(V, R);
    if (cosPhi <= 0)
        return brdf; // farther by PI/2 from reflected direction
    // max-Phong specular BRDF: symmetric and energy conserving
    return brdf + specularAlbedo * ((shininess + 1.0) / 2.0 / M_PI * pow(cosPhi, shininess) / fmax(cosThetaL, cosThetaV));
}

bool Material::sampleDirection(const dvec3 &N, const dvec3 &V, dvec3 &L)
{ // output - the incoming light direction
    // To be implemented during exercise 1

    double r = Globals::drandom();
    if ((r -= average(diffuseAlbedo)) < 0) {
        double u = Globals::drandom(), v = Globals::drandom();
        double theta = asin(sqrt(u)), phi = M_PI * 2.0 * v;
        dvec3 O = cross(N, dvec3(1, 0, 0));
        if (length(O) < Globals::epsilon) O = cross(N, dvec3(0, 0, 1));
        dvec3 P = cross(N, O);
        L = N * cos(theta) + O * sin(theta) * cos(phi) + P * sin(theta) * sin(phi);
        return true;
    }
    if ((r -= average(specularAlbedo)) < 0) {
        double u = Globals::drandom(), v = Globals::drandom();
        double cos_ang_V_R = pow(u, 1.0 / (shininess + 1.0));
        double sin_ang_V_R = sqrt(1.0 - cos_ang_V_R * cos_ang_V_R);
        dvec3 O = cross(V, dvec3(1, 0, 0));
        if (length(O) < Globals::epsilon) O = cross(N, dvec3(0, 0, 1));
        O = normalize(O);
        dvec3 P = cross(V, O);
        dvec3 R = O * sin_ang_V_R * cos(2.0 * M_PI * v) +
        P * sin_ang_V_R * sin(2.0 * M_PI * v) +
        V * cos_ang_V_R;
        L = N * dot(N, R) * 2.0 - R;
        return true;
    }
    return false;
}

double Material::sampleProb(const dvec3 &N, const dvec3 &V, const dvec3 &L)
{
    double cosTheta = dot(N, L);
    dvec3 R = N * dot(N, L) * 2.0 - L;
    double cosPhi = dot(V, R);
    if (cosTheta <= 0 || cosPhi <= 0) return 0;
    return average(diffuseAlbedo) * cosTheta / M_PI +
        average(specularAlbedo) * (shininess + 1) / 2.0 / M_PI *
        pow(cosPhi, shininess);
}

EnvMapMaterial::EnvMapMaterial(const char* hdrFilename)
{
    diffuseAlbedo = dvec3(0);
    specularAlbedo = dvec3(0);
    hdrImage = ReadHDR(hdrFilename);
    int size = hdrImage.width * hdrImage.height;
    /*double max = Globals::epsilon;
    for (int i = 0; i < size; ++i) {
        dvec3 c = hdrImage.data[i];
        max = std::max(max, std::max(c.x, std::max(c.y, c.z)));
    }
    double invMax = 1 / max;*/
    Le = dvec3();
    for (int i = 0; i < size; ++i) {
        //hdrImage.data[i] *= invMax;
        Le += hdrImage.data[i];
    }
    //Le = Le * (1.0 / size);
}

dvec3 EnvMapMaterial::getLe(dvec3 dir) const
{
    double theta = acos(dir.z);
    double phi = atan2(dir.y, dir.x);
    if (phi < 0) {
        phi += 2 * M_PI;
    }

    int x = clamp((int)(hdrImage.width * phi / (2 * M_PI)), 0, (hdrImage.width - 1));
    int y = clamp((int)(hdrImage.height * theta / M_PI), 0, (hdrImage.height - 1));
    return sampleBilinear(x, y);
}

dvec3& EnvMapMaterial::getLeRef(dvec3 dir)
{
    double theta = acos(dir.z);
    double phi = atan2(dir.y, dir.x);
    if (phi < 0) {
        phi += 2 * M_PI;
    }

    int x = clamp((int)(hdrImage.width * phi / (2 * M_PI)), 0, (hdrImage.width - 1));
    int y = clamp((int)(hdrImage.height * theta / M_PI), 0, (hdrImage.height - 1));
    return hdrImage.data[x + y * hdrImage.width];
}

dvec3 EnvMapMaterial::sample(int x, int y) const
{
    return hdrImage.data[x + y * hdrImage.width];
}

dvec3 EnvMapMaterial::sampleBilinear(double x, double y) const
{
    int x0 = (int)x;
    int y0 = (int)y;
    double tx = x - (double)x0;
    double ty = y - (double)y0;
    int x1 = (int)ceil(x);
    int y1 = (int)ceil(y);
    return (1 - ty) * ((1 - tx) * sample(x0, y0) + tx * sample(x1, y0)) + ty * ((1 - tx) * sample(x0, y1) + tx * sample(x1, y1)); 
}