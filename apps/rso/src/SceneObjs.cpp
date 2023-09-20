#include "SceneObjs.hpp"

#define _USE_MATH_DEFINES
#include <math.h>

namespace Morph {

Rect::Rect(dvec3 _r0, dvec3 _r1, dvec3 _r2, double _width, double _height, Material *mat)
{
    r0 = _r0;
    dvec3 L = _r1 - r0;
    dvec3 V = _r2 - r0;
    // compute normal
    normal = normalize(normalize(L) + normalize(V));
    material = mat;
    power = 0; // default - does not emit light
    width = _width;
    height = _height;
    // recompute directions to get rectangle
    right = normalize(cross(dvec3(0, 0, 1), normal));
    forward = normalize(cross(normal, right));
}

Rect::Rect(double _size, Material *mat)
{
    r0 = dvec3(0, 0, 0);
    // compute normal
    normal = dvec3(0, 0, 1);
    material = mat;
    power = 0; // default - does not emit light
    width = _size / 2;
    height = _size / 2;
    // recompute directions to get rectangle
    right = dvec3(1, 0, 0);
    forward = dvec3(0, 1, 0);
}

Hit Rect::intersect(const Ray &ray)
{
    Hit hit;
    double denom = dot(normal, ray.dir);
    if (fabs(denom) > Globals::epsilon)
    {
        hit.t = dot(normal, r0 - ray.start) / denom;
        if (hit.t < 0)
        return hit;
        hit.position = ray.start + ray.dir * hit.t;
        double x = dot(hit.position - r0, right);
        double y = dot(hit.position - r0, forward);
        if (fabs(x) > width || fabs(y) > height)
        {
        hit.t = -1;
        return hit;
        }
        hit.normal = normal;
        hit.position = ray.start + ray.dir * hit.t;
        hit.material = material;
        hit.object = this;
    }
    return hit;
}

Sphere::Sphere(const dvec3 &cent, double rad, Material *mat, bool emit, const double targetPower)
{
    center = cent;
    radius = rad;
    material = mat;
    if (emit)
    {
        power = average(material->Le) * (4 * radius * radius * M_PI) * M_PI;
        material->Le = material->Le * (targetPower / power);
        power = targetPower;
    }
}

Hit Sphere::intersect(const Ray &r)
{
    Hit hit;
    dvec3 dist = r.start - center;
    double b = dot(dist, r.dir) * 2.0;
    double a = dot(r.dir, r.dir);
    double c = dot(dist, dist) - radius * radius;
    double discr = b * b - 4.0 * a * c;
    if (discr < 0)
        return hit;
    double sqrt_discr = sqrt(discr);
    double t1 = (-b + sqrt_discr) / 2.0 / a;
    double t2 = (-b - sqrt_discr) / 2.0 / a;
    if (t1 <= 0 && t2 <= 0)
        return hit;
    if (t1 <= 0 && t2 > 0)
        hit.t = t2;
    else if (t2 <= 0 && t1 > 0)
        hit.t = t1;
    else if (t1 < t2)
        hit.t = t1;
    else
        hit.t = t2;
    hit.position = r.start + r.dir * hit.t;
    hit.normal = (hit.position - center) / radius;
    hit.material = material;
    hit.object = this;
    return hit;
}

void Sphere::samplePoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal)
{
    return sampleUniformPoint(illuminatedPoint, point, normal);
}

void Sphere::sampleUniformPoint(const dvec3 &illuminatedPoint, dvec3 &point, dvec3 &normal)
{
    normal = dvec3(2);
    while (dot(normal, normal) > 1)
    {
        // uniform in a cube of edge size 2
        normal = dvec3(Globals::drandom() * 2 - 1, Globals::drandom() * 2 - 1, Globals::drandom() * 2 - 1);
        if (dot(illuminatedPoint - center, normal) < 0)
        continue;                      // ignore surely non visible points
    } // finish if the point is in the unit sphere
    normal = normalize(normal);       // project points onto the surface of the unit sphere
    point = center + normal * radius;  // project onto the real sphere
}

double Sphere::pointSampleProb(double totalPower, dvec3 dir)
{
    return power / totalPower / (4 * radius * radius * M_PI);
}

}