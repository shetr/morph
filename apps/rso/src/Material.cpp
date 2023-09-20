#include "Material.hpp"

#include "Globals.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

#define USE_CUSTOM_BRDF

namespace Morph {

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
    #ifdef USE_CUSTOM_BRDF
        L = dvec3(0, 0, 0);
        double e1 = Globals::drandom();
        double e2 = Globals::drandom();

        double avgDiffAlbedo = average(diffuseAlbedo);
        if (e1 < avgDiffAlbedo) // sample diffuse
        {
            double length = sqrt(N.x * N.x + N.y * N.y);
            dvec3 T;
            if (abs(N.x) > Globals::epsilon && abs(N.y) > Globals::epsilon) {
            T = dvec3(N.y / length, -N.x / length, 0);
            } else if (abs(N.y) > Globals::epsilon) {
            length = sqrt(N.y * N.y + N.z * N.z);
            T = dvec3(0, -N.z / length, N.y / length);
            } else {
            length = sqrt(N.x * N.x + N.z * N.z);
            T = dvec3(-N.z / length, 0, N.x / length);
            }
            dvec3 B = cross(N, T);
            
            e1 = e1 / avgDiffAlbedo;

            // compute diffuse sample
            double sqrt1me1 = sqrt(1 - e1);
            double x = sqrt1me1 * cos(2 * M_PI * e2);
            double y = sqrt1me1 * sin(2 * M_PI * e2);
            double z = sqrt(e1);

            L = T * x + B * y + N * z;

            double cosTheta = dot(N, L);
            return cosTheta >= 0;
        }
        double avgSpecAlbedo = average(specularAlbedo);
        if (e1 < avgDiffAlbedo + avgSpecAlbedo) {
            dvec3 R = N * (2 * dot(V, N)) - V;

            dvec3 B = cross(N, R);
            dvec3 T = cross(R, B);
            
            e1 = (e1 - avgDiffAlbedo) / avgSpecAlbedo; 

            double sqrt1mPow = sqrt(1 - pow(e1, 2 / (shininess + 1)));
            double x = sqrt1mPow * cos(2 * M_PI * e2);
            double y = sqrt1mPow * sin(2 * M_PI * e2);
            double z = pow(e1, 1 / (shininess + 1));
            
            L = T * x + B * y + R * z;

            //printf("%f %f %f \n", L.x, L.y, L.z);
            double cosTheta = dot(N, L);
            return cosTheta >= 0;
        }

    // otherwise the contribution is 0
    return false;
    #else
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
    #endif
}

double Material::sampleProb(const dvec3 &N, const dvec3 &V, const dvec3 &L)
{
    #ifdef USE_CUSTOM_BRDF
        dvec3 R = N * (2 * dot(L, N)) - L;
        double cosAlpha = dot(V, R);
        double cosTheta = dot(N, L);
        if (cosTheta <= 0 || cosAlpha <= 0) {
        return 0;
        }
        double avgDiffAlbedo = average(diffuseAlbedo);
        double avgSpecAlbedo = average(specularAlbedo);
        double probDiffuse = avgDiffAlbedo * cosTheta / M_PI;
        double probSpecular = avgSpecAlbedo * (shininess + 1) * pow(cosAlpha, shininess) / (2 * M_PI);
        return probDiffuse + probSpecular;
    #else
        double cosTheta = dot(N, L);
        dvec3 R = N * dot(N, L) * 2.0 - L;
        double cosPhi = dot(V, R);
        if (cosTheta <= 0 || cosPhi <= 0) return 0;
        return average(diffuseAlbedo) * cosTheta / M_PI +
            average(specularAlbedo) * (shininess + 1) / 2.0 / M_PI *
            pow(cosPhi, shininess);
    #endif
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

    int x = glm::clamp((int)(hdrImage.width * phi / (2 * M_PI)), 0, (hdrImage.width - 1));
    int y = glm::clamp((int)(hdrImage.height * theta / M_PI), 0, (hdrImage.height - 1));
    return sampleBilinear(x, y);
}

dvec3& EnvMapMaterial::getLeRef(dvec3 dir)
{
    double theta = acos(dir.z);
    double phi = atan2(dir.y, dir.x);
    if (phi < 0) {
        phi += 2 * M_PI;
    }

    int x = glm::clamp((int)(hdrImage.width * phi / (2 * M_PI)), 0, (hdrImage.width - 1));
    int y = glm::clamp((int)(hdrImage.height * theta / M_PI), 0, (hdrImage.height - 1));
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

}