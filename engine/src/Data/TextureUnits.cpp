#include "TextureUnits.hpp"

namespace Morph {


TextureUnitsBinder::TextureUnitsBinder(GraphicsContext& context, const GraphicsProgramBinder& programBinder, vector<TextureUnitData>& texturesData)
    : m_texturesData(texturesData)
{
    for(TextureUnitData& textureData : texturesData) {
        textureData.uniform.Set(programBinder);
        textureData.binder = context.BindTextureUnit(textureData.unit, textureData.texture);
    }
}
TextureUnitsBinder::TextureUnitsBinder(TextureUnitsBinder&& other)
{
    std::swap(m_texturesData, other.m_texturesData);
}
TextureUnitsBinder& TextureUnitsBinder::operator=(TextureUnitsBinder&& other)
{
    std::swap(m_texturesData, other.m_texturesData);
    return *this;
}
TextureUnitsBinder::~TextureUnitsBinder()
{
    if(m_texturesData) {
        vector<TextureUnitData>& texturesData = m_texturesData.value();
        for(TextureUnitData& textureData : texturesData) {
            textureData.binder = TextureUnitBinder();
        }
    }
}

TextureUnits::TextureUnits(init_list<TextureUnitArgs> init)
    : TextureUnits("", init)
{
}
TextureUnits::TextureUnits(const string& structName, init_list<TextureUnitArgs> init)
    : m_structName(structName + ".")
{
    m_texturesData.reserve(init.size());
    for(const TextureUnitArgs& textureUnitArgs : init) {
        Add(textureUnitArgs);
    }
}
void TextureUnits::Add(const TextureUnitArgs& textureUnitArgs)
{
    m_texturesData.push_back({textureUnitArgs.texture, textureUnitArgs.unit, Uniform<TextureUnit>(textureUnitArgs.unit), TextureUnitBinder()});
    TextureUnitData& textureUnitData = m_texturesData.back();
    textureUnitData.uniform = Uniform<TextureUnit>(m_structName + textureUnitArgs.name, textureUnitData.unit);
}
TextureUnitsBinder TextureUnits::Bind(GraphicsContext& context, const GraphicsProgramBinder& programBinder)
{
    return TextureUnitsBinder(context, programBinder, m_texturesData);
}

}