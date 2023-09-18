#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>
#include <Resource/GraphicsProgramCompiler.hpp>
#include <ImGui/ImGui.hpp>

#include <limits>

using namespace magic_enum::ostream_operators;

namespace Morph {

struct Vertex
{
    vec2 pos;
    vec2 uv;
};

array<Vertex,4> g_vertices = { 
    Vertex({vec2(-1, -1), vec2(0, 0)}),
    Vertex({vec2( 1, -1), vec2(1, 0)}),
    Vertex({vec2( 1,  1), vec2(1, 1)}),
    Vertex({vec2(-1,  1), vec2(0, 1)})
};

array<u32,6> g_indices = {
    0, 1, 2,
    0, 2, 3
};

class Application
{
private:
    unique<WindowManager> m_windowManager;
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<WindowFramebufferSizeEvent, Application> m_windowFramebufferSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    WindowID m_windowID;
    unique<GraphicsContext> context;
    VertexLayout m_verticesLayout = {
        { VertexType::FLOAT32, VertexElementCount::_2 },
        { VertexType::FLOAT32, VertexElementCount::_2 }
    };
    Texture2D m_computeTexture;
public:
    Application() {

        m_windowManager = unique<WindowManager>(WindowManager::Get());

        m_windowManagerErrorAttacher = {m_windowManager.get(), this, &Application::OnWindowManagerError};
        m_windowSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowSizeEvent};
        m_keyEventAttacher = {m_windowManager.get(), this, &Application::OnKeyEvent};

        m_windowID = value_or_panic(m_windowManager->CreateWindow(ivec2(800,800), "compute test"), "failed to create window");
    }

    void Run() {
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        context = unique<GraphicsContext>(value_or_panic(GraphicsContext::MakeCurrent(window)));
        DefaultFramebuffer& defaultFramebuffer = context->GetDefaultFramebuffer();
        defaultFramebuffer.SetSwapWaitInterval(1);

        ImGuiContext imguiContext(window);

        ResourceStorage resStorage;
        resStorage.InsertResFolder("engine", MORPH_ENGINE_RES);
        resStorage.InsertResFolder("app", MORPH_APP_RES);
        GraphicsProgramCompiler progCompiler(*context, resStorage);
        ComputeProgram computeProgram = value_or_panic(progCompiler.CompileComputeProgramRes("app:sphereCmp.glsl"));
        RenderProgram renderProgram = value_or_panic(progCompiler.CompileRenderProgramRes("app:texturedQuad.glsl"));

        VertexBuffer<Vertex> vertexBuffer = context->CreateVertexBuffer(g_vertices, m_verticesLayout);
        IndexBuffer indexBuffer = context->CreateIndexBuffer(g_indices);
        IndexedVertexArray vertexArray = context->CreateIndexedVertexArray(indexBuffer);
        {
            IndexedVertexArrayBinder vertexArrayBinder = context->BindIndexedVertexArray(vertexArray);
            vertexArrayBinder.AddVertexBuffer(vertexBuffer);
        }

        TextureSettings textureSettings = {
            TextureMinFilterSetting(TextureMinFilter::NEAREST),
            TextureMagFilterSetting(TextureMagFilter::NEAREST),
            TextureWrap2DSetting(TextureWrapType::CLAMP_TO_EDGE)
        };
        m_computeTexture = context->CreateTexture2D(window.GetSize(), TextureSizedFormat::RGBA32F, textureSettings);
        TextureUnit textureImageUnit = TextureUnit::_0;
        Uniform<TextureUnit> textureImageUniform("u_textureImage", textureImageUnit);
        TextureUnit textureSamplerUnit = TextureUnit::_0;
        Uniform<TextureUnit> textureSamplerUniform("u_textureSampler", textureSamplerUnit);

        vec3 backgroundColor = vec3(0.05, 0.05, 0.1);
        vec3 sphereColor = vec3(0.1, 0.3, 0.9);
        Uniform<vec3> backgroundColorUniform("u_backgroundColor", backgroundColor);
        Uniform<vec3> sphereColorUniform("u_sphereColor", sphereColor);

        bool show_demo_window = true;

        FramebufferBinder framebufferBinder = context->BindFramebuffer(defaultFramebuffer);
        while(!window.ShouldClose())
        {
            {
                ComputeProgramBinder programBinder = context->BindProgram(computeProgram);
                ImageBinder<false, true> imageBinder = context->BindImage<false, true>(textureImageUnit, m_computeTexture);
                textureImageUniform.Set(programBinder);
                backgroundColorUniform.Set(programBinder);
                sphereColorUniform.Set(programBinder);
                context->DispatchCompute(programBinder, uvec3(window.GetSize(), 1));
            }
            context->MemoryBarrier(bits(MemoryBarrierType::SHADER_IMAGE_ACCESS));
            {
                RenderProgramBinder programBinder = context->BindProgram(renderProgram);
                IndexedVertexArrayBinder vertexArrayBinder = context->BindIndexedVertexArray(vertexArray);
                TextureUnitBinder textureUnitBinder = context->BindTextureUnit(textureSamplerUnit, m_computeTexture);
                textureSamplerUniform.Set(programBinder);
                framebufferBinder.DrawElements(PrimitiveType::TRIANGLES, programBinder, vertexArrayBinder, 6);
            }
            {
                ImGuiDraw imguiDraw = imguiContext.Draw();
                
                static float f = 0.0f;
                static int counter = 0;

                ImGui::Begin("imgui test");                          // Create a window called "Hello, world!" and append into it.

                ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

                ImGui::SliderFloat("float", &f, 0, 50);            // Edit 1 float using a slider from 0.0f to 1.0f
                ImGui::ColorEdit3("background color", (float*)&backgroundColor);
                ImGui::ColorEdit3("sphere color", (float*)&sphereColor);

                if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                    counter++;
                ImGui::SameLine();
                ImGui::Text("counter = %d", counter);

                ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                ImGui::End();
            }
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
        if(m_computeTexture) {
            m_computeTexture.ResizeUninit(event.size);
        }
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
