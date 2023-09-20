#include "Scene.hpp"

#include <iostream>
#include <chrono>
#define _USE_MATH_DEFINES
#include <math.h>

namespace Morph {

void Camera::set(const dvec3 &_eye, const dvec3 &_lookat, const dvec3 &_vup, double fov)
{
    eye = _eye;
    lookat = _lookat;
    dvec3 w = eye - lookat;
    double f = length(w);
    right = normalize(cross(_vup, w)) * f * tan(fov / 2);
    up = normalize(cross(w, right)) * f * tan(fov / 2);
}
Ray Camera::getRay(int X, int Y)
{ // X,Y - pixel coordinates, compute a primary ray
    dvec3 dir = lookat +
                right * (2.0 * (X + 0.5) / Globals::screenSize.x - 1) +
                up * (2.0 * (Y + 0.5) / Globals::screenSize.y - 1) - eye;
    return Ray(eye, normalize(dir));
}

void Scene::buildHw1_3Test()
{
    dvec3 eyePos(0, 6, 18);         // camera center
    dvec3 lightCenterPos(0, 4, -6); // first light source

    // Create geometry - 4 rectangles
    //objects.push_back(new Rect(dvec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, dvec3(0), dvec3(1))));
    //objects.push_back(new Rect(dvec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, dvec3(0), dvec3(1))));
    //objects.push_back(new Rect(dvec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, dvec3(0), dvec3(1))));
    //objects.push_back(new Rect(dvec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, dvec3(0), dvec3(1))));

    //objects.push_back(new Rect(dvec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, dvec3(1), dvec3(0))));
    //objects.push_back(new Rect(dvec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, dvec3(1), dvec3(0))));
    //objects.push_back(new Rect(dvec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, dvec3(1), dvec3(0))));
    //objects.push_back(new Rect(dvec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, dvec3(1), dvec3(0))));

    objects.push_back(new Rect(dvec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, dvec3(0.5), dvec3(0.5))));
    objects.push_back(new Rect(dvec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, dvec3(0.5), dvec3(0.5))));
    objects.push_back(new Rect(dvec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, dvec3(0.5), dvec3(0.5))));
    objects.push_back(new Rect(dvec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, dvec3(0.5), dvec3(0.5))));

    //objects.push_back(new Rect(dvec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, dvec3(0.1), dvec3(0.9))));
    //objects.push_back(new Rect(dvec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, dvec3(0.3), dvec3(0.6))));
    //objects.push_back(new Rect(dvec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, dvec3(0.5), dvec3(0.5))));
    //objects.push_back(new Rect(dvec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, dvec3(0.9), dvec3(0.1))));

    // Create 4 light sources
    objects.push_back(new Sphere(lightCenterPos + dvec3(-4.5, 0, 0), 0.07, new LightMaterial(dvec3(4, 2, 1))));
    objects.push_back(new Sphere(lightCenterPos + dvec3(-1.5, 0, 0), 0.16, new LightMaterial(dvec3(2, 4, 1))));
    objects.push_back(new Sphere(lightCenterPos + dvec3(1.5, 0, 0), 0.4, new LightMaterial(dvec3(2, 1, 4))));
    objects.push_back(new Sphere(lightCenterPos + dvec3(4.5, 0, 0), 1, new LightMaterial(dvec3(4, 1, 2))));

    // Set the camera
    camera.set(eyePos, dvec3(0, 0, 0), dvec3(0, 1, 0), 35.0 * M_PI / 180.0);
}

void Scene::buildHw2Test(const char* hdrFilename)
{
    dvec3 eyePos(0, 6, 18);         // camera center
    dvec3 lightCenterPos(0, 4, -6); // first light source

    objects.push_back(new Rect(dvec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, dvec3(0.5), dvec3(0.5))));
    objects.push_back(new Rect(dvec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, dvec3(0.5), dvec3(0.5))));
    objects.push_back(new Rect(dvec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, dvec3(0.5), dvec3(0.5))));
    objects.push_back(new Rect(dvec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, dvec3(0.5), dvec3(0.5))));

    //objects.push_back(new Sphere(lightCenterPos + dvec3(0, -3, 5), 2, new TableMaterial(5000, dvec3(0.5), dvec3(0.5)), false));

    objects.push_back(new EnvMap(hdrFilename));

    camera.set(eyePos, dvec3(0, 0, 0), dvec3(0, 1, 0), 35.0 * M_PI / 180.0);
}

void Scene::buildHw4Test(const char* hdrFilename)
{
    dvec3 eyePos(-12, -12, 10);         // camera center

    objects.push_back(new Rect(16, new TableMaterial(500, dvec3(0.5), dvec3(0.5))));

    objects.push_back(new Sphere(dvec3(-0.5, -4, 2), 2, new TableMaterial(5000, dvec3(0.05), dvec3(0.9)), false));
    objects.push_back(new Sphere(dvec3(0, 0, 2), 2, new TableMaterial(5000, dvec3(0, 0, 0.75), dvec3(0.2)), false));
    objects.push_back(new Sphere(dvec3(-4, -0.5, 2), 2, new TableMaterial(5000, dvec3(0.9, 0, 0), dvec3(0.05)), false));
    objects.push_back(new Sphere(dvec3(-1, -1, 5.5), 2, new TableMaterial(5000, dvec3(0.5), dvec3(0.5)), false));

    objects.push_back(new Sphere(dvec3(-2, -2, 8), 1, new LightMaterial(dvec3(4, 1, 2))));
    objects.push_back(new Sphere(dvec3(-5, 0, 5), 0.4, new LightMaterial(dvec3(2, 1, 4))));

    objects.push_back(new EnvMap(hdrFilename));

    camera.set(eyePos, dvec3(0, 0, 0), dvec3(0, 0, 1), 35.0 * M_PI / 180.0);
}

void Scene::build(const char* hdrFilename)
{
    for (Intersectable * obj : objects) {
        delete obj;
    }
    objects.clear();
    // Create a simple scene

    buildHw1_3Test();
    //buildHw2Test(hdrFilename);
    //buildHw4Test(hdrFilename);

    totalPower = 0;
    for (int i = 0; i < objects.size(); i++)
    {
        totalPower += objects[i]->power; //  hit.t < 0 if no intersection
    }
}

void Scene::render()
{
    #ifdef ENABLE_MULTITHREADING
    g_randomGenerators.clear();
    for (int i = 0; i < omp_get_max_threads(); ++i)
        g_randomGenerators.push_back(RandGen(i + 1));
    #else
    srand(1);
    #endif

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    for (int y = 0; y < Globals::screenSize.y; y++)
    {
        printf("%d\r", y);
        for (int x = 0; x < Globals::screenSize.x; x++)
        {
            nLightSamples = (int)(Globals::weight * Globals::nTotalSamples + 0.5);
            nBRDFSamples = Globals::nTotalSamples - nLightSamples;

            // For a primary ray at pixel (x,y) compute the color
            dvec3 color = dvec3(0);
            if (Globals::method == PATH_TRACING) {
                color = pathTrace(camera.getRay(x, y));
            } else {
                color = trace(camera.getRay(x, y));
            }
            Globals::hdrImage(x, y) = color;

            // map HDR to LDR
            vec3 hdrColor = Globals::hdrImage(x, y);
            vec3 mapped = vec3(1.0) - glm::exp(-hdrColor * Globals::exposure);
            // gamma correction 
            mapped = glm::pow(mapped, vec3(1.0 / Globals::gamma));
            Globals::ldrImage(x, y) = mapped;
        }
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
} // render

void Scene::renderIteration()
{
    bool computeLightSamples = Globals::weight > 0;
    bool computeBRDFSamples = Globals::weight < 1;
    for (int y = 0; y < Globals::screenSize.y; y++)
    {
        printf("%d\r", y);
        for (int x = 0; x < Globals::screenSize.x; x++)
        {
            Ray ray = camera.getRay(x, y);
            Hit hit = firstIntersect(ray, NULL); // find visible point
            
            int sampleMultiplier = 1;
            dvec3 radiance = dvec3(0);
            if (hit.t >= 0) {
                for (int i = 0; i < Globals::samplesPerFrame; i++)
                {
                    // The energy emanated from the material
                    dvec3 radianceEmitted = hit.material->getLe(ray.dir);
                    if (average(hit.material->diffuseAlbedo) < Globals::epsilon && average(hit.material->specularAlbedo) < Globals::epsilon) {
                        radiance += radianceEmitted; // if albedo is low, no energy can be reefleted
                    }
                    else if (Globals::method == PATH_TRACING) {
                        radiance += pathTraceSample(ray, hit);
                    } else {
                        if (computeLightSamples && computeBRDFSamples) {
                            sampleMultiplier = 2;
                        }
                        if (computeLightSamples) {
                            radiance += traceLightSample(ray, hit);
                        }
                        if (computeBRDFSamples) {
                            radiance += traceBRDFSample(ray, hit);
                        }
                    }
                }
            }
            Globals::radianceAccumulator(x, y) += radiance;
            int numSamples = Globals::currentNumSamples * sampleMultiplier;
            Globals::hdrImage(x, y) = Globals::radianceAccumulator(x, y) / (double)numSamples;

            // map HDR to LDR
            vec3 hdrColor = Globals::hdrImage(x, y);
            vec3 mapped = vec3(1.0) - glm::exp(-hdrColor * Globals::exposure);
            // gamma correction 
            mapped = glm::pow(mapped, vec3(1.0 / Globals::gamma));
            Globals::ldrImage(x, y) = mapped;
        }
    }
    ++Globals::currentNumSamples;
}

Hit Scene::firstIntersect(const Ray &ray, Intersectable *skip)
{
    Hit bestHit;
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i] == skip)
        continue;
        Hit hit = objects[i]->intersect(ray); //  hit.t < 0 if no intersection
        if (hit.t > Globals::epsilon)
        {
        if (bestHit.t < 0 || hit.t < bestHit.t)
            bestHit = hit;
        }
    }
    return bestHit;
}

LightSource Scene::sampleLightSource(const dvec3 &illuminatedPoint) // the 3D point on an object
{
    while (true)
    { // if no light source is selected due to floating point inaccuracies, repeat
        double threshold = totalPower * Globals::drandom();
        double running = 0;
        for (int i = 0; i < objects.size(); i++)
        {
        running += objects[i]->power; // select light source with the probability proportional to its power
        if (running > threshold)
        {
            Sphere *sphere = (Sphere *)objects[i];
            dvec3 point, normal;
            // select a point on the visible half of the light source
            ((Sphere *)objects[i])->samplePoint(illuminatedPoint, point, normal);
            return LightSource(sphere, point, normal);
        } // if
        }   // for i
    }     // for ever
}

dvec3 Scene::pathTrace(const Ray &primaryRay)
{
    Hit primaryHit = firstIntersect(primaryRay, nullptr);
    if (primaryHit.t < 0)
        return dvec3(0);

    if (average(primaryHit.material->diffuseAlbedo) < Globals::epsilon &&
        average(primaryHit.material->specularAlbedo) < Globals::epsilon) {
        return primaryHit.material->getLe(primaryRay.dir);
    }

    dvec3 radiance(0, 0, 0);

    #ifdef ENABLE_MULTITHREADING
    size_t threadCount = omp_get_max_threads();
    int localSamples = (int)ceil((double)Globals::nTotalSamples / threadCount);
    Globals::nTotalSamples = localSamples * threadCount;
    #else
    int localSamples = Globals::nTotalSamples;
    #endif

    #ifdef ENABLE_MULTITHREADING
    #pragma omp parallel num_threads(threadCount)
    {
    #endif

    dvec3 localRadiance(0, 0, 0);

    for (int i = 0; i < localSamples; i++)
    {
        dvec3 radianceTraced = pathTraceSample(primaryRay, primaryHit);
        localRadiance += radianceTraced / (double)Globals::nTotalSamples;
    }
    #ifdef ENABLE_MULTITHREADING
    #pragma omp critical
    radiance += localRadiance;
    }
    #else
    radiance += localRadiance;
    #endif

    return radiance;
}

dvec3 Scene::pathTraceSample(const Ray &primaryRay, const Hit& primaryHit)
{
    dvec3 radianceTraced(0, 0, 0);
    double factor = 1;
    Ray r = primaryRay;
    Intersectable* ignore = nullptr;
    while(true)
    {
        Hit hit = firstIntersect(r, ignore);
        if (hit.t < 0)
            break;

        dvec3 radianceEmitted = hit.material->getLe(r.dir);
        radianceTraced += factor * radianceEmitted;
        if (average(hit.material->diffuseAlbedo) < Globals::epsilon &&
            average(hit.material->specularAlbedo) < Globals::epsilon) {
            break;
        }
        dvec3 inDir = r.dir * (-1.0); // incident direction

        LightSource lightSample = sampleLightSource(hit.position); // generate a light sample
        dvec3 outDir = lightSample.point - hit.position;            // compute direction towards sample
        double distance2 = dot(outDir, outDir);
        double distance = sqrt(distance2);
        if (distance >= Globals::epsilon)
        {
            outDir = outDir / distance; // normalize the direction
            double cosThetaLight = dot(lightSample.normal, outDir * (-1.0));
            if (cosThetaLight > Globals::epsilon)
            {
            // visibility is not needed to handle, all lights are visible
            double pdfLightSourceSampling =
                lightSample.sphere->pointSampleProb(totalPower, outDir) * distance2 / cosThetaLight;
            // the theta angle on the surface between normal and light direction
            double cosThetaSurface = dot(hit.normal, outDir);
            if (cosThetaSurface > 0)
            {
                // yes, the light is visible and contributes to the output power
                // The evaluation of rendering equation locally: (light power) * brdf * cos(theta)
                dvec3 f = lightSample.sphere->material->getLe(outDir) *
                        hit.material->BRDF(hit.normal, inDir, outDir) * cosThetaSurface;
                double p = pdfLightSourceSampling;
                // importance sample = 1/n . \sum (f/prob)
                radianceTraced += 0.5 * factor * f / p;
            } // if
            }
        }

        if (!hit.material->sampleDirection(hit.normal, inDir, outDir))
            break;
        
        double cosThetaSurface = dot(hit.normal, outDir);
        if (cosThetaSurface <= 0)
            break;

        double p = std::min(0.9, average(hit.material->specularAlbedo));
        double e = Globals::drandom();
        if (e >= p)
            break;

        r = Ray(hit.position, outDir);
        ignore = hit.object;
        factor *= 0.5;
    }
    return radianceTraced;
}

dvec3 Scene::trace(const Ray &r)
{
    // error measures for the two combined techniques: used for adaptation
    Hit hit = firstIntersect(r, NULL); // find visible point
    if (hit.t < 0)
        return dvec3(0, 0, 0);
    // The energy emanated from the material
    dvec3 radianceEmitted = hit.material->getLe(r.dir);
    if (average(hit.material->diffuseAlbedo) < Globals::epsilon &&
        average(hit.material->specularAlbedo) < Globals::epsilon)
        return radianceEmitted; // if albedo is low, no energy can be reefleted
    // Compute the contribution of reflected lgiht
    dvec3 radianceBRDFSampling(0, 0, 0), radianceLightSourceSampling(0, 0, 0);

    int localLighSamples = nLightSamples;
    int localBRDFSamples = nBRDFSamples;

    #ifdef ENABLE_MULTITHREADING
    size_t threadCount = omp_get_max_threads();
    localLighSamples = (int)ceil((double)localLighSamples / threadCount);
    localBRDFSamples = (int)ceil((double)localBRDFSamples / threadCount);
    int nTotalSamples = (localLighSamples + localBRDFSamples) * threadCount;
    #else
    int nTotalSamples = (localLighSamples + localBRDFSamples);
    #endif
    double alpha = (double)localLighSamples / nTotalSamples;

    if (localLighSamples > 0)
    {
        #ifdef ENABLE_MULTITHREADING
        #pragma omp parallel num_threads(threadCount)
        {
        #endif

        dvec3 localRadianceLightSourceSampling(0);
        for (int i = 0; i < localLighSamples; i++)
        {
            localRadianceLightSourceSampling += traceLightSample(r, hit) / (double)Globals::nTotalSamples;
        } // for all the samples from light
        #ifdef ENABLE_MULTITHREADING
        #pragma omp critical
        radianceLightSourceSampling += localRadianceLightSourceSampling;
        }
        #else
        radianceLightSourceSampling += localRadianceLightSourceSampling;
        #endif
    }

    if (localBRDFSamples > 0)
    {
        #ifdef ENABLE_MULTITHREADING
        #pragma omp parallel num_threads(threadCount)
        {
        #endif

        dvec3 localRadianceBRDFSampling(0);
        for (int i = 0; i < localBRDFSamples; i++)
        {
            localRadianceBRDFSampling += traceBRDFSample(r, hit) / (double)Globals::nTotalSamples;
        } // for i
        #ifdef ENABLE_MULTITHREADING
        #pragma omp critical
        radianceBRDFSampling += localRadianceBRDFSampling;
        }
        #else
        radianceBRDFSampling += localRadianceBRDFSampling;
        #endif
    }

    return radianceEmitted + radianceLightSourceSampling + radianceBRDFSampling;
}


dvec3 Scene::traceLightSample(const Ray &r, const Hit& hit)
{
    dvec3 radiance = dvec3(0);
    dvec3 inDir = r.dir * (-1.0); // incident direction
    LightSource lightSample = sampleLightSource(hit.position); // generate a light sample
    dvec3 outDir = lightSample.point - hit.position;            // compute direction towards sample
    double distance2 = dot(outDir, outDir);
    double distance = sqrt(distance2);
    if (distance >= Globals::epsilon)
    {
        outDir = outDir / distance; // normalize the direction
        double cosThetaLight = dot(lightSample.normal, outDir * (-1.0));
        if (cosThetaLight > Globals::epsilon)
        {
            // visibility is not needed to handle, all lights are visible
            double pdfLightSourceSampling = lightSample.sphere->pointSampleProb(totalPower, outDir) * distance2 / cosThetaLight;
            double pdfBRDFSampling = hit.material->sampleProb(hit.normal, inDir, outDir);
            // the theta angle on the surface between normal and light direction
            double cosThetaSurface = dot(hit.normal, outDir);
            if (cosThetaSurface > 0)
            {
                // yes, the light is visible and contributes to the output power
                // The evaluation of rendering equation locally: (light power) * brdf * cos(theta)
                dvec3 f = lightSample.sphere->material->getLe(outDir) * hit.material->BRDF(hit.normal, inDir, outDir) * cosThetaSurface;
                double p = pdfLightSourceSampling;
                if (Globals::method == MULTIPLE_IMPORTANCE) {
                    p += pdfBRDFSampling;
                }
                // importance sample = 1/n . \sum (f/prob)
                radiance = f / p;
            } // if
        }
    }
    return radiance;
}

dvec3 Scene::traceBRDFSample(const Ray &r, const Hit& hit)
{
    dvec3 radiance = dvec3(0);
    dvec3 inDir = r.dir * (-1.0); // incident direction
    // BRDF.cos(theta) sampling should be implemented first!
    dvec3 outDir;
    // BRDF sampling with Russian roulette
    if (hit.material->sampleDirection(hit.normal, inDir, outDir))
    {
        double pdfBRDFSampling = hit.material->sampleProb(hit.normal, inDir, outDir);
        double cosThetaSurface = dot(hit.normal, outDir);
        if (cosThetaSurface > 0)
        {
            dvec3 brdf = hit.material->BRDF(hit.normal, inDir, outDir);
            // Trace a ray to the scene
            Hit lightSource = firstIntersect(Ray(hit.position, outDir), hit.object);
            // Do we hit a light source
            if (lightSource.t > 0 && average(lightSource.material->getLe(outDir)) > 0)
            {
                dvec3 dirToLight = lightSource.position - hit.position;
                // squared distance between an illuminated point and light source
                double distance2 = dot(dirToLight, dirToLight);
                double cosThetaLight = dot(lightSource.normal, outDir * (-1.0));
                if (cosThetaLight > Globals::epsilon)
                {
                    double pdfLightSourceSampling = lightSource.object->pointSampleProb(totalPower, outDir) * distance2 / cosThetaLight;
                    // The evaluation of rendering equation locally: (light power) * brdf * cos(theta)
                    dvec3 f = lightSource.material->getLe(outDir) * brdf * cosThetaSurface;
                    double p = pdfBRDFSampling;
                    if (Globals::method == MULTIPLE_IMPORTANCE) {
                        p += pdfLightSourceSampling;
                    }
                    radiance = f / p;
                }
                else
                    printf("ERROR: Sphere hit from back\n");
            }
        }
    }
    return radiance;
}

void Scene::testRay(int X, int Y)
{
    nBRDFSamples = nLightSamples = 1000;
    dvec3 current = trace(camera.getRay(X, Y));
    printf("Pixel %d, %d Value = %f, %f, %f\n", X, Y, current.x, current.y, current.z);
}

}