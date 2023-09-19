#include "vec.hpp"


vec3::vec3(dvec3 v)
{
    x = v.x;
    y = v.y;
    z = v.z;
}

float length(vec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

vec3 normalize(vec3 v)
{
  float l = length(v);
  if (l > 0.00000001)
    return v / l;
  else
    return vec3(0, 0, 0);
}


float dot(const vec3 &v1, const vec3 &v2)
{
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

vec3 cross(const vec3 &v1, const vec3 &v2)
{
  return vec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}

vec3 exp(vec3 v) {
  return vec3(exp(v.x), exp(v.y), exp(v.z));
}
vec3 pow(vec3 v, vec3 a) {
  return vec3(pow(v.x, a.x), pow(v.y, a.y), pow(v.z, a.z));
}

double length(dvec3 v) { return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); }

dvec3 normalize(dvec3 v)
{
  double l = length(v);
  if (l > 0.00000001)
    return v / l;
  else
    return dvec3(0, 0, 0);
}

// dot product of two vectors
double dot(const dvec3 &v1, const dvec3 &v2)
{
  return (v1.x * v2.x + v1.y * v2.y + v1.z * v2.z);
}

// cross product of two vectors
dvec3 cross(const dvec3 &v1, const dvec3 &v2)
{
  return dvec3(v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x);
}