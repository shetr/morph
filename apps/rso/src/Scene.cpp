#include "Scene.hpp"

void Scene::buildHw1_3Test()
{
    vec3 eyePos(0, 6, 18);         // camera center
    vec3 lightCenterPos(0, 4, -6); // first light source

    // Create geometry - 4 rectangles
    //objects.push_back(new Rect(vec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, vec3(0), vec3(1))));
    //objects.push_back(new Rect(vec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, vec3(0), vec3(1))));
    //objects.push_back(new Rect(vec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, vec3(0), vec3(1))));
    //objects.push_back(new Rect(vec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, vec3(0), vec3(1))));

    //objects.push_back(new Rect(vec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, vec3(1), vec3(0))));
    //objects.push_back(new Rect(vec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, vec3(1), vec3(0))));
    //objects.push_back(new Rect(vec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, vec3(1), vec3(0))));
    //objects.push_back(new Rect(vec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, vec3(1), vec3(0))));

    objects.push_back(new Rect(vec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, vec3(0.5), vec3(0.5))));
    objects.push_back(new Rect(vec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, vec3(0.5), vec3(0.5))));
    objects.push_back(new Rect(vec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, vec3(0.5), vec3(0.5))));
    objects.push_back(new Rect(vec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, vec3(0.5), vec3(0.5))));

    //objects.push_back(new Rect(vec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, vec3(0.1), vec3(0.9))));
    //objects.push_back(new Rect(vec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, vec3(0.3), vec3(0.6))));
    //objects.push_back(new Rect(vec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, vec3(0.5), vec3(0.5))));
    //objects.push_back(new Rect(vec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, vec3(0.9), vec3(0.1))));

    // Create 4 light sources
    objects.push_back(new Sphere(lightCenterPos + vec3(-4.5, 0, 0), 0.07, new LightMaterial(vec3(4, 2, 1))));
    objects.push_back(new Sphere(lightCenterPos + vec3(-1.5, 0, 0), 0.16, new LightMaterial(vec3(2, 4, 1))));
    objects.push_back(new Sphere(lightCenterPos + vec3(1.5, 0, 0), 0.4, new LightMaterial(vec3(2, 1, 4))));
    objects.push_back(new Sphere(lightCenterPos + vec3(4.5, 0, 0), 1, new LightMaterial(vec3(4, 1, 2))));

    // Set the camera
    camera.set(eyePos, vec3(0, 0, 0), vec3(0, 1, 0), 35.0 * M_PI / 180.0);
}

void Scene::buildHw2Test(const char* hdrFilename)
{
    vec3 eyePos(0, 6, 18);         // camera center
    vec3 lightCenterPos(0, 4, -6); // first light source

    objects.push_back(new Rect(vec3(0, -4, +2), eyePos, lightCenterPos, 8, 1, new TableMaterial(500, vec3(0.5), vec3(0.5))));
    objects.push_back(new Rect(vec3(0, -3.5, -2), eyePos, lightCenterPos, 8, 1, new TableMaterial(1000, vec3(0.5), vec3(0.5))));
    objects.push_back(new Rect(vec3(0, -2.5, -6), eyePos, lightCenterPos, 8, 1, new TableMaterial(5000, vec3(0.5), vec3(0.5))));
    objects.push_back(new Rect(vec3(0, -1, -10), eyePos, lightCenterPos, 8, 1, new TableMaterial(10000, vec3(0.5), vec3(0.5))));

    //objects.push_back(new Sphere(lightCenterPos + vec3(0, -3, 5), 2, new TableMaterial(5000, vec3(0.5), vec3(0.5)), false));

    objects.push_back(new EnvMap(hdrFilename));

    camera.set(eyePos, vec3(0, 0, 0), vec3(0, 1, 0), 35.0 * M_PI / 180.0);
}

void Scene::buildHw4Test(const char* hdrFilename)
{
    vec3 eyePos(-12, -12, 10);         // camera center

    objects.push_back(new Rect(16, new TableMaterial(500, vec3(0.5), vec3(0.5))));

    objects.push_back(new Sphere(vec3(-0.5, -4, 2), 2, new TableMaterial(5000, vec3(0.05), vec3(0.9)), false));
    objects.push_back(new Sphere(vec3(0, 0, 2), 2, new TableMaterial(5000, vec3(0, 0, 0.75), vec3(0.2)), false));
    objects.push_back(new Sphere(vec3(-4, -0.5, 2), 2, new TableMaterial(5000, vec3(0.9, 0, 0), vec3(0.05)), false));
    objects.push_back(new Sphere(vec3(-1, -1, 5.5), 2, new TableMaterial(5000, vec3(0.5), vec3(0.5)), false));

    objects.push_back(new Sphere(vec3(-2, -2, 8), 1, new LightMaterial(vec3(4, 1, 2))));
    objects.push_back(new Sphere(vec3(-5, 0, 5), 0.4, new LightMaterial(vec3(2, 1, 4))));

    objects.push_back(new EnvMap(hdrFilename));

    camera.set(eyePos, vec3(0, 0, 0), vec3(0, 0, 1), 35.0 * M_PI / 180.0);
}

void Scene::build(const char* hdrFilename)
{
    for (Intersectable * obj : objects) {
        delete obj;
    }
    objects.clear();
    // Create a simple scene

    //buildHw1_3Test();
    //buildHw2Test(hdrFilename);
    buildHw4Test(hdrFilename);

    totalPower = 0;
    for (int i = 0; i < objects.size(); i++)
    {
        totalPower += objects[i]->power; //  hit.t < 0 if no intersection
    }
}

void Scene::setWeight(double wval)
{
    for (int Y = 0; Y < screenHeight; Y++)
        for (int X = 0; X < screenWidth; X++)
        weight[Y * screenWidth + X] = wval;
}

void Scene::render()
{
    // Total number of samples per pixel is: nIterators*nTotalSamples
    #ifdef ENABLE_MULTITHREADING
    g_randomGenerators.clear();
    for (int i = 0; i < omp_get_max_threads(); ++i)
        g_randomGenerators.push_back(RandGen(i + 1));
    #else
    srand(1);
    #endif
    char buffer[100];
    FILE *errorFile = 0;

    switch (method)
    {
    case BRDF:
        nBRDFSamples = nTotalSamples;
        nLightSamples = 0;
        errorFile = fopen("BRDF.txt", "w");
        setWeight(0);
        break;
    case LIGHT_SOURCE:
        nBRDFSamples = 0;
        nLightSamples = nTotalSamples;
        errorFile = fopen("light.txt", "w");
        setWeight(1.0);
        break;
    case HALF_WEIGHT:
        errorFile = fopen("light.txt", "w");
        setWeight(0.5);
        break;
    case MULTIPLE_IMPORTANCE:
        errorFile = fopen("light.txt", "w");
        setWeight(0.5);
        break;
    case PATH_TRACING:
        errorFile = fopen("light.txt", "w");
        setWeight(1.0);
        break;
    } // switch


    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    double cost = 0;
    bool debug = false;
    // How many iterations
    for (int iIter = 1; iIter <= nIterations; iIter++)
    {
        double error = 0;
        for (int Y = 0; Y < screenHeight; Y++)
        { // for all rows
        printf("%d\r", Y);
        for (int X = 0; X < screenWidth; X++)
        { // for all pixels in a row
            if (debug)
            { // debug particular pixel x,y, coordinates from pfsv (pfstools)
            X = 441;
            Y = screenHeight - 529;
            }

            nLightSamples = (int)(weight[Y * screenWidth + X] * nTotalSamples + 0.5);
            nBRDFSamples = nTotalSamples - nLightSamples;
            cost += nBRDFSamples * costBRDF + nLightSamples * costLight;

            // For a primary ray at pixel (X,Y) compute the color
            vec3 color;
            if (method == PATH_TRACING) {
            color = pathTrace(camera.getRay(X, Y));
            } else {
            color = trace(camera.getRay(X, Y));
            }
            double w = 1.0 / iIter; // the same weight for all samples for computing mean incrementally
            image[Y * screenWidth + X] = color * w + image[Y * screenWidth + X] * (1.0 - w);

            w = 1.0 / sqrt(iIter); // emphasize later samples
            vec3 diff = reference[Y * screenWidth + X] - image[Y * screenWidth + X];
            error += dot(diff, diff);
            if (debug)
            break;
        } // for X
        if (debug)
            break;
        } // for Y
        double eff = 100000.0 * nIterations * nTotalSamples * screenWidth * screenHeight / error / cost;
        printf("Iter: %d, Error: %4.2f, Efficiency: %f, Relative Efficiency: %f\n",
                iIter, sqrt(error), eff, eff / referenceEfficiency);
        fprintf(errorFile, "%d, %f\n", iIter * nTotalSamples, sqrt(error));
    } // for iTer
    fclose(errorFile);
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();

    std::cout << "Took " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
} // render

Hit Scene::firstIntersect(const Ray &ray, Intersectable *skip)
{
    Hit bestHit;
    for (int i = 0; i < objects.size(); i++)
    {
        if (objects[i] == skip)
        continue;
        Hit hit = objects[i]->intersect(ray); //  hit.t < 0 if no intersection
        if (hit.t > epsilon)
        {
        if (bestHit.t < 0 || hit.t < bestHit.t)
            bestHit = hit;
        }
    }
    return bestHit;
}

LightSource Scene::sampleLightSource(const vec3 &illuminatedPoint) // the 3D point on an object
{
    while (true)
    { // if no light source is selected due to floating point inaccuracies, repeat
        double threshold = totalPower * drandom();
        double running = 0;
        for (int i = 0; i < objects.size(); i++)
        {
        running += objects[i]->power; // select light source with the probability proportional to its power
        if (running > threshold)
        {
            Sphere *sphere = (Sphere *)objects[i];
            vec3 point, normal;
            // select a point on the visible half of the light source
            ((Sphere *)objects[i])->samplePoint(illuminatedPoint, point, normal);
            return LightSource(sphere, point, normal);
        } // if
        }   // for i
    }     // for ever
}

vec3 Scene::pathTrace(const Ray &primaryRay)
{
    Hit primaryHit = firstIntersect(primaryRay, nullptr);
    if (primaryHit.t < 0)
        return vec3(0);

    if (primaryHit.material->diffuseAlbedo.average() < epsilon &&
        primaryHit.material->specularAlbedo.average() < epsilon) {
        return primaryHit.material->getLe(primaryRay.dir);
    }

    vec3 radiance(0, 0, 0);

    #ifdef ENABLE_MULTITHREADING
    size_t threadCount = omp_get_max_threads();
    int localSamples = (int)ceil((double)nTotalSamples / threadCount);
    nTotalSamples = localSamples * threadCount;
    #else
    int localSamples = nTotalSamples;
    #endif

    #ifdef ENABLE_MULTITHREADING
    #pragma omp parallel num_threads(threadCount)
    {
    #endif

    vec3 localRadiance(0, 0, 0);

    for (int i = 0; i < localSamples; i++)
    {
        vec3 radianceTraced(0, 0, 0);
        double factor = 1;
        Ray r = primaryRay;
        Intersectable* ignore = nullptr;
        while(true)
        {
        Hit hit = firstIntersect(r, ignore);
        if (hit.t < 0)
            break;

        vec3 radianceEmitted = hit.material->getLe(r.dir);
        radianceTraced += factor * radianceEmitted;
        if (hit.material->diffuseAlbedo.average() < epsilon &&
            hit.material->specularAlbedo.average() < epsilon) {
            break;
        }
        vec3 inDir = r.dir * (-1); // incident direction

        LightSource lightSample = sampleLightSource(hit.position); // generate a light sample
        vec3 outDir = lightSample.point - hit.position;            // compute direction towards sample
        double distance2 = dot(outDir, outDir);
        double distance = sqrt(distance2);
        if (distance >= epsilon)
        {
            outDir = outDir / distance; // normalize the direction
            double cosThetaLight = dot(lightSample.normal, outDir * (-1));
            if (cosThetaLight > epsilon)
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
                vec3 f = lightSample.sphere->material->getLe(outDir) *
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

        double p = std::min(0.9, hit.material->specularAlbedo.average());
        double e = drandom();
        if (e >= p)
            break;

        r = Ray(hit.position, outDir);
        ignore = hit.object;
        factor *= 0.5;
        }
        localRadiance += radianceTraced / nTotalSamples;
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

vec3 Scene::trace(const Ray &r)
{
    // error measures for the two combined techniques: used for adaptation
    Hit hit = firstIntersect(r, NULL); // find visible point
    if (hit.t < 0)
        return vec3(0, 0, 0);
    // The energy emanated from the material
    vec3 radianceEmitted = hit.material->getLe(r.dir);
    if (hit.material->diffuseAlbedo.average() < epsilon &&
        hit.material->specularAlbedo.average() < epsilon)
        return radianceEmitted; // if albedo is low, no energy can be reefleted
    // Compute the contribution of reflected lgiht
    vec3 radianceBRDFSampling(0, 0, 0), radianceLightSourceSampling(0, 0, 0);
    vec3 inDir = r.dir * (-1); // incident direction

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

        vec3 localRadianceLightSourceSampling(0);
        for (int i = 0; i < localLighSamples; i++)
        {
        LightSource lightSample = sampleLightSource(hit.position); // generate a light sample
        vec3 outDir = lightSample.point - hit.position;            // compute direction towards sample
        double distance2 = dot(outDir, outDir);
        double distance = sqrt(distance2);
        if (distance >= epsilon)
        {
            outDir = outDir / distance; // normalize the direction
            double cosThetaLight = dot(lightSample.normal, outDir * (-1));
            if (cosThetaLight > epsilon)
            {
            // visibility is not needed to handle, all lights are visible
            double pdfLightSourceSampling =
                lightSample.sphere->pointSampleProb(totalPower, outDir) * distance2 / cosThetaLight;
            double pdfBRDFSampling = hit.material->sampleProb(hit.normal, inDir, outDir);
            // the theta angle on the surface between normal and light direction
            double cosThetaSurface = dot(hit.normal, outDir);
            if (cosThetaSurface > 0)
            {
                // yes, the light is visible and contributes to the output power
                // The evaluation of rendering equation locally: (light power) * brdf * cos(theta)
                vec3 f = lightSample.sphere->material->getLe(outDir) *
                        hit.material->BRDF(hit.normal, inDir, outDir) * cosThetaSurface;
                double p = pdfLightSourceSampling;
                if (method == MULTIPLE_IMPORTANCE) {
                p += pdfBRDFSampling;
                }
                // importance sample = 1/n . \sum (f/prob)
                localRadianceLightSourceSampling += f / p / nTotalSamples;
            } // if
            }
        }
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

        vec3 localRadianceBRDFSampling(0);
        for (int i = 0; i < localBRDFSamples; i++)
        {
        // BRDF.cos(theta) sampling should be implemented first!
        vec3 outDir;
        // BRDF sampling with Russian roulette
        if (hit.material->sampleDirection(hit.normal, inDir, outDir))
        {
            double pdfBRDFSampling = hit.material->sampleProb(hit.normal, inDir, outDir);
            double cosThetaSurface = dot(hit.normal, outDir);
            if (cosThetaSurface > 0)
            {
            vec3 brdf = hit.material->BRDF(hit.normal, inDir, outDir);
            // Trace a ray to the scene
            Hit lightSource = firstIntersect(Ray(hit.position, outDir), hit.object);
            // Do we hit a light source
            if (lightSource.t > 0 && lightSource.material->getLe(outDir).average() > 0)
            {
                vec3 dirToLight = lightSource.position - hit.position;
                // squared distance between an illuminated point and light source
                double distance2 = dot(dirToLight, dirToLight);
                double cosThetaLight = dot(lightSource.normal, outDir * (-1));
                if (cosThetaLight > epsilon)
                {
                double pdfLightSourceSampling =
                    lightSource.object->pointSampleProb(totalPower, outDir) * distance2 / cosThetaLight;
                // The evaluation of rendering equation locally: (light power) * brdf * cos(theta)
                vec3 f = lightSource.material->getLe(outDir) * brdf * cosThetaSurface;
                double p = pdfBRDFSampling;
                if (method == MULTIPLE_IMPORTANCE) {
                    p += pdfLightSourceSampling;
                }
                localRadianceBRDFSampling += f / p / nTotalSamples;
                }
                else
                printf("ERROR: Sphere hit from back\n");
            }
            }
        }
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

void Scene::testRay(int X, int Y)
{
    nBRDFSamples = nLightSamples = 1000;
    vec3 current = trace(camera.getRay(X, Y));
    printf("Pixel %d, %d Value = %f, %f, %f\n", X, Y, current.x, current.y, current.z);
}