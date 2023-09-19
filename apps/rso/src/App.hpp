#ifndef RSO_APP_HPP
#define RSO_APP_HPP

#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <App/WindowApp.hpp>
#include <Core/JobManager.hpp>

#include "Scene.hpp"

namespace Morph {


class Application : public WindowApp
{
private:
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;

    Scene m_scene;
    JobManager m_jobManager;
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