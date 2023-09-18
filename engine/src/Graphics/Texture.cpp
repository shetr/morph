#include "Texture.hpp"

#include "OpenGL.hpp"

namespace Morph {

void TextureSettings::Add(const TextureSettingVar& setting)
{
    m_settings.push_back(setting);
}
void TextureSettings::Set(uint textureID) const
{
    for(const TextureSettingVar& settingVar : m_settings) {
        match(settingVar,
            [&](const TextureSetting& setting) {
                setting.Set(textureID);
            }
        );
    }
}

void TextureMinFilterSetting::Set(uint textureID) const
{
    GL(TextureParameteri(textureID, GL_TEXTURE_MIN_FILTER, GraphicsEnums::textureMinFilters.ToValue(m_minFilter)));
}
void TextureMagFilterSetting::Set(uint textureID) const
{
    GL(TextureParameteri(textureID, GL_TEXTURE_MAG_FILTER, GraphicsEnums::textureMagFilters.ToValue(m_magFilter)));
}
void TextureWrapSetting::Set(uint textureID) const
{
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(m_axis), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
}
void TextureWrap1DSetting::Set(uint textureID) const
{
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(TextureWrapAxis::X), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
}
void TextureWrap2DSetting::Set(uint textureID) const
{
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(TextureWrapAxis::X), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(TextureWrapAxis::Y), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
}
void TextureWrap3DSetting::Set(uint textureID) const
{
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(TextureWrapAxis::X), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(TextureWrapAxis::Y), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
    GL(TextureParameteri(textureID, GraphicsEnums::textureWrapAxis.ToValue(TextureWrapAxis::Z), GraphicsEnums::textureWrapTypes.ToValue(m_type)));
}

Texture::Texture(Texture&& texture) 
    : m_id(texture.m_id), m_type(texture.m_type), m_sizedFormat(texture.m_sizedFormat), m_format(texture.m_format), m_dataType(texture.m_dataType)
{
    texture.m_id = 0;
    texture.m_type = TextureType::_1D;
    texture.m_sizedFormat = TextureSizedFormat::R8;
    texture.m_format = TextureFormat::R;
    texture.m_dataType = TextureDataType::UINT8;
}
Texture& Texture::operator=(Texture&& texture)
{
    m_id = texture.m_id;
    texture.m_id = 0;
    m_type = texture.m_type;
    texture.m_type = TextureType::_1D;
    m_sizedFormat = texture.m_sizedFormat;
    texture.m_sizedFormat = TextureSizedFormat::R8;
    m_format = texture.m_format;
    texture.m_format = TextureFormat::R;
    m_dataType = texture.m_dataType;
    texture.m_dataType = TextureDataType::UINT8;
    return *this;
}


void Texture::GenerateMipmap()
{
    GL(GenerateTextureMipmap(m_id));
}

Texture::Texture(TextureType type, TextureSizedFormat format) : m_id(0), m_type(type), m_sizedFormat(format), m_format(TextureFormat::R), m_dataType(TextureDataType::UINT8)
{
    GL(CreateTextures(GraphicsEnums::textureTypes.ToValue(type), 1, &m_id));
    TextureDataFormat dataFormat = GraphicsEnums::GetTextureDataFormat(format);
    m_format = dataFormat.format;
    m_dataType = dataFormat.type;
}
Texture::Texture(TextureType type, TextureSizedFormat format, const TextureSettings& settings) : Texture(type, format)
{
    settings.Set(m_id);
}
Texture::~Texture()
{
    if(m_id != 0) {
        GL(DeleteTextures(1, &m_id));
    }
}


void Texture::SetData1D(uint dim, const void* data)
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexImage1D(
        GraphicsEnums::textureTypes.ToValue(m_type),
        0,
        GraphicsEnums::textureSizedFormats.ToValue(m_sizedFormat),
        dim,
        0,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::SetData2D(uvec2 dim, const void* data)
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexImage2D(
        GraphicsEnums::textureTypes.ToValue(m_type),
        0,
        GraphicsEnums::textureSizedFormats.ToValue(m_sizedFormat),
        dim.x,
        dim.y,
        0,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::SetData3D(uvec3 dim, const void* data)
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexImage3D(
        GraphicsEnums::textureTypes.ToValue(m_type),
        0,
        GraphicsEnums::textureSizedFormats.ToValue(m_sizedFormat),
        dim.x,
        dim.y,
        dim.z,
        0,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::SetDataCubeMap(TextureCubeMapSide side, uint dim, const void* data)
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexImage2D(
        GraphicsEnums::textureCubeMapSides.ToValue(side),
        0,
        GraphicsEnums::textureSizedFormats.ToValue(m_sizedFormat),
        dim,
        dim,
        0,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::SetDataMultisample2D(uvec2 dim, uint samples, bool fixedSampleLocations)
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexImage2DMultisample(
        GraphicsEnums::textureTypes.ToValue(m_type),
        samples,
        GraphicsEnums::textureSizedFormats.ToValue(m_sizedFormat),
        dim.x,
        dim.y,
        fixedSampleLocations
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::SetDataMultisample3D(uvec3 dim, uint samples, bool fixedSampleLocations)
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexImage3DMultisample(
        GraphicsEnums::textureTypes.ToValue(m_type),
        samples,
        GraphicsEnums::textureSizedFormats.ToValue(m_sizedFormat),
        dim.x,
        dim.y,
        dim.z,
        fixedSampleLocations
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}

void Texture::UpdateData1D(uint offset, uint subDim, const void* data) 
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexSubImage1D(
        GraphicsEnums::textureTypes.ToValue(m_type),
        0,
        offset,
        subDim,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data    
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::UpdateData2D(uvec2 offset, uvec2 subDim, const void* data) 
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexSubImage2D(
        GraphicsEnums::textureTypes.ToValue(m_type),
        0,
        offset.x,
        offset.y,
        subDim.x,
        subDim.y,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data    
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::UpdateData3D(uvec3 offset, uvec3 subDim, const void* data) 
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexSubImage3D(
        GraphicsEnums::textureTypes.ToValue(m_type),
        0,
        offset.x,
        offset.y,
        offset.z,
        subDim.x,
        subDim.y,
        subDim.z,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data    
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::UpdateDataCubeMap(TextureCubeMapSide side, uvec2 offset, uvec2 subDim, const void* data) 
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(TexSubImage2D(
        GraphicsEnums::textureCubeMapSides.ToValue(side),
        0,
        offset.x,
        offset.y,
        subDim.x,
        subDim.y,
        GraphicsEnums::textureFormats.ToValue(m_format),
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        data    
    ));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}

void Texture::GetData(TextureType type, void* outPixels) const
{
    GetData(type, outPixels, 0);
}
void Texture::GetData(TextureType type, void* outPixels, uint level) const
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(GetTexImage(
        GraphicsEnums::textureTypes.ToValue(m_type), 
        level, 
        GraphicsEnums::textureFormats.ToValue(m_format), 
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        outPixels));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}
void Texture::GetData(TextureCubeMapSide side, void* outPixels) const
{
    GetData(side, outPixels, 0);
}
void Texture::GetData(TextureCubeMapSide side, void* outPixels, uint level) const
{
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), m_id));
    GL(GetTexImage(
        GraphicsEnums::textureCubeMapSides.ToValue(side), 
        level, 
        GraphicsEnums::textureFormats.ToValue(m_format), 
        GraphicsEnums::textureDataTypes.ToValue(m_dataType),
        outPixels));
    GL(BindTexture(GraphicsEnums::textureTypes.ToValue(m_type), 0));
}

void ImageBinderImpl::BindImage(TextureUnit unit, Texture& texture, GraphicsAccess access, bool layered)
{
    GL(BindImageTexture(
        enum_index(unit).value(),
        texture.id(),
        0,
        layered,
        0,
        GraphicsEnums::accessValues.ToValue(access),
        GraphicsEnums::textureSizedFormats.ToValue(texture.GetSizedFormat())
    ));
}
void ImageBinderImpl::UnBindImage(TextureUnit unit)
{
    GL(BindImageTexture(
        enum_index(unit).value(),
        0,
        0,
        GL_FALSE,
        0,
        GraphicsEnums::accessValues.ToValue(GraphicsAccess::READ),
        GraphicsEnums::textureSizedFormats.ToValue(TextureSizedFormat::R8)
    ));
}


TextureUnitBinder::TextureUnitBinder(TextureUnit unit, Texture& texture, bool& bound)
    : m_binderData({texture, bound}), m_unit(unit)
{
    GL(BindTextureUnit(enum_index(unit).value(), texture.id()));
    bound = true;
}
TextureUnitBinder::TextureUnitBinder() : m_unit(TextureUnit::_0) {}
TextureUnitBinder::TextureUnitBinder(TextureUnitBinder&& other) : m_unit(other.m_unit)
{
    other.m_unit = TextureUnit::_0;
    std::swap(m_binderData, other.m_binderData);
}
TextureUnitBinder& TextureUnitBinder::operator=(TextureUnitBinder&& other)
{
    std::swap(m_unit, other.m_unit);
    std::swap(m_binderData, other.m_binderData);
    return *this;
}
TextureUnitBinder::~TextureUnitBinder()
{
    if(m_binderData) {
        GL(BindTextureUnit(enum_index(m_unit).value(), 0));
        bool& bound = m_binderData.value().bound;
        bound = false;
    }
}

Texture1D::Texture1D(uint dim, TextureSizedFormat format) 
    : m_dim(dim), Texture(TextureType::_1D, format)
{
    ResizeUninit(dim);
}
Texture1D::Texture1D(uint dim, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), Texture(TextureType::_1D, format, settings)
{
    ResizeUninit(dim);
}
Texture1D::Texture1D(uint dim, const void* data, TextureSizedFormat format) 
    : m_dim(dim), Texture(TextureType::_1D, format)
{
    ResizeAndSet(dim, data);
}
Texture1D::Texture1D(uint dim, const void* data, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), Texture(TextureType::_1D, format, settings)
{
    ResizeAndSet(dim, data);
}

void Texture1D::ResizeUninit(uint dim)
{
    m_dim = dim;
    SetData1D(dim, nullptr);
}
void Texture1D::ResizeAndSet(uint dim, const void* data)
{
    m_dim = dim;
    SetData1D(dim, data);
}
void Texture1D::Update(const void* data)
{
    UpdateData1D(0, m_dim, data);
}
void Texture1D::Update(uint offset, uint subDim, const void* data)
{
    UpdateData1D(offset, subDim, data);
}
void Texture1D::GetData(void* outPixels) const
{
    Texture::GetData(m_type, outPixels);
}


Texture2D::Texture2D(uvec2 dim, TextureSizedFormat format) 
    : m_dim(dim), Texture(TextureType::_2D, format)
{
    ResizeUninit(dim);
}
Texture2D::Texture2D(uvec2 dim, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), Texture(TextureType::_2D, format, settings)
{
    ResizeUninit(dim);
}
Texture2D::Texture2D(uvec2 dim, const void* data, TextureSizedFormat format) 
    : m_dim(dim), Texture(TextureType::_2D, format)
{
    ResizeAndSet(dim, data);
}
Texture2D::Texture2D(uvec2 dim, const void* data, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), Texture(TextureType::_2D, format, settings)
{
    ResizeAndSet(dim, data);
}

void Texture2D::ResizeUninit(uvec2 dim)
{
    m_dim = dim;
    SetData2D(dim, nullptr);
}
void Texture2D::ResizeAndSet(uvec2 dim, const void* data)
{
    m_dim = dim;
    SetData2D(dim, data);
}
void Texture2D::Update(const void* data)
{
    UpdateData2D(uvec2(0), m_dim, data);
}
void Texture2D::Update(uvec2 offset, uvec2 subDim, const void* data)
{
    UpdateData2D(offset, subDim, data);
}
void Texture2D::GetData(void* outPixels) const
{
    Texture::GetData(m_type, outPixels);
}


Texture3D::Texture3D(uvec3 dim, TextureSizedFormat format) 
    : m_dim(dim), Texture(TextureType::_3D, format)
{
    ResizeUninit(dim);
}
Texture3D::Texture3D(uvec3 dim, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), Texture(TextureType::_3D, format, settings)
{
    ResizeUninit(dim);
}
Texture3D::Texture3D(uvec3 dim, const void* data, TextureSizedFormat format) 
    : m_dim(dim), Texture(TextureType::_3D, format)
{
    ResizeAndSet(dim, data);
}
Texture3D::Texture3D(uvec3 dim, const void* data, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), Texture(TextureType::_3D, format, settings)
{
    ResizeAndSet(dim, data);
}

void Texture3D::ResizeUninit(uvec3 dim)
{
    m_dim = dim;
    SetData3D(dim, nullptr);
}
void Texture3D::ResizeAndSet(uvec3 dim, const void* data)
{
    m_dim = dim;
    SetData3D(dim, data);
}
void Texture3D::Update(const void* data)
{
    UpdateData3D(uvec3(0), m_dim, data);
}
void Texture3D::Update(uvec3 offset, uvec3 subDim, const void* data)
{
    UpdateData3D(offset, subDim, data);
}
void Texture3D::GetData(void* outPixels) const
{
    Texture::GetData(m_type, outPixels);
}


TextureCubeMap::TextureCubeMap(uint dim, TextureSizedFormat format)
    : m_dim(dim), Texture(TextureType::CUBE_MAP, format)
{
    ResizeUninit(dim);
}
TextureCubeMap::TextureCubeMap(uint dim, TextureSizedFormat format, const TextureSettings& settings)
    : m_dim(dim), Texture(TextureType::CUBE_MAP, format, settings)
{
    ResizeUninit(dim);
}
TextureCubeMap::TextureCubeMap(uint dim, array<const void*, 6> data, TextureSizedFormat format)
    : m_dim(dim), Texture(TextureType::CUBE_MAP, format)
{
    ResizeAndSet(dim, data);
}
TextureCubeMap::TextureCubeMap(uint dim, array<const void*, 6> data, TextureSizedFormat format, const TextureSettings& settings)
    : m_dim(dim), Texture(TextureType::CUBE_MAP, format, settings)
{
    ResizeAndSet(dim, data);
}

void TextureCubeMap::ResizeUninit(uint dim)
{
    for(TextureCubeMapSide side : enum_values<TextureCubeMapSide>()) {
        SetDataCubeMap(side, dim, nullptr);
    }
}
void TextureCubeMap::ResizeAndSet(uint dim,  array<const void*, 6> data)
{
    usize i = 0;
    for(TextureCubeMapSide side : enum_values<TextureCubeMapSide>()) {
        SetDataCubeMap(side, dim, data[i++]);
    }
}
void TextureCubeMap::Update(array<const void*, 6> data)
{
    usize i = 0;
    for(TextureCubeMapSide side : enum_values<TextureCubeMapSide>()) {
        UpdateDataCubeMap(side, uvec2(0), uvec2(m_dim, m_dim), data[i++]);
    }
}
void TextureCubeMap::Update(TextureCubeMapSide side, const void* data)
{
    UpdateDataCubeMap(side, uvec2(0), uvec2(m_dim, m_dim), data);
}
void TextureCubeMap::Update(TextureCubeMapSide side, uint offset, uint subDim, const void* data)
{
    UpdateDataCubeMap(side, uvec2(offset, offset), uvec2(subDim, subDim), data);
}
void TextureCubeMap::GetData(TextureCubeMapSide side, void* outPixels) const
{
    Texture::GetData(side, outPixels);
}


TextureMultisample2D::TextureMultisample2D(uvec2 dim, TextureSizedFormat format) 
    : m_dim(dim), m_samples(1), Texture(TextureType::_2D_MULTISAMPLE, format)
{
    ResizeChangeSamples(m_dim, m_samples);
}
TextureMultisample2D::TextureMultisample2D(uvec2 dim, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), m_samples(1), Texture(TextureType::_2D_MULTISAMPLE, format, settings)
{
    ResizeChangeSamples(m_dim, m_samples);
}
TextureMultisample2D::TextureMultisample2D(uvec2 dim, uint samples, TextureSizedFormat format) 
    : m_dim(dim), m_samples(samples), Texture(TextureType::_2D_MULTISAMPLE, format)
{
    ResizeChangeSamples(m_dim, m_samples);
}
TextureMultisample2D::TextureMultisample2D(uvec2 dim, uint samples, TextureSizedFormat format, const TextureSettings& settings) 
    : m_dim(dim), m_samples(samples), Texture(TextureType::_2D_MULTISAMPLE, format, settings)
{
    ResizeChangeSamples(m_dim, m_samples);
}

void TextureMultisample2D::Resize(uvec2 dim)
{
    m_dim = dim;
    SetDataMultisample2D(m_dim, m_samples, true);
}
void TextureMultisample2D::ChangeSamples(uint samples)
{
    m_samples = samples;
    SetDataMultisample2D(m_dim, m_samples, true);
}
void TextureMultisample2D::ResizeChangeSamples(uvec2 dim, uint samples)
{
    m_dim = dim;
    m_samples = samples;
    SetDataMultisample2D(m_dim, m_samples, true);
}


}