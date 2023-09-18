
#ifndef MORPH_TEXTURE_UNITS_HPP
#define MORPH_TEXTURE_UNITS_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>

namespace Morph {

struct TextureUnitArgs
{
    ref<Texture> texture;
    string name;
    TextureUnit unit;
};

struct TextureUnitData
{
    ref<Texture> texture;
    TextureUnit unit;
    Uniform<TextureUnit> uniform;
    TextureUnitBinder binder;
};

class TextureUnitsBinder
{
private:
    opt<ref<vector<TextureUnitData>>> m_texturesData;
public:
    TextureUnitsBinder() {}
    TextureUnitsBinder(GraphicsContext& context, const GraphicsProgramBinder& programBinder, vector<TextureUnitData>& texturesData);
    TextureUnitsBinder(const TextureUnitsBinder& other) = delete;
    TextureUnitsBinder(TextureUnitsBinder&& other);
    TextureUnitsBinder& operator=(TextureUnitsBinder&& other);
    ~TextureUnitsBinder();
};

class TextureUnits
{
private:
    string m_structName;
    vector<TextureUnitData> m_texturesData;
public:
    TextureUnits() : m_structName("") {}
    TextureUnits(size_t count) : m_structName("") { m_texturesData.reserve(count); }
    TextureUnits(const string& structName) : m_structName(structName + ".") {}
    TextureUnits(const string& structName, size_t count) : m_structName(structName + ".") { m_texturesData.reserve(count); }
    TextureUnits(init_list<TextureUnitArgs> init);
    TextureUnits(const string& structName, init_list<TextureUnitArgs> init);
    void Add(const TextureUnitArgs& textureUnitArgs);
    TextureUnitsBinder Bind(GraphicsContext& context, const GraphicsProgramBinder& programBinder);
};

}

#endif // MORPH_TEXTURE_UNITS_HPP