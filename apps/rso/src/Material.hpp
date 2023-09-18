#pragma once

#include <Morph.hpp>
#include "Image.hpp"

using namespace glm;

// The definition of material surface (BRDF + emission)
struct Material
{
  vec3 Le;             // the emmited power
  vec3 diffuseAlbedo;  // albedo for diffuse component
  vec3 specularAlbedo; // albedo for specular component
  double shininess;

  Material() { shininess = 0; }

  // Evaluate the BRDF given normal, view direction (outgoing) and light direction (incoming)
  vec3 BRDF(const vec3 &N, const vec3 &V, const vec3 &L);
  // BRDF.cos(theta) importance sampling for input normal, outgoing direction
  bool sampleDirection(const vec3 &N, const vec3 &V, vec3 &L);
  // Evaluate the probability given input normal, view (outgoing) direction and incoming light direction
  double sampleProb(const vec3 &N, const vec3 &V, const vec3 &L);

  virtual vec3 getLe(vec3 dir) const { return Le; }
};

// Material used for light source
struct LightMaterial : Material
{
  LightMaterial(vec3 _Le) { Le = _Le; }
};

// Material used for objects, given how much is reflective/shiny
struct TableMaterial : Material
{
  TableMaterial(double shine, const vec3& diff, const vec3& spec)
  {
    shininess = shine;
    diffuseAlbedo = diff;
    specularAlbedo = spec;
  }
};

struct EnvMapMaterial : public Material
{
  Image hdrImage;

  EnvMapMaterial(const char* hdrFilename);

  vec3 getLe(vec3 dir) const override;

  vec3& getLeRef(vec3 dir);
private:
  vec3 sample(int x, int y) const;

  vec3 sampleBilinear(double x, double y) const;
};