
#ifndef MORPH_GRAPHICS_SETTINGS_HPP
#define MORPH_GRAPHICS_SETTINGS_HPP

#include <Core/Core.hpp>

#include "Enums.hpp"

namespace Morph {

class GraphicsContext;

class GraphicsSettings;

class GraphicsSetting
{
    friend GraphicsSettings;
protected:
    virtual void Set() const = 0;
};

class ClearColorSetting : public GraphicsSetting
{
private:
    vec4 m_color;
public:
    ClearColorSetting() : m_color(0) {}
    ClearColorSetting(vec4 color) : m_color(color) {}
    ClearColorSetting(vec3 color) : m_color(color, 1) {}
    ClearColorSetting(float grey) : m_color(grey, grey, grey, 1) {}
protected:
    void Set() const override;
};
class ClearDepthSetting : public GraphicsSetting
{
private:
    float m_depth;
public:
    ClearDepthSetting() : m_depth(1) {}
    ClearDepthSetting(float depht) : m_depth(depht) {}
protected:
    void Set() const override;
};
class ClearStencilSetting : public GraphicsSetting
{
private:
    int m_stencil;
public:
    ClearStencilSetting() : m_stencil(0) {}
    ClearStencilSetting(int stencil) : m_stencil(stencil) {}
protected:
    void Set() const override;
};
class BlendEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    BlendEnabledSetting() : m_enabled(false) {}
    BlendEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class CullFaceEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    CullFaceEnabledSetting() : m_enabled(false) {}
    CullFaceEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class DepthClampEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    DepthClampEnabledSetting() : m_enabled(false) {}
    DepthClampEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class DepthTestEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    DepthTestEnabledSetting() : m_enabled(false) {}
    DepthTestEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class StencilTestEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    StencilTestEnabledSetting() : m_enabled(false) {}
    StencilTestEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class FramebufferSRGB_EnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    FramebufferSRGB_EnabledSetting() : m_enabled(false) {}
    FramebufferSRGB_EnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class MultisampleEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    MultisampleEnabledSetting() : m_enabled(true) {}
    MultisampleEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class LineSmoothEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    LineSmoothEnabledSetting() : m_enabled(false) {}
    LineSmoothEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class PolygonSmoothEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    PolygonSmoothEnabledSetting() : m_enabled(false) {}
    PolygonSmoothEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class ProgramPointSizeEnabledSetting : public GraphicsSetting
{
private:
    bool m_enabled;
public:
    ProgramPointSizeEnabledSetting() : m_enabled(false) {}
    ProgramPointSizeEnabledSetting(bool enabled) : m_enabled(enabled) {}
protected:
    void Set() const override;
};
class BlendFuncSetting : public GraphicsSetting
{
private:
    BlendFuncType m_srcFactor;
    BlendFuncType m_dstFactor;
public:
    BlendFuncSetting() : m_srcFactor(BlendFuncType::ONE), m_dstFactor(BlendFuncType::ZERO) {}
    BlendFuncSetting(BlendFuncType srcFactor, BlendFuncType dstFactor) : m_srcFactor(srcFactor), m_dstFactor(dstFactor) {}
protected:
    void Set() const override;
};
class CullFaceSetting : public GraphicsSetting
{
private:
    CullFaceType m_cullFaceType;
public:
    CullFaceSetting() : m_cullFaceType(CullFaceType::BACK) {}
    CullFaceSetting(CullFaceType cullFaceType) : m_cullFaceType(cullFaceType) {}
protected:
    void Set() const override;
};
class FrontFaceSetting : public GraphicsSetting
{
private:
    FrontFaceType m_frontFaceType;
public:
    FrontFaceSetting() : m_frontFaceType(FrontFaceType::CCW) {}
    FrontFaceSetting(FrontFaceType frontFaceType) : m_frontFaceType(frontFaceType) {}
protected:
    void Set() const override;
};
class DephtFuncSetting : public GraphicsSetting
{
private:
    DepthFuncType m_depthFuncType;
public:
    DephtFuncSetting() : m_depthFuncType(DepthFuncType::LESS) {}
    DephtFuncSetting(DepthFuncType depthFuncType) : m_depthFuncType(depthFuncType) {}
protected:
    void Set() const override;
};
class DephtRangeSetting : public GraphicsSetting
{
private:
    float m_near;
    float m_far;
public:
    DephtRangeSetting() : m_near(0), m_far(1) {}
    DephtRangeSetting(float near, float far) : m_near(near), m_far(far) {}
protected:
    void Set() const override;
};
class StencilFuncSetting : public GraphicsSetting
{
private:
    StencilFuncType m_stencilFuncType;
    int m_ref;
    uint m_mask;
public:
    StencilFuncSetting() : m_stencilFuncType(StencilFuncType::ALWAYS), m_ref(0), m_mask(UINT32_MAX) {}
    StencilFuncSetting(StencilFuncType stencilFuncType, int ref, uint mask) : m_stencilFuncType(stencilFuncType), m_ref(ref), m_mask(mask) {}
protected:
    void Set() const override;
};
class StencilOpSetting : public GraphicsSetting
{
private:
    StencilOpType m_onStencilFail;
    StencilOpType m_onDephtFail;
    StencilOpType m_onPass;
public:
    StencilOpSetting() : m_onStencilFail(StencilOpType::KEEP), m_onDephtFail(StencilOpType::KEEP), m_onPass(StencilOpType::KEEP) {}
    StencilOpSetting(StencilOpType onStencilFail, StencilOpType onDephtFail, StencilOpType onPass) 
        : m_onStencilFail(onStencilFail), m_onDephtFail(onDephtFail), m_onPass(onPass) {}
protected:
    void Set() const override;
};
class LineWidthSetting : public GraphicsSetting
{
private:
    float m_pixelsWidth;
public:
    LineWidthSetting() : m_pixelsWidth(1) {}
    LineWidthSetting(float pixelsWidth) : m_pixelsWidth(pixelsWidth) {}
protected:
    void Set() const override;
};
class WriteDepthSetting : public GraphicsSetting
{
private:
    bool m_write;
public:
    WriteDepthSetting() : m_write(true) {}
    WriteDepthSetting(bool write) : m_write(write) {}
protected:
    void Set() const override;
};
class WriteStencilSetting : public GraphicsSetting
{
private:
    uint m_writeMask;
public:
    WriteStencilSetting() : m_writeMask(UINT32_MAX) {}
    WriteStencilSetting(bool write) : m_writeMask(write ? UINT32_MAX : 0) {}
    WriteStencilSetting(uint writeMask) : m_writeMask(writeMask) {}
protected:
    void Set() const override;
};
class PolygonModeSetting : public GraphicsSetting
{
private:
    CullFaceType m_faces;
    PolygonModeType m_mode;
public:
    PolygonModeSetting() : m_faces(CullFaceType::FRONT_AND_BACK), m_mode(PolygonModeType::FILL) {}
    PolygonModeSetting(CullFaceType faces, PolygonModeType mode) : m_faces(faces), m_mode(mode) {}
protected:
    void Set() const override;
};
class ViewportSetting : public GraphicsSetting
{
private:
    uvec2 m_offset;
    uvec2 m_size;
public:
    ViewportSetting() : m_offset(uvec2(0)), m_size(uvec2(0)) {}
    ViewportSetting(uvec2 size) : m_offset(uvec2(0)), m_size(size) {}
    ViewportSetting(uvec2 offset, uvec2 size) : m_offset(offset), m_size(size) {}
protected:
    void Set() const override;
};

using GraphicsSettingVar = variant<
    ClearColorSetting,
    ClearDepthSetting,
    ClearStencilSetting,
    BlendEnabledSetting,
    CullFaceEnabledSetting,
    DepthClampEnabledSetting,
    DepthTestEnabledSetting,
    StencilTestEnabledSetting,
    FramebufferSRGB_EnabledSetting,
    MultisampleEnabledSetting,
    LineSmoothEnabledSetting,
    PolygonSmoothEnabledSetting,
    ProgramPointSizeEnabledSetting,
    BlendFuncSetting,
    CullFaceSetting,
    FrontFaceSetting,
    DephtFuncSetting,
    DephtRangeSetting,
    StencilFuncSetting,
    StencilOpSetting,
    LineWidthSetting,
    WriteDepthSetting,
    WriteStencilSetting,
    PolygonModeSetting,
    ViewportSetting
>;

class GraphicsSettingsApplier;

class GraphicsSettings
{
    friend class GraphicsContext;
    friend class GraphicsSettingsApplier;
private:
    unord_map<usize, GraphicsSettingVar> m_settings;
public:
    GraphicsSettings(const init_list<GraphicsSettingVar>& settings);
    GraphicsSettings() {}
    void Add(const GraphicsSettingVar& setting);
    void Insert(const GraphicsSettings& settings);
private:
    void Set() const;
    void Reset(const GraphicsSettings& oldSettings);
    GraphicsSettings ApplyRememberPrev(const GraphicsSettings& settings);
};

class GraphicsSettingsApplier
{
    friend class GraphicsContext;
    struct ApplierData {
        ref<GraphicsSettings> globalSettings;
        GraphicsSettings prevSettings;
    };
private:
    opt<ApplierData> m_data;
protected:
    GraphicsSettingsApplier(const GraphicsSettings& settings, GraphicsSettings& globalSettings);
public:
    GraphicsSettingsApplier() {}
    GraphicsSettingsApplier(const GraphicsSettingsApplier& other) = delete;
    GraphicsSettingsApplier(GraphicsSettingsApplier&& old);
    GraphicsSettingsApplier& operator=(GraphicsSettingsApplier&& old);
    virtual ~GraphicsSettingsApplier();

    inline operator bool() const { return m_data.has_value(); }
};

}

#endif // MORPH_GRAPHICS_SETTINGS_HPP