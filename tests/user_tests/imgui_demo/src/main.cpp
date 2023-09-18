#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <App/WindowApp.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <ImGui/ImGui.hpp>

#include <limits>

using namespace magic_enum::ostream_operators;

namespace Morph {

class Application : public WindowApp
{
private:
    GraphicsSettings graphicsSettings;
    GraphicsSettingsApplier graphicsSettingsApplier;
    ImGuiContext imguiContext;
    vec4 clear_color;
    bool show_demo_window = true;
    bool show_another_window = false;
public:
    Application(const WindowAppConfig& config) 
        : WindowApp(config),
        graphicsSettings({
                MultisampleEnabledSetting(true),
                DepthTestEnabledSetting(true),
                CullFaceEnabledSetting(true),
                CullFaceSetting(CullFaceType::BACK),
                FrontFaceSetting(FrontFaceType::CCW),
                ClearColorSetting(0)
        }),
        graphicsSettingsApplier(context().ApplySettings(graphicsSettings)),
        imguiContext(window()),
        clear_color(0.45f, 0.55f, 0.60f, 1.00f)
    {
    }

public:

    void RunIter(f64 lastIterTime, f64 lastFrameTime) override {
        DefaultFramebuffer& defaultFramebuffer = context().GetDefaultFramebuffer();
        FramebufferBinder framebufferBinder = context().BindFramebuffer(defaultFramebuffer);

        GraphicsSettingsApplier localSettings(context().ApplySettings({ClearColorSetting(clear_color)}));
        framebufferBinder.Clear();
        context().SetViewport(context().GetDefaultFramebuffer().dim());
        
        ImGuiDraw imguiDraw = imguiContext.Draw();

        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        {
            static float f = 0.0f;
            static int counter = 0;

            ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
            ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
            ImGui::Checkbox("Another Window", &show_another_window);

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
            ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
        }

        // 3. Show another simple window.
        if (show_another_window)
        {
            ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
            ImGui::Text("Hello from another window!");
            if (ImGui::Button("Close Me"))
                show_another_window = false;
            ImGui::End();
        }
    };
};

}

int main(int, char**) {
    using namespace Morph;
    WindowAppConfig appConfig = {
        ivec2(1280, 720),
        "imgui demo",
        ExecutionTypes::AdaptiveVsyncFrames()
    };
    Application app(appConfig);
    app.Run();
}
