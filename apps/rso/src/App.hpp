#ifndef RSO_APP_HPP
#define RSO_APP_HPP

#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Core/JobManager.hpp>
#include <App/WindowApp.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>
#include <Resource/GraphicsProgramCompiler.hpp>
#include <Resource/Common.hpp>

#include "Scene.hpp"

namespace Morph {


class Application : public WindowApp
{
private:
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;

    JobManager m_jobManager;
    Scene m_scene;

    GraphicsSettings m_graphicsSettings;
    GraphicsSettingsApplier m_graphicsSettingsApplier;
    ResourceStorage m_resStorage;
    GraphicsProgramCompiler m_progCompiler;
    CommonResources m_commonResources;
    TextureSettings m_screenTextureSettings;
    Texture2D m_screenTexture;
    TextureUnit m_screenTextureSamplerUnit;
    Uniform<TextureUnit> m_screenTextureSamplerUniform;
public:
    Application(const WindowAppConfig& config);

    void RunFrame(f64 lastIterTime, f64 lastFrameTime) override;
private:
    void OnWindowManagerError(const WindowManagerError& error);
    void OnWindowSizeEvent(const WindowSizeEvent& event);
    void OnKeyEvent(const KeyEvent& event);
    void OnScrollEvent(const ScrollEvent& event);

    void Usage();
};

}

#endif // RSO_APP_HPP