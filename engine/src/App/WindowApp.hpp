#ifndef MORPH_WINDOW_APP_HPP
#define MORPH_WINDOW_APP_HPP

#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <Profile/Timer.hpp>

#include "App.hpp"
#include "WindowAppConfig.hpp"

namespace Morph {

class WindowApp : public App
{
private:
    ExecutionTypeVar m_executionType;
    unique<WindowManager> m_windowManager;
    MethodAttacher<WindowManagerError, WindowApp> m_windowManagerErrorAttacher;
    WindowID m_windowID;
    ref<Window> m_window;
    unique<GraphicsContext> m_context;
    u64 m_numberOfFramesExecuted = 0;
    f64 m_runTime = 0;
    f64 m_firstFrameRunTime = 0;
    bool m_shouldClose = false;
    TimerResult m_lastIterTime;
    TimerResult m_lastFrameTime;
    Void d_updateExecutionType;
public:
    WindowApp(const WindowAppConfig& config);
    void Run() override;
protected:
    // times are in seconds
    virtual void RunFrame(f64 lastIterTime, f64 lastFrameTime) = 0;

    inline void SetShouldClose() { m_shouldClose = true; }

    inline void SetExecutionType(const ExecutionTypeVar& executionType) {
        m_executionType = executionType; UpdateExecutionType();
    }
    inline const ExecutionTypeVar& GetExecutionType() const { return m_executionType; }

    inline u64 GetNumberOfFramesExecuted() const { return m_numberOfFramesExecuted; }
    // time is in seconds
    inline f64 GetRunTime() const { return m_runTime; }

    inline       WindowManager& windowManager()       { return *m_windowManager; }
    inline const WindowManager& windowManager() const { return *m_windowManager; }
    inline       Window& window()       { return m_window; }
    inline const Window& window() const { return m_window; }
    inline       GraphicsContext& context()       { return *m_context; }
    inline const GraphicsContext& context() const { return *m_context; }
private:
    void UpdateExecutionType();
    void OnWindowManagerError(const WindowManagerError& error);
};

}

#endif // MORPH_WINDOW_APP_HPP