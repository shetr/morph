#ifndef RSO_MATERIAL_HPP
#define RSO_MATERIAL_HPP

#include "Image.hpp"

namespace Morph {

// The definition of material surface (BRDF + emission)
struct Material
{
  dvec3 Le = dvec3(0);             // the emmited power
  dvec3 diffuseAlbedo = dvec3(0);  // albedo for diffuse component
  dvec3 specularAlbedo = dvec3(0); // albedo for specular component
  double shininess = 0;

  Material() { shininess = 0; }

  // Evaluate the BRDF given normal, view direction (outgoing) and light direction (incoming)
  dvec3 BRDF(const dvec3 &N, const dvec3 &V, const dvec3 &L);
  // BRDF.cos(theta) importance sampling for input normal, outgoing direction
  bool sampleDirection(const dvec3 &N, const dvec3 &V, dvec3 &L, int workerId);
  // Evaluate the probability given input normal, view (outgoing) direction and incoming light direction
  double sampleProb(const dvec3 &N, const dvec3 &V, const dvec3 &L);

  virtual dvec3 getLe(dvec3 dir) const { return Le; }
};

// Material used for light source
struct LightMaterial : Material
{
  LightMaterial(dvec3 _Le) { Le = _Le; }
};

// Material used for objects, given how much is reflective/shiny
struct TableMaterial : Material
{
  TableMaterial(double shine, const dvec3& diff, const dvec3& spec)
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

  dvec3 getLe(dvec3 dir) const override;

  dvec3& getLeRef(dvec3 dir);
private:
  dvec3 sample(int x, int y) const;

  dvec3 sampleBilinear(double x, double y) const;
};

}

#endif // RSO_MATERIAL_HPP