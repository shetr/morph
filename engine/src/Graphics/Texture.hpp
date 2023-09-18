
#ifndef MORPH_TEXTURE_HPP
#define MORPH_TEXTURE_HPP

#include <Core/Core.hpp>

#include "Enums.hpp"

namespace Morph {

class GraphicsContext;

class TextureSettings;

class TextureSetting
{
    friend class TextureSettings;
protected:
    virtual void Set(uint textureID) const = 0;
};

class TextureMinFilterSetting : public TextureSetting
{
private:
    TextureMinFilter m_minFilter;
public:
    TextureMinFilterSetting(TextureMinFilter minFilter) : m_minFilter(minFilter) {}
protected:
    void Set(uint textureID) const override;
};

class TextureMagFilterSetting : public TextureSetting
{
private:
    TextureMagFilter m_magFilter;
public:
    TextureMagFilterSetting(TextureMagFilter magFilter) : m_magFilter(magFilter) {}
protected:
    void Set(uint textureID) const override;
};

class TextureWrapSetting : public TextureSetting
{
private:
    TextureWrapAxis m_axis;
    TextureWrapType m_type;
public:
    TextureWrapSetting(TextureWrapAxis axis, TextureWrapType type) : m_axis(axis), m_type(type) {}
protected:
    void Set(uint textureID) const override;
};
class TextureWrap1DSetting : public TextureSetting
{
private:
    TextureWrapType m_type;
public:
    TextureWrap1DSetting(TextureWrapType type) : m_type(type) {}
protected:
    void Set(uint textureID) const override;
};
class TextureWrap2DSetting : public TextureSetting
{
private:
    TextureWrapType m_type;
public:
    TextureWrap2DSetting(TextureWrapType type) : m_type(type) {}
protected:
    void Set(uint textureID) const override;
};
class TextureWrap3DSetting : public TextureSetting
{
private:
    TextureWrapType m_type;
public:
    TextureWrap3DSetting(TextureWrapType type) : m_type(type) {}
protected:
    void Set(uint textureID) const override;
};

using TextureSettingVar = variant<
    TextureMinFilterSetting,
    TextureMagFilterSetting,
    TextureWrapSetting,
    TextureWrap1DSetting,
    TextureWrap2DSetting,
    TextureWrap3DSetting
    >;

class Texture;

class TextureSettings
{
    friend class Texture;
private:
    vector<TextureSettingVar> m_settings;
public:
    TextureSettings(const init_list<TextureSettingVar>& settings) : m_settings(settings) {}
    TextureSettings() {}
    void Add(const TextureSettingVar& setting);
private:
    void Set(uint textureID) const;
};

class Texture
{
    friend class GraphicsContext;
protected:
    uint m_id;
    TextureType m_type;
    TextureSizedFormat m_sizedFormat;
    TextureFormat m_format;
    TextureDataType m_dataType;
public:
    Texture(const Texture& texture) = delete;
    Texture(Texture&& texture);
    Texture& operator=(Texture&& texture);
    virtual ~Texture();

    void GenerateMipmap();

    inline operator bool() const { return m_id != 0; }
    inline uint id() const { return m_id; }
    inline TextureType GetType() const { return m_type; }
    inline TextureSizedFormat GetSizedFormat() const { return m_sizedFormat; }
    inline TextureFormat GetFormat() const { return m_format; }
    inline TextureDataType GetDataType() const { return m_dataType; }
protected:
    Texture() : m_id(0), m_type(TextureType::_1D), m_sizedFormat(TextureSizedFormat::R8), m_format(TextureFormat::R), m_dataType(TextureDataType::UINT8) {}
    Texture(TextureType type, TextureSizedFormat format);
    Texture(TextureType type, TextureSizedFormat format, const TextureSettings& settings);

    void SetData1D(uint dim, const void* data);
    void SetData2D(uvec2 dim, const void* data);
    void SetData3D(uvec3 dim, const void* data);
    void SetDataCubeMap(TextureCubeMapSide side, uint dim, const void* data);
    void SetDataMultisample2D(uvec2 dim, uint samples, bool fixedSampleLocations);
    void SetDataMultisample3D(uvec3 dim, uint samples, bool fixedSampleLocations);

    void UpdateData1D(uint offset, uint subDim, const void* data);
    void UpdateData2D(uvec2 offset, uvec2 subDim, const void* data);
    void UpdateData3D(uvec3 offset, uvec3 subDim, const void* data);
    void UpdateDataCubeMap(TextureCubeMapSide side, uvec2 offset, uvec2 subDim, const void* data);

    void GetData(TextureType type, void* outPixels) const;
    void GetData(TextureType type, void* outPixels, uint level) const;
    void GetData(TextureCubeMapSide side, void* outPixels) const;
    void GetData(TextureCubeMapSide side, void* outPixels, uint level) const;
};

class TextureUnitBinder
{
friend class GraphicsContext;
    struct BinderData {
        ref<Texture> texture;
        ref<bool> bound;
    };
protected:
    opt<BinderData> m_binderData;
    TextureUnit m_unit;
protected:
    TextureUnitBinder(TextureUnit unit, Texture& texture, bool& bound);
public:
    TextureUnitBinder();
    TextureUnitBinder(const TextureUnitBinder& other) = delete;
    TextureUnitBinder(TextureUnitBinder&& other);
    TextureUnitBinder& operator=(TextureUnitBinder&& other);
    virtual ~TextureUnitBinder();

    inline TextureUnit GetUnit() const { return m_unit; }
    inline Texture& GetTexture() { return m_binderData.value().texture; }
    inline operator bool() const { return m_binderData.has_value(); }
};

class ImageBinderImpl
{
protected:
    void BindImage(TextureUnit unit, Texture& texture, GraphicsAccess access, bool layered);
    void UnBindImage(TextureUnit unit);
};

template<bool READ = true, bool WRITE = true>
class ImageBinder : public ImageBinderImpl
{
    static_assert(READ || WRITE);
    friend class GraphicsContext;
    struct BinderData {
        ref<Texture> texture;
        ref<bool> bound;
    };
protected:
    opt<BinderData> m_binderData;
    TextureUnit m_unit;
protected:
    ImageBinder(TextureUnit unit, Texture& texture, bool& bound, bool layered)
        : m_binderData({texture, bound}), m_unit(unit)
    {
        BindImage(m_unit, texture, GetAccess(), layered);
        bound = true;
    }
public:
    ImageBinder() : m_unit(TextureUnit::_0) {}
    ImageBinder(const ImageBinder& other) = delete;
    ImageBinder(ImageBinder&& other) : m_unit(other.m_unit) {
        other.m_unit = TextureUnit::_0;
        std::swap(m_binderData, other.m_binderData);
    }
    ImageBinder& operator=(ImageBinder&& other) {
        std::swap(m_unit, other.m_unit);
        std::swap(m_binderData, other.m_binderData);
        return *this;
    }
    virtual ~ImageBinder() {
        if(m_binderData) {
            UnBindImage(m_unit);
            bool& bound = m_binderData.value().bound;
            bound = false;
        }
    }

    inline GraphicsAccess GetAccess() const { 
        return GraphicsEnums::ConvertAccess(READ, WRITE);
    }
    inline TextureUnit GetUnit() const { return m_unit; }
    inline Texture& GetTexture() { return m_binderData.value().texture; }
    inline operator bool() const { return m_binderData.has_value(); }
};

class Texture1D : public Texture
{
    friend class GraphicsContext;
private:
    uint m_dim;
private:
    Texture1D(uint dim, TextureSizedFormat format);
    Texture1D(uint dim, TextureSizedFormat format, const TextureSettings& settings);
    Texture1D(uint dim, const void* data, TextureSizedFormat format);
    Texture1D(uint dim, const void* data, TextureSizedFormat format, const TextureSettings& settings);
public:
    Texture1D() : Texture(), m_dim(0) {}

    void ResizeUninit(uint dim);
    void ResizeAndSet(uint dim, const void* data);
    void Update(const void* data);
    void Update(uint offset, uint subDim, const void* data);

    void GetData(void* outPixels) const;

    inline uint dim() const { return m_dim; }
};

class Texture2D : public Texture
{
    friend class GraphicsContext;
private:
    uvec2 m_dim;
private:
    Texture2D(uvec2 dim, TextureSizedFormat format);
    Texture2D(uvec2 dim, TextureSizedFormat format, const TextureSettings& settings);
    Texture2D(uvec2 dim, const void* data, TextureSizedFormat format);
    Texture2D(uvec2 dim, const void* data, TextureSizedFormat format, const TextureSettings& settings);
public:
    Texture2D() : Texture(), m_dim(0) {}

    void ResizeUninit(uvec2 dim);
    void ResizeAndSet(uvec2 dim, const void* data);
    void Update(const void* data);
    void Update(uvec2 offset, uvec2 subDim, const void* data);

    void GetData(void* outPixels) const;

    inline uvec2 dim() const { return m_dim; }
};

class Texture3D : public Texture
{
    friend class GraphicsContext;
private:
    uvec3 m_dim;
private:
    Texture3D(uvec3 dim, TextureSizedFormat format);
    Texture3D(uvec3 dim, TextureSizedFormat format, const TextureSettings& settings);
    Texture3D(uvec3 dim, const void* data, TextureSizedFormat format);
    Texture3D(uvec3 dim, const void* data, TextureSizedFormat format, const TextureSettings& settings);
public:
    Texture3D() : Texture(), m_dim(0) {}

    void ResizeUninit(uvec3 dim);
    void ResizeAndSet(uvec3 dim, const void* data);
    void Update(const void* data);
    void Update(uvec3 offset, uvec3 subDim, const void* data);

    void GetData(void* outPixels) const;

    inline uvec3 dim() const { return m_dim; }
};

class TextureCubeMap : public Texture
{
    friend class GraphicsContext;
private:
    uint m_dim;
private:
    TextureCubeMap(uint dim, TextureSizedFormat format);
    TextureCubeMap(uint dim, TextureSizedFormat format, const TextureSettings& settings);
    TextureCubeMap(uint dim, array<const void*, 6> data, TextureSizedFormat format);
    TextureCubeMap(uint dim, array<const void*, 6> data, TextureSizedFormat format, const TextureSettings& settings);
public:
    TextureCubeMap() : Texture(), m_dim(0) {}

    void ResizeUninit(uint dim);
    void ResizeAndSet(uint dim,  array<const void*, 6> data);
    void Update(array<const void*, 6> data);
    void Update(TextureCubeMapSide side, const void* data);
    void Update(TextureCubeMapSide side, uint offset, uint subDim, const void* data);

    void GetData(TextureCubeMapSide side, void* outPixels) const;

    inline uint dim() const { return m_dim; }
};

class TextureMultisample2D : public Texture
{
    friend class GraphicsContext;
private:
    uvec2 m_dim;
    uint m_samples;
private:
    TextureMultisample2D(uvec2 dim, TextureSizedFormat format);
    TextureMultisample2D(uvec2 dim, TextureSizedFormat format, const TextureSettings& settings);
    TextureMultisample2D(uvec2 dim, uint samples, TextureSizedFormat format);
    TextureMultisample2D(uvec2 dim, uint samples, TextureSizedFormat format, const TextureSettings& settings);
public:
    TextureMultisample2D() : Texture(), m_dim(0), m_samples(0) {}

    void Resize(uvec2 dim);
    void ChangeSamples(uint samples);
    void ResizeChangeSamples(uvec2 dim, uint samples);

    inline uvec2 dim() const { return m_dim; }
    inline uint samples() const { return m_samples; }
};

}

#endif // MORPH_TEXTURE_HPP