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

class ViewTransform
{
public:
    vec2 pos = vec2(-1.0, 0);
    float rot = 0;
    float scale = 1;
    float aspectRatio = 1;
    mat2 scaleMat = mat2(1);
    mat2 rotMat = mat2(1);
    mat2 rotScaleMat = mat2(1);
    mat3 trans = mat3(1);
public:
    void Recalculate() {
        scaleMat = mat2(1);
        scaleMat[0][0] = aspectRatio *scale;
        scaleMat[1][1] = scale;
        float rad = glm::radians(rot);
        float cosmrot = glm::cos(-rad);
        float sinmrot = glm::sin(-rad);
        rotMat = mat2(cosmrot, sinmrot, -sinmrot, cosmrot);
        rotScaleMat = rotMat * scaleMat;
        trans = mat3(rotScaleMat);
        trans[2].x = pos.x;
        trans[2].y = pos.y;
    }
};

template<uint QUERIES_COUNT>
class GraphicsTimer
{
private:
    usize m_useIndex = 0;
    ref<GraphicsContext> m_context;
    array<GraphicsTimeElapsedQuery, QUERIES_COUNT> m_timerQueries;
    array<bool, QUERIES_COUNT> m_timerQueriesUsed;
public:
    GraphicsTimer(GraphicsContext& context) : m_context(context) {
        for(GraphicsTimeElapsedQuery& timerQuery : m_timerQueries) {
            timerQuery = context.CreateQuery<GraphicsQueryType::TIME_ELAPSED>();
        }
        m_timerQueriesUsed.fill(false);
    }

    GraphicsTimeElapsedQueryScope Begin() {
        GraphicsContext& context = m_context;
        GraphicsTimeElapsedQueryScope scope = context.BeginQuery(m_timerQueries[m_useIndex]);
        m_timerQueriesUsed[m_useIndex] = true;
        m_useIndex = (m_useIndex + 1) % QUERIES_COUNT;
        return scope;
    }

    u64 GetAvgTime() const {
        u64 avg = 0;
        u64 count = 0;
        usize i = 0;
        for(const GraphicsTimeElapsedQuery& timerQuery : m_timerQueries) {
            if(m_timerQueriesUsed[m_useIndex] && timerQuery.IsValueAvailable()) {
                avg += timerQuery.GetValueNoWait();
                count++;
            }
            i++;
        }
        if(count == 0) {
            return 0;
        }
        return avg / count;
    }
};

struct CountsStorageData
{
    u32 minIterations;
    u32 maxIterations;
};

class Application
{
private:
    unique<WindowManager> m_windowManager;
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<WindowFramebufferSizeEvent, Application> m_windowFramebufferSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;
    WindowID m_windowID;
    unique<GraphicsContext> m_context;
    array<vec2,4> m_quadVertices = { 
        vec2(-1, -1),
        vec2( 1, -1),
        vec2( 1,  1),
        vec2(-1,  1)
    };
    array<vec2,4> m_quadUvs = { 
        vec2(0, 0),
        vec2(1, 0),
        vec2(1, 1),
        vec2(0, 1)
    };

    array<u32,6> m_quadIndices = {
        0, 1, 2,
        0, 2, 3
    };
    VertexLayout m_basicLayout = {
        { VertexType::FLOAT32, VertexElementCount::_2 }
    };
    CustomFramebuffer m_iterationsFramebuffer;
    ViewTransform m_viewTransfrorm;
    float m_moveSpeed = 1;
    float m_scrollZoomSpeed = 0.02;
    float m_keyZoomSpeed = 0.25;
    float m_rotSpeed = 45;
    vec2 m_settingsWindowPos = vec2(0);
    vec2 m_settingsWindowSize = vec2(0);
    float m_zoom = 0;
    float m_computeTimer = 0;
    float m_renderTimer = 0;
    int m_antialiasing = 0;
    int m_SSAA_item_current = 0;
    array<uint,4> m_samplesWidths = {1, 2, 3, 4};
    uint m_currentSamplesWidth = 1;
    int m_currentAntialiasing = 0;
public:
    Application() {

        m_windowManager = unique<WindowManager>(WindowManager::Get());

        m_windowManagerErrorAttacher = {m_windowManager.get(), this, &Application::OnWindowManagerError};
        m_windowSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowSizeEvent};
        m_keyEventAttacher = {m_windowManager.get(), this, &Application::OnKeyEvent};
        m_scrollEventAttacher = {m_windowManager.get(), this, &Application::OnScrollEvent};

        m_windowID = value_or_panic(m_windowManager->CreateWindow(ivec2(1200,800), "mandelbrot"), "failed to create window");
    }

    void Run() {
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        m_context = unique<GraphicsContext>(value_or_panic(GraphicsContext::MakeCurrent(window)));
        DefaultFramebuffer& defaultFramebuffer = m_context->GetDefaultFramebuffer();
        defaultFramebuffer.SetSwapWaitInterval(1);

        GraphicsSettings graphicsSettings = {
            MultisampleEnabledSetting(true)
        };
        GraphicsSettingsApplier graphicsSettingsApplier = m_context->ApplySettings(graphicsSettings);

        ImGuiContext imguiContext(window);

        ResourceStorage resStorage;
        resStorage.InsertResFolder("engine", MORPH_ENGINE_RES);
        resStorage.InsertResFolder("app", MORPH_APP_RES);
        GraphicsProgramCompiler progCompiler(*m_context, resStorage);

        RenderProgram getIterationsProgram = value_or_panic(progCompiler.CompileRenderProgramRes("app:GetIterations.glsl"));
        RenderProgram showIterationsProgram = value_or_panic(progCompiler.CompileRenderProgramRes("app:ShowIterations.glsl"));
        
        VertexBuffer<vec2> verticesBuffer = m_context->CreateVertexBuffer(m_quadVertices, m_basicLayout);
        VertexBuffer<vec2> uvsBuffer = m_context->CreateVertexBuffer(m_quadUvs, m_basicLayout);
        IndexBuffer indicesBuffer = m_context->CreateIndexBuffer(m_quadIndices);
        IndexedVertexArray basicQuadArray = m_context->CreateIndexedVertexArray(indicesBuffer);
        {
            IndexedVertexArrayBinder vertexArrayBinder = m_context->BindIndexedVertexArray(basicQuadArray);
            vertexArrayBinder.AddVertexBuffer(verticesBuffer);
        }
        IndexedVertexArray uvedQuadArray = m_context->CreateIndexedVertexArray(indicesBuffer);
        {
            IndexedVertexArrayBinder vertexArrayBinder = m_context->BindIndexedVertexArray(uvedQuadArray);
            vertexArrayBinder.AddVertexBuffer(verticesBuffer);
            vertexArrayBinder.AddVertexBuffer(uvsBuffer);
        }
        
        uint iterationsLimit = 255;

        m_iterationsFramebuffer = m_context->CreateFramebuffer();
        TextureSettings textureSettings = {
            TextureMinFilterSetting(TextureMinFilter::NEAREST),
            TextureMagFilterSetting(TextureMagFilter::NEAREST),
            TextureWrap2DSetting(TextureWrapType::CLAMP_TO_EDGE)
        };
        uint initSamples = 1;
        Texture2D iterationsTexture = m_context->CreateTexture2D(window.GetSize(), TextureSizedFormat::R32UI, textureSettings);
        Renderbuffer iterationsRenderbuffer = m_context->CreateRenderbuffer(window.GetSize(), TextureSizedFormat::DEPTH24_STENCIL8);
        m_iterationsFramebuffer.Attach(CustomFramebufferAttachment::COLOR_0, iterationsTexture);
        m_iterationsFramebuffer.Attach(CustomFramebufferAttachment::DEPTH_STENCIL, iterationsRenderbuffer);


        GraphicsTimer<5> computeTimer(*m_context);
        GraphicsTimer<5> renderTimer(*m_context);

        m_viewTransfrorm.aspectRatio = (float)window.GetSize().x / (float)window.GetSize().y;
        m_viewTransfrorm.Recalculate();

        TextureUnit textureSamplerUnit = TextureUnit::_0;

        Uniform<mat3> transformUniform("u_transform", m_viewTransfrorm.trans);
        Uniform<uint> iterationsLimit1Uniform("u_iterationsLimit", iterationsLimit);

        Uniform<uint> iterationsLimit2Uniform("u_iterationsLimit", iterationsLimit);
        Uniform<TextureUnit> textureSamplerUniform("u_iterationsSampler", textureSamplerUnit);

        vec3 insideColor = vec3(0.0, 0.0, 0.0);
        vec3 minColor = vec3(0.0, 0.0, 0.0);
        vec3 maxColor = vec3(1.0, 1.0, 1.0);
        Uniform<vec3> insideColorUniform("u_insideColor", insideColor);
        Uniform<vec3> minColorUniform("u_minColor", minColor);
        Uniform<vec3> maxColorUniform("u_maxColor", maxColor);
        Uniform<int> antialiasingUniform("u_antialiasing", m_antialiasing);
        Uniform<uint> SSAA_samplesWidthUniform("u_SSAA_samplesWidth", m_currentSamplesWidth);

        bool show_demo_window = true;

        dvec2 lastCursorPos = window.GetCursorPosEye();
        vec2 realCursorPos = vec2(0);

        while(!window.ShouldClose())
        {
            float deltaTime = 1.0f / ImGui::GetIO().Framerate;
            // logic
            {
                dvec2 cursorPos = window.GetCursorPosEye();
                if(!ImGui::IsAnyWindowHovered() && !ImGui::IsAnyWindowFocused() && window.IsMouseButtonPressed(MouseButton::LEFT_1)) {
                    vec2 cursorDir = cursorPos - lastCursorPos;
                    m_viewTransfrorm.pos -= m_viewTransfrorm.rotScaleMat * cursorDir;
                }
                if(window.IsKeyPressed(Key::A)) {
                    m_viewTransfrorm.pos -= m_viewTransfrorm.rotScaleMat[0] * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::D)) {
                    m_viewTransfrorm.pos += m_viewTransfrorm.rotScaleMat[0] * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::S)) {
                    m_viewTransfrorm.pos -= m_viewTransfrorm.rotScaleMat[1] * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::W)) {
                    m_viewTransfrorm.pos += m_viewTransfrorm.rotScaleMat[1] * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::Q)) {
                    m_viewTransfrorm.rot -= m_rotSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::E)) {
                    m_viewTransfrorm.rot += m_rotSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::N)) {
                    m_zoom -= m_keyZoomSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::M)) {
                    m_zoom += m_keyZoomSpeed * deltaTime;
                }

                lastCursorPos = cursorPos;
                realCursorPos = vec2(m_viewTransfrorm.trans * vec3(vec2(lastCursorPos), 1));
                m_viewTransfrorm.scale = glm::pow(10, -m_zoom);
                m_viewTransfrorm.Recalculate();
            }
            {
                uint samplesWidth = m_samplesWidths[m_SSAA_item_current];
                if(samplesWidth != m_currentSamplesWidth || (m_currentAntialiasing != 2 && m_antialiasing == 2)) {
                    m_currentSamplesWidth = samplesWidth;
                    m_currentAntialiasing = m_antialiasing;
                    m_iterationsFramebuffer.Resize(defaultFramebuffer.dim() * uvec2(m_currentSamplesWidth));
                }
                if(m_currentAntialiasing == 2 && m_currentAntialiasing != m_antialiasing) {
                    m_currentAntialiasing = m_antialiasing;
                    m_iterationsFramebuffer.Resize(defaultFramebuffer.dim());
                }
            }
            // compute iterations
            {
                GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = computeTimer.Begin();
                FramebufferBinder framebufferBinder = m_context->BindFramebuffer(m_iterationsFramebuffer);
                RenderProgramBinder programBinder = m_context->BindProgram(getIterationsProgram);
                IndexedVertexArrayBinder vertexArrayBinder = m_context->BindIndexedVertexArray(basicQuadArray);
                transformUniform.Set(programBinder);
                iterationsLimit1Uniform.Set(programBinder);
                m_context->SetViewport(m_iterationsFramebuffer.dim());
                framebufferBinder.DrawElements(PrimitiveType::TRIANGLES, programBinder, vertexArrayBinder, 6);
            }
            // draw iterations
            {
                GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = renderTimer.Begin();
                FramebufferBinder framebufferBinder = m_context->BindFramebuffer(defaultFramebuffer);
                RenderProgramBinder programBinder = m_context->BindProgram(showIterationsProgram);
                IndexedVertexArrayBinder vertexArrayBinder = m_context->BindIndexedVertexArray(uvedQuadArray);
                TextureUnitBinder textureUnitBinder = m_context->BindTextureUnit(textureSamplerUnit, iterationsTexture);
                iterationsLimit2Uniform.Set(programBinder);
                textureSamplerUniform.Set(programBinder);
                insideColorUniform.Set(programBinder);
                minColorUniform.Set(programBinder);
                maxColorUniform.Set(programBinder);
                antialiasingUniform.Set(programBinder);
                SSAA_samplesWidthUniform.Set(programBinder);
                m_context->SetViewport(m_context->GetDefaultFramebuffer().dim());
                framebufferBinder.DrawElements(PrimitiveType::TRIANGLES, programBinder, vertexArrayBinder, 6);
            }
            m_computeTimer = (float)computeTimer.GetAvgTime() / 1000000.0f;
            m_renderTimer = (float)renderTimer.GetAvgTime() / 1000000.0f;
            // draw ui
            {
                ImGuiDraw imguiDraw = imguiContext.Draw();

                ImGui::Begin("Mandelbrot settings");

                m_settingsWindowPos = vec2(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y);
                m_settingsWindowSize = vec2(ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);

                if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Text("cursor pos = %f, %f", realCursorPos.x, realCursorPos.y);
                    ImGui::InputFloat2("position", (float*)&m_viewTransfrorm.pos);
                    ImGui::InputFloat("zoom", &m_zoom);
                    ImGui::Text("scale = %f", m_viewTransfrorm.scale);
                    ImGui::InputFloat("rotation", &m_viewTransfrorm.rot);
                    ImGui::Text("Speeds:");
                    ImGui::InputFloat("move speed", &m_moveSpeed);
                    ImGui::InputFloat("scroll zoom speed", &m_scrollZoomSpeed);
                    ImGui::InputFloat("key zoom speed", &m_keyZoomSpeed);
                    ImGui::InputFloat("rotation speed", &m_rotSpeed);
                }
                if (ImGui::CollapsingHeader("Appearence", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    {
                        static const char* items[] = { "NONE", "FXAA", "SSAA"};
                        ImGui::Combo("antialiasing", &m_antialiasing, items, IM_ARRAYSIZE(items));
                    }
                    if(m_antialiasing == 2) {
                        static const char* items[] = { "1x", "4x", "9x", "16x"};
                        ImGui::Combo("SSAA samples", &m_SSAA_item_current, items, IM_ARRAYSIZE(items));
                    } else {
                        ImGui::Text(" ");
                    }
                    ImGui::ColorEdit3("inside color", (float*)&insideColor);
                    ImGui::ColorEdit3("min color", (float*)&minColor);
                    ImGui::ColorEdit3("max color", (float*)&maxColor);
                }
                if (ImGui::CollapsingHeader("Computation", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    static int sliderIterations = 300;
                    ImGui::InputInt("iterations limit max", &sliderIterations);
                    ImGui::SliderInt("iterations limit", (int*)&iterationsLimit, 1, sliderIterations);
                }
                if (ImGui::CollapsingHeader("Statistics", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Text("gpu avg. compute time %.3f ms", m_computeTimer);
                    ImGui::Text("gpu avg. render time %.3f ms", m_renderTimer);
                    ImGui::Text("application avg. %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
                }

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
        if(m_iterationsFramebuffer) {
            m_iterationsFramebuffer.Resize(event.size * uvec2(m_currentSamplesWidth));
        }
        m_viewTransfrorm.aspectRatio = (float)event.size.x / (float)event.size.y;
    }
    void OnKeyEvent(const KeyEvent& event) {
        spdlog::trace("key {} {}", event.key, event.action);
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
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
        m_zoom += event.offset.y * m_scrollZoomSpeed;
    }
};

}

int main(int, char**) {
    Morph::Application app;
    app.Run();
}
