#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>

using namespace magic_enum::ostream_operators;

namespace Morph {

class Application
{
private:
    unique<WindowManager> m_windowManager;
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<MonitorEvent, Application> m_monitorEventAttacher;
    MethodAttacher<WindowCloseEvent, Application> m_windowCloseEventAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<WindowFramebufferSizeEvent, Application> m_windowFramebufferSizeEventAttacher;
    MethodAttacher<WindowContentScaleEvent, Application> m_windowContentScaleEventAttacher;
    MethodAttacher<WindowMinimizeEvent, Application> m_windowMinimizeEventAttacher;
    MethodAttacher<WindowMaximizeEvent, Application> m_windowMaximizeEventAttacher;
    MethodAttacher<WindowRestoreEvent, Application> m_windowRestoreEventAttacher;
    MethodAttacher<WindowFocusEvent, Application> m_windowFocusEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<CharEvent, Application> m_charEventAttacher;
    MethodAttacher<CursorPosEvent, Application> m_cursorPosEventAttacher;
    MethodAttacher<CursorEnterEvent, Application> m_cursorEnterEventAttacher;
    MethodAttacher<MouseButtonEvent, Application> m_couseButtonEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;
    MethodAttacher<WindowDropEvent, Application> m_windowDropEventAttacher;
    WindowID m_windowID;
public:
    Application() {
        spdlog::info("Starting, {}!", "app");
        spdlog::info("enum {} size {}", nameof_type<Key>(), enum_count<Key>());

        m_windowManager = unique<WindowManager>(WindowManager::Get());

        m_windowManagerErrorAttacher = {m_windowManager.get(), this, &Application::OnWindowManagerError};
        m_monitorEventAttacher = {m_windowManager.get(), this, &Application::OnMonitorEvent};
        m_windowCloseEventAttacher = {m_windowManager.get(), this, &Application::OnWindowCloseEvent};
        m_windowSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowSizeEvent};
        m_windowFramebufferSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowFramebufferSizeEvent};
        m_windowContentScaleEventAttacher = {m_windowManager.get(), this, &Application::OnWindowContentScaleEvent};
        m_windowMinimizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowMinimizeEvent};
        m_windowMaximizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowMaximizeEvent};
        m_windowRestoreEventAttacher = {m_windowManager.get(), this, &Application::OnWindowRestoreEvent};
        m_windowFocusEventAttacher = {m_windowManager.get(), this, &Application::OnWindowFocusEvent};
        m_keyEventAttacher = {m_windowManager.get(), this, &Application::OnKeyEvent};
        m_charEventAttacher = {m_windowManager.get(), this, &Application::OnCharEvent};
        m_cursorPosEventAttacher = {m_windowManager.get(), this, &Application::OnCursorPosEvent};
        m_cursorEnterEventAttacher = {m_windowManager.get(), this, &Application::OnCursorEnterEvent};
        m_couseButtonEventAttacher = {m_windowManager.get(), this, &Application::OnMouseButtonEvent};
        m_scrollEventAttacher = {m_windowManager.get(), this, &Application::OnScrollEvent};
        m_windowDropEventAttacher = {m_windowManager.get(), this, &Application::OnWindowDropEvent};

        m_windowID = value_or_panic(m_windowManager->CreateWindow(ivec2(400,400), "events test"), "failed to create window");
    }

    void Run() {
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        unique<GraphicsContext> context = unique<GraphicsContext>(value_or_panic(GraphicsContext::MakeCurrent(window)));
        DefaultFramebuffer& framebuffer = context->GetDefaultFramebuffer();
        FramebufferBinder framebufferBinder = context->BindFramebuffer(framebuffer);
        framebuffer.SetSwapWaitInterval(1);

        GraphicsSettingsApplier graphicsSettingsApplier = context->ApplySettings({ClearColorSetting(vec3(0.2, 0.3, 0.8))});

        while(!window.ShouldClose())
        {
            framebufferBinder.ClearColor();
            framebuffer.SwapBuffers();
            m_windowManager->PollEvents();
        }
    };

private:
    void OnWindowManagerError(const WindowManagerError& error) {
        spdlog::error("{}: {}", error.type, error.message);
    }
    void OnMonitorEvent(const MonitorEvent& event) {
        spdlog::info("monitor {} {}", (void*)event.monitor, event.state == MonitorState::CONNECTED ? "connected" : "disconnected");
    }
    void OnWindowCloseEvent(const WindowCloseEvent& event) {
        spdlog::info("closing window");
    }
    void OnWindowSizeEvent(const WindowSizeEvent& event) {
        spdlog::info("window resize {}", event.size);
    }
    void OnWindowFramebufferSizeEvent(const WindowFramebufferSizeEvent& event) {
        spdlog::info("framebuffer resize {}", event.size);
    }
    void OnWindowContentScaleEvent(const WindowContentScaleEvent& event) {
        spdlog::info("content scale {}", event.scale);
    }
    void OnWindowMinimizeEvent(const WindowMinimizeEvent& event) {
        spdlog::info(event.minimized ? "window minimized" : "window restored from minimization");
    }
    void OnWindowMaximizeEvent(const WindowMaximizeEvent& event) {
        spdlog::info(event.maximized ? "window maximized" : "window restored from maximization");
    }
    void OnWindowRestoreEvent(const WindowRestoreEvent& event) {
        spdlog::info("window restored");
    }
    void OnWindowFocusEvent(const WindowFocusEvent& event) {
        spdlog::info(event.focused ? "window focused" : "window unfocused");
    }
    void OnKeyEvent(const KeyEvent& event) {
        spdlog::info("key {} {}", event.key, event.action);
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        if(event.action == KeyAction::PRESS) {
            if(event.key == Key::ALPHA_1) {
                spdlog::set_level(spdlog::level::trace);
            }
            if(event.key == Key::ALPHA_2) {
                spdlog::set_level(spdlog::level::info);
            }
            if(event.key == Key::R) {
                window.RequestAttention();
            }
            if(event.key == Key::N) {
                window.Minimize();
            }
            if(event.key == Key::M) {
                window.Maximize();
            }
            if(event.key == Key::F) {
                window.SetFullScreen(m_windowManager->GetPrimaryMonitor().value());
            }
            if(event.key == Key::W) {
                window.SetWindowed(ivec2(0,0), ivec2(400, 400));
            }
            if(event.key == Key::D) {
                window.SetDecorated(!window.IsDecorated());
            }
        }
    }
    void OnCharEvent(const CharEvent& event) {
        spdlog::info("char codepoint {}", event.codepoint);
    }
    void OnCursorPosEvent(const CursorPosEvent& event) {
        spdlog::trace("cursor screen pos {} eye pos {} eye dir {}", event.screenPos, event.eyePos, event.eyeDir);
    }
    void OnCursorEnterEvent(const CursorEnterEvent& event) {
        spdlog::info(event.entered ? "cursor entered window" : "cursor exited window");
    }
    void OnMouseButtonEvent(const MouseButtonEvent& event) {
        spdlog::info("mouse button {} {}", event.button, event.pressed ? "pressed" : "released");
    }
    void OnScrollEvent(const ScrollEvent& event) {
        spdlog::info("scroll {}", event.offset);
    }
    void OnWindowDropEvent(const WindowDropEvent& event) {
        spdlog::info("path drop");
    }
};

}

int main(int, char**) {
    Morph::Application app;
    app.Run();
}
