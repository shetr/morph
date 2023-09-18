#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <App/WindowApp.hpp>

namespace Morph {


class Application : public WindowApp
{
private:
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;

public:
    Application(const WindowAppConfig& config)
        : WindowApp(config),
        m_windowManagerErrorAttacher(&windowManager(), this, &Application::OnWindowManagerError),
        m_windowSizeEventAttacher(&windowManager(), this, &Application::OnWindowSizeEvent),
        m_keyEventAttacher(&windowManager(), this, &Application::OnKeyEvent),
        m_scrollEventAttacher(&windowManager(), this, &Application::OnScrollEvent)
        {}

    void RunFrame(f64 lastIterTime, f64 lastFrameTime) override {

    }
private:
    void OnWindowManagerError(const WindowManagerError& error) {
        spdlog::error("{}: {}", enum_name(error.type), error.message);
    }
    void OnWindowSizeEvent(const WindowSizeEvent& event) {
        spdlog::trace("window resize {}", event.size);
    }
    void OnKeyEvent(const KeyEvent& event) {
        spdlog::trace("key {} {}", enum_name(event.key), enum_name(event.action));
        if(event.action == KeyAction::PRESS) {
            if(event.key == Key::O) {
                spdlog::set_level(spdlog::level::trace);
            }
            if(event.key == Key::P) {
                spdlog::set_level(spdlog::level::info);
            }
        }
    }

    void OnScrollEvent(const ScrollEvent& event) {
        spdlog::trace("scroll {}", event.offset);
    }
};

}

int main(int, char**) {
    using namespace Morph;
    WindowAppConfig appConfig = {
        ivec2(1200,800),
        "rso",
        ExecutionTypes::VsyncFrames()
    };
    Application app(appConfig);
    app.Run();
}