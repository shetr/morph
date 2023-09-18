#ifndef MORPH_PBR_DATA_HPP
#define MORPH_PBR_DATA_HPP

#include <Core/Core.hpp>
#include <Graphics/Uniforms.hpp>
#include <Graphics/Context.hpp>

#include "TextureUnits.hpp"

namespace Morph {

enum class LightType : int
{
    NONE = 0,
    DIR = 1,
    POINT = 2,
    SPOT = 3
};

struct Light
{
    LightType lightType = LightType::NONE;
    vec3 color = vec3(1.0, 1.0, 1.0);
    vec3 position = vec3(0.0, 0.0, 1.0);
    vec3 spotDir = vec3(0.0, 0.0, -1.0);
    float spotExponent = 0.0;
    float spotCutoff = glm::pi<float>()/2;
    float attenuationConst = 1.0;
    float attenuationLin = 0.0;
    float attenuationQuad = 0.0;
};

struct Material
{
    vec3 albedo = vec3(1.0, 1.0, 1.0);
    float metallic = 0.0;
    float roughness = 0.0;
    float ao = 1.0;
};

struct MaterialTex
{
    opt<ref<Texture2D>> albedo;
    opt<ref<Texture2D>> metallic;
    opt<ref<Texture2D>> roughness;
    opt<ref<Texture2D>> ao;
    opt<ref<Texture2D>> normal;
};

class LightUniform : public UniformStruct
{
private:
    Uniform<int> m_lightType;
    Uniform<vec3> m_color;
    Uniform<vec3> m_position;
    Uniform<vec3> m_spotDir;
    Uniform<float> m_spotExponent;
    Uniform<float> m_spotCutoff;
    Uniform<float> m_attenuationConst;
    Uniform<float> m_attenuationLin;
    Uniform<float> m_attenuationQuad;
public:
    LightUniform(const Light& light) : LightUniform(light, "u_light") {}
    LightUniform(const Light& light, const string& name) : UniformStruct(name), 
        m_lightType(reinterpret_cast<const int&>(static_cast<const LightType&>(std::cref(light.lightType)))),
        m_color(light.color),
        m_position(light.position),
        m_spotDir(light.spotDir),
        m_spotExponent(light.spotExponent),
        m_spotCutoff(light.spotCutoff),
        m_attenuationConst(light.attenuationConst),
        m_attenuationLin(light.attenuationLin),
        m_attenuationQuad(light.attenuationQuad)
    {
        AddUniform(m_lightType, "lightType");
        AddUniform(m_color, "color");
        AddUniform(m_position, "position");
        AddUniform(m_spotDir, "spotDir");
        AddUniform(m_spotExponent, "spotExponent");
        AddUniform(m_spotCutoff, "spotCutoff");
        AddUniform(m_attenuationConst, "attenuationConst");
        AddUniform(m_attenuationLin, "attenuationLin");
        AddUniform(m_attenuationQuad, "attenuationQuad");
    }
};

class LightsUniformArray : public IUniform
{
private:
    Uniform<uint> m_count;
    vector<LightUniform> m_lights;
public:
    LightsUniformArray(const uint& lightsCount, const raw<Light>& lightsCache, const string& name)
        : m_count(name + "_count", lightsCount)
    {
        uint i = 0;
        for(const Light& light : lightsCache) {
            m_lights.emplace_back(light, name + "[" + std::to_string(i) + "]");
            i++;
        }
    }
    void Set(const GraphicsProgramBinder& programBinder) const override {
        m_count.Set(programBinder);
        for(const LightUniform& lightUniform : m_lights) {
            lightUniform.Set(programBinder);
        }
    }
};

class MaterialUniform : public UniformStruct
{
private:
    Uniform<vec3> m_albedo;
    Uniform<float> m_metallic;
    Uniform<float> m_roughness;
    Uniform<float> m_ao;
public:
    MaterialUniform(const Material& material) : MaterialUniform(material, "u_material") {}
    MaterialUniform(const Material& material, const string& name) : UniformStruct(name),
        m_albedo(material.albedo),
        m_metallic(material.metallic),
        m_roughness(material.roughness),
        m_ao(material.ao)
    {
        AddUniform(m_albedo, "albedo");
        AddUniform(m_metallic, "metallic");
        AddUniform(m_roughness, "roughness");
        AddUniform(m_ao, "ao");
    }
};

class PBRForwardUniforms : public Uniforms
{
private:
    Uniform<vec3> m_envGlobalAmbient;
    Uniform<vec3> m_cameraPosition;
    MaterialUniform m_material;
    LightsUniformArray m_lights;
public:
    PBRForwardUniforms(const vec3& envGlobalAmbient, const vec3& cameraPosition, const Material& material, const uint& lightsCount, const raw<Light>& lightsCache)
        :
        m_envGlobalAmbient("u_envGlobalAmbient", envGlobalAmbient),
        m_cameraPosition("u_cameraPosition", cameraPosition),
        m_material(material, "u_material"),
        m_lights(lightsCount, lightsCache, "u_lights")
    {
        AddUniform(m_envGlobalAmbient);
        AddUniform(m_cameraPosition);
        AddUniform(m_material);
        AddUniform(m_lights);
    }
};

class PBRForwardTextureUnits : public TextureUnits
{
public:
    PBRForwardTextureUnits(const MaterialTex& materialTex, Texture2D& whiteTexture)
        : TextureUnits("u_materialTex", 5)
    {
        Add(TextureUnitArgs({materialTex.albedo.value_or(whiteTexture), "albedo", TextureUnit::_0}));
        Add(TextureUnitArgs({materialTex.metallic.value_or(whiteTexture), "metallic", TextureUnit::_1}));
        Add(TextureUnitArgs({materialTex.roughness.value_or(whiteTexture), "roughness", TextureUnit::_2}));
        Add(TextureUnitArgs({materialTex.ao.value_or(whiteTexture), "ao", TextureUnit::_3}));
        Add(TextureUnitArgs({materialTex.normal.value_or(whiteTexture), "normal", TextureUnit::_4}));
    }
};


}

#endif // MORPH_PBR_DATA_HPP