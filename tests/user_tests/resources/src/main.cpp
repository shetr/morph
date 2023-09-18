#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <Resource/Storage.hpp>

using namespace magic_enum::ostream_operators;

namespace Morph {

class Application
{
private:
    unique<WindowManager> m_windowManager;
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<WindowFramebufferSizeEvent, Application> m_windowFramebufferSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    WindowID m_windowID;
    unique<GraphicsContext> m_context;
public:
    Application() {

        spdlog::info("engine res location: {}", MORPH_ENGINE_RES);
        spdlog::info("app res location: {}", MORPH_APP_RES);

        m_windowManager = unique<WindowManager>(WindowManager::Get());

        m_windowManagerErrorAttacher = {m_windowManager.get(), this, &Application::OnWindowManagerError};
        m_windowSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowSizeEvent};
        m_keyEventAttacher = {m_windowManager.get(), this, &Application::OnKeyEvent};

        m_windowID = value_or_panic(m_windowManager->CreateWindow(ivec2(400,400), "resources test"), "failed to create window");
    }

    void Run() {
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        m_context = unique<GraphicsContext>(value_or_panic(GraphicsContext::MakeCurrent(window)));

        ResourceStorage resStorage;
        resStorage.InsertResFolder("engine", MORPH_ENGINE_RES);
        resStorage.InsertResFolder("app", MORPH_APP_RES);

        string fileContents = value_or_panic(resStorage.ReadFileRes("app:testFile.txt"), "cannot read file");
        spdlog::info("file contents: {}", fileContents);

        GraphicsSettings settings = { ClearColorSetting(vec3(0.8, 0.2, 0.1))};
        GraphicsSettingsApplier settingsApplyer = m_context->ApplySettings(settings);

        DefaultFramebuffer& defaultFramebuffer = m_context->GetDefaultFramebuffer();
        defaultFramebuffer.SetSwapWaitInterval(1);

        FramebufferBinder framebufferBinder = m_context->BindFramebuffer(defaultFramebuffer);
        while(!window.ShouldClose())
        {
            framebufferBinder.ClearColor();
            defaultFramebuffer.SwapBuffers();
            m_windowManager->PollEvents();
        }

    };

private:
    void OnWindowManagerError(const WindowManagerError& error) {
        spdlog::error("{}: {}", error.type, error.message);
    }
    void OnWindowSizeEvent(const WindowSizeEvent& event) {
        spdlog::trace("window resize {}", event.size);
        m_context->GetDefaultFramebuffer().Resize(event.size);
    }
    void OnKeyEvent(const KeyEvent& event) {
        spdlog::trace("key {} {}", event.key, event.action);
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
};

}

int main(int, char**) {
    Morph::Application app;
    app.Run();
}
