
#ifndef MORPH_PHONG_DATA_HPP
#define MORPH_PHONG_DATA_HPP

#include <Core/Core.hpp>
#include <Graphics/Uniforms.hpp>
#include <Graphics/Context.hpp>

#include "TextureUnits.hpp"

namespace Morph {

enum class PhongLightType : int
{
    NONE = 0,
    DIR = 1,
    POINT = 2,
    SPOT = 3
};

struct PhongLight
{
    PhongLightType lightType = PhongLightType::NONE;
    vec4 ambient = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 diffuse = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 position = vec4(0.0, 0.0, 1.0, 0.0);
    vec3 spotDir = vec3(0.0, 0.0, -1.0);
    float spotExponent = 0.0;
    float spotCutoff = glm::pi<float>()/2;
    float attenuationConst = 1.0;
    float attenuationLin = 0.0;
    float attenuationQuad = 0.0;
};

struct PhongMaterial
{
    vec4 ambient = vec4(0.2, 0.2, 0.2, 1.0);
    vec4 diffuse = vec4(0.8, 0.8, 0.8, 1.0);
    vec4 specular = vec4(1.0, 1.0, 1.0, 1.0);
    vec4 emission = vec4(0.0, 0.0, 0.0, 1.0);
    float shinines = 1.0;
};

struct PhongMaterialTex
{
    opt<ref<Texture2D>> ambient;
    opt<ref<Texture2D>> diffuse;
    opt<ref<Texture2D>> specular;
    opt<ref<Texture2D>> emission;
    opt<ref<Texture2D>> normal;
};

class PhongLightUniform : public UniformStruct
{
private:
    Uniform<int> m_lightType;
    Uniform<vec4> m_ambient;
    Uniform<vec4> m_diffuse;
    Uniform<vec4> m_specular;
    Uniform<vec4> m_position;
    Uniform<vec3> m_spotDir;
    Uniform<float> m_spotExponent;
    Uniform<float> m_spotCutoff;
    Uniform<float> m_attenuationConst;
    Uniform<float> m_attenuationLin;
    Uniform<float> m_attenuationQuad;
public:
    PhongLightUniform(const PhongLight& light) : PhongLightUniform(light, "u_light") {}
    PhongLightUniform(const PhongLight& light, const string& name) : UniformStruct(name), 
        m_lightType(reinterpret_cast<const int&>(static_cast<const PhongLightType&>(std::cref(light.lightType)))),
        m_ambient(light.ambient),
        m_diffuse(light.diffuse),
        m_specular(light.specular),
        m_position(light.position),
        m_spotDir(light.spotDir),
        m_spotExponent(light.spotExponent),
        m_spotCutoff(light.spotCutoff),
        m_attenuationConst(light.attenuationConst),
        m_attenuationLin(light.attenuationLin),
        m_attenuationQuad(light.attenuationQuad)
    {
        AddUniform(m_lightType, "lightType");
        AddUniform(m_ambient, "ambient");
        AddUniform(m_diffuse, "diffuse");
        AddUniform(m_specular, "specular");
        AddUniform(m_position, "position");
        AddUniform(m_spotDir, "spotDir");
        AddUniform(m_spotExponent, "spotExponent");
        AddUniform(m_spotCutoff, "spotCutoff");
        AddUniform(m_attenuationConst, "attenuationConst");
        AddUniform(m_attenuationLin, "attenuationLin");
        AddUniform(m_attenuationQuad, "attenuationQuad");
    }
};

class PhongLightsUniformArray : public IUniform
{
private:
    Uniform<uint> m_count;
    vector<PhongLightUniform> m_lights;
public:
    PhongLightsUniformArray(const uint& lightsCount, const raw<PhongLight>& lightsCache, const string& name)
        : m_count(name + "_count", lightsCount)
    {
        uint i = 0;
        for(const PhongLight& light : lightsCache) {
            m_lights.emplace_back(light, name + "[" + std::to_string(i) + "]");
            i++;
        }
    }
    void Set(const GraphicsProgramBinder& programBinder) const override {
        m_count.Set(programBinder);
        for(const PhongLightUniform& lightUniform : m_lights) {
            lightUniform.Set(programBinder);
        }
    }
};

class PhongMaterialUniform : public UniformStruct
{
private:
    Uniform<vec4> m_ambient;
    Uniform<vec4> m_diffuse;
    Uniform<vec4> m_specular;
    Uniform<vec4> m_emission;
    Uniform<float> m_shinines;
public:
    PhongMaterialUniform(const PhongMaterial& material) : PhongMaterialUniform(material, "u_material") {}
    PhongMaterialUniform(const PhongMaterial& material, const string& name) : UniformStruct(name),
        m_ambient(material.ambient),
        m_diffuse(material.diffuse),
        m_specular(material.specular),
        m_emission(material.emission),
        m_shinines(material.shinines)
    {
        AddUniform(m_ambient, "ambient");
        AddUniform(m_diffuse, "diffuse");
        AddUniform(m_specular, "specular");
        AddUniform(m_emission, "emission");
        AddUniform(m_shinines, "shinines");
    }
};

class PhongForwardUniforms : public Uniforms
{
private:
    Uniform<vec4> m_envGlobalAmbient;
    Uniform<vec3> m_cameraPosition;
    PhongMaterialUniform m_material;
    PhongLightsUniformArray m_lights;
public:
    PhongForwardUniforms(const vec4& envGlobalAmbient, const vec3& cameraPosition, const PhongMaterial& material, const uint& lightsCount, const raw<PhongLight>& lightsCache)
        :
        m_envGlobalAmbient("u_envGlobalAmbient", envGlobalAmbient),
        m_cameraPosition("u_cameraPosition", cameraPosition),
        m_material(material),
        m_lights(lightsCount, lightsCache, "u_lights")
    {
        AddUniform(m_envGlobalAmbient);
        AddUniform(m_cameraPosition);
        AddUniform(m_material);
        AddUniform(m_lights);
    }
};

class PhongForwardTextureUnits : public TextureUnits
{
public:
    PhongForwardTextureUnits(const PhongMaterialTex& materialTex, Texture2D& whiteTexture)
        : TextureUnits("u_materialTex", 5)
    {
        Add(TextureUnitArgs({materialTex.ambient.value_or(whiteTexture), "ambient", TextureUnit::_0}));
        Add(TextureUnitArgs({materialTex.diffuse.value_or(whiteTexture), "diffuse", TextureUnit::_1}));
        Add(TextureUnitArgs({materialTex.specular.value_or(whiteTexture), "specular", TextureUnit::_2}));
        Add(TextureUnitArgs({materialTex.emission.value_or(whiteTexture), "emission", TextureUnit::_3}));
        Add(TextureUnitArgs({materialTex.normal.value_or(whiteTexture), "normal", TextureUnit::_4}));
    }
};

}

#endif // MORPH_PHONG_DATA_HPP