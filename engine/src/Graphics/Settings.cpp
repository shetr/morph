#include "Settings.hpp"

#include "OpenGL.hpp"

namespace Morph {


void ClearColorSetting::Set() const
{
    GL(ClearColor(m_color.r, m_color.g, m_color.b, m_color.a));
}
void ClearDepthSetting::Set() const
{
    GL(ClearDepthf(m_depth));
}
void ClearStencilSetting::Set() const
{
    GL(ClearStencil(m_stencil));
}
void BlendEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_BLEND));
    } else {
        GL(Disable(GL_BLEND));
    }
}
void CullFaceEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_CULL_FACE));
    } else {
        GL(Disable(GL_CULL_FACE));
    }
}
void DepthClampEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_DEPTH_CLAMP));
    } else {
        GL(Disable(GL_DEPTH_CLAMP));
    }
}
void DepthTestEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_DEPTH_TEST));
    } else {
        GL(Disable(GL_DEPTH_TEST));
    }
}
void StencilTestEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_STENCIL_TEST));
    } else {
        GL(Disable(GL_STENCIL_TEST));
    }
}
void FramebufferSRGB_EnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_FRAMEBUFFER_SRGB));
    } else {
        GL(Disable(GL_FRAMEBUFFER_SRGB));
    }
}
void MultisampleEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_MULTISAMPLE));
    } else {
        GL(Disable(GL_MULTISAMPLE));
    }
}
void LineSmoothEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_LINE_SMOOTH));
    } else {
        GL(Disable(GL_LINE_SMOOTH));
    }
}
void PolygonSmoothEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_POLYGON_SMOOTH));
    } else {
        GL(Disable(GL_POLYGON_SMOOTH));
    }
}
void ProgramPointSizeEnabledSetting::Set() const
{
    if(m_enabled) {
        GL(Enable(GL_PROGRAM_POINT_SIZE));
    } else {
        GL(Disable(GL_PROGRAM_POINT_SIZE));
    }
}
void BlendFuncSetting::Set() const
{
    GL(BlendFunc(GraphicsEnums::blendFuncTypes.ToValue(m_srcFactor), GraphicsEnums::blendFuncTypes.ToValue(m_dstFactor)));
}
void CullFaceSetting::Set() const
{
    GL(CullFace(GraphicsEnums::cullFaceTypes.ToValue(m_cullFaceType)));
}
void FrontFaceSetting::Set() const
{
    GL(FrontFace(GraphicsEnums::frontFaceTypes.ToValue(m_frontFaceType)));
}
void DephtFuncSetting::Set() const
{
    GL(DepthFunc(GraphicsEnums::depthFuncTypes.ToValue(m_depthFuncType)));
}
void DephtRangeSetting::Set() const
{
    GL(DepthRange(m_near, m_far));
}
void StencilFuncSetting::Set() const
{
    GL(StencilFunc(GraphicsEnums::stencilFuncTypes.ToValue(m_stencilFuncType), m_ref, m_mask));
}
void StencilOpSetting::Set() const
{
    GL(StencilOp(
        GraphicsEnums::stencilOpTypes.ToValue(m_onStencilFail),
        GraphicsEnums::stencilOpTypes.ToValue(m_onDephtFail),
        GraphicsEnums::stencilOpTypes.ToValue(m_onPass)
    ));
}
void LineWidthSetting::Set() const
{
    GL(LineWidth(m_pixelsWidth));
}
void WriteDepthSetting::Set() const
{
    GL(DepthMask(m_write));
}
void WriteStencilSetting::Set() const
{
    GL(StencilMask(m_writeMask));
}
void PolygonModeSetting::Set() const
{
    GL(PolygonMode(
        GraphicsEnums::cullFaceTypes.ToValue(m_faces),
        GraphicsEnums::polygonModeTypes.ToValue(m_mode)
    ));
}
void ViewportSetting::Set() const
{
    GL(Viewport(m_offset.x, m_offset.y, m_size.x, m_size.y));
}

GraphicsSettings::GraphicsSettings(const init_list<GraphicsSettingVar>& settings) {
    for(const GraphicsSettingVar& setting : settings) {
        m_settings.insert_or_assign(setting.index(), setting);
    }
}
void GraphicsSettings::Add(const GraphicsSettingVar& setting)
{
    m_settings.insert_or_assign(setting.index(), setting);
}
void GraphicsSettings::Insert(const GraphicsSettings& settings)
{
    for(const pair<usize, GraphicsSettingVar>& settingPair : settings.m_settings) {
        Add(settingPair.second);
    }
}
void GraphicsSettings::Set() const
{
    for(const pair<usize, GraphicsSettingVar>& settingPair : m_settings) {
        const GraphicsSettingVar& settingVar = settingPair.second;
        match(settingVar,
            [](const GraphicsSetting& setting) {
                setting.Set();
            }
        );
    }
}
void GraphicsSettings::Reset(const GraphicsSettings& oldSettings)
{
    for(const pair<usize, GraphicsSettingVar>& settingPair : oldSettings.m_settings) {
        const GraphicsSettingVar& settingVar = settingPair.second;
        Add(settingVar);
        match(settingVar,
            [](const GraphicsSetting& setting) {
                setting.Set();
            }
        );
    }
}
GraphicsSettings GraphicsSettings::ApplyRememberPrev(const GraphicsSettings& settings)
{
    GraphicsSettings changes;
    for(const pair<usize, GraphicsSettingVar>& settingPair : settings.m_settings) {
        auto settingPairIt = m_settings.find(settingPair.first);
        if(settingPairIt != m_settings.end()) {
            const GraphicsSettingVar& settingVar = settingPairIt->second;
            changes.Add(settingVar);
        }
        Add(settingPair.second);
        match(settingPair.second,
            [](const GraphicsSetting& setting) {
                setting.Set();
            }
        );
    }
    return changes;
}

GraphicsSettingsApplier::GraphicsSettingsApplier(const GraphicsSettings& settings, GraphicsSettings& globalSettings)
    : m_data({globalSettings, globalSettings.ApplyRememberPrev(settings)})
{
}
GraphicsSettingsApplier::GraphicsSettingsApplier(GraphicsSettingsApplier&& old)
{
    std::swap(m_data, old.m_data);
}
GraphicsSettingsApplier& GraphicsSettingsApplier::operator=(GraphicsSettingsApplier&& old)
{
    std::swap(m_data, old.m_data);
    return *this;
}
GraphicsSettingsApplier::~GraphicsSettingsApplier()
{
    if(m_data) {
        const ApplierData& data = m_data.value();
        data.globalSettings.get().Reset(data.prevSettings);
    }
}


}