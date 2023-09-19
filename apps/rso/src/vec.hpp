#ifndef RSO_VEC_HPP
#define RSO_VEC_HPP

#include <stdlib.h>
#include <math.h>

struct dvec3;

// Vector 3D
struct vec3
{
  float x, y, z;

  vec3() { x = y = z = 0; }
  vec3(float v)
  {
    x = v;
    y = v;
    z = v;
  }
  vec3(float x0, float y0, float z0 = 0)
  {
    x = x0;
    y = y0;
    z = z0;
  }
  vec3(dvec3 v);
  vec3 operator-() const { return vec3(-x, -y, -z); }
  vec3 operator*(float a) const { return vec3(x * a, y * a, z * a); }
  friend vec3 operator*(float a, const vec3 v) { return vec3(v.x * a, v.y * a, v.z * a); }
  vec3 operator*(const vec3 r) const { return vec3(x * r.x, y * r.y, z * r.z); }
  vec3 operator/(const float r) const
  {
    if (fabs(r) > 0.00000001)
      return vec3(x / r, y / r, z / r);
    else
      return vec3(0, 0, 0);
  }
  vec3 operator+(const vec3 &v) const { return vec3(x + v.x, y + v.y, z + v.z); }
  vec3 operator-(const vec3 &v) const { return vec3(x - v.x, y - v.y, z - v.z); }
  void operator+=(const vec3 &v) { x += v.x, y += v.y, z += v.z; }
  void operator*=(float a) { x *= a, y *= a, z *= a; }
};
float length(vec3 v);

vec3 normalize(vec3 v);

// dot product of two vectors
float dot(const vec3 &v1, const vec3 &v2);

// cross product of two vectors
vec3 cross(const vec3 &v1, const vec3 &v2);

vec3 exp(vec3 v);
vec3 pow(vec3 v, vec3 a);

struct dvec3
{
  double x, y, z;

  dvec3() { x = y = z = 0; }
  dvec3(double v)
  {
    x = v;
    y = v;
    z = v;
  }
  dvec3(double x0, double y0, double z0 = 0)
  {
    x = x0;
    y = y0;
    z = z0;
  }
  dvec3(vec3 v)
  {
    x = v.x;
    y = v.y;
    z = v.z;
  }
  dvec3 operator-() const { return dvec3(-x, -y, -z); }
  dvec3 operator*(double a) const { return dvec3(x * a, y * a, z * a); }
  friend dvec3 operator*(double a, const dvec3 v) { return dvec3(v.x * a, v.y * a, v.z * a); }
  dvec3 operator*(const dvec3 r) const { return dvec3(x * r.x, y * r.y, z * r.z); }
  dvec3 operator/(const double r) const
  {
    if (fabs(r) > 0.00000001)
      return dvec3(x / r, y / r, z / r);
    else
      return dvec3(0, 0, 0);
  }
  dvec3 operator+(const dvec3 &v) const { return dvec3(x + v.x, y + v.y, z + v.z); }
  dvec3 operator-(const dvec3 &v) const { return dvec3(x - v.x, y - v.y, z - v.z); }
  void operator+=(const dvec3 &v) { x += v.x, y += v.y, z += v.z; }
  void operator*=(double a) { x *= a, y *= a, z *= a; }
};

double length(dvec3 v);

dvec3 normalize(dvec3 v);

// dot product of two vectors
double dot(const dvec3 &v1, const dvec3 &v2);

// cross product of two vectors
dvec3 cross(const dvec3 &v1, const dvec3 &v2);

template<typename T>
T clamp(T x, T min, T max)
{
  return std::max(min, std::min(max, x));
}

#endif // RSO_VEC_HPP