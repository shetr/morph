#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>
#include <Resource/GraphicsProgramCompiler.hpp>
#include <Resource/Common.hpp>
#include <Data/ShaderTransform.hpp>
#include <Data/Transform.hpp>
#include <Data/Camera.hpp>
#include <Data/Phong.hpp>
#include <ImGui/ImGui.hpp>

#include <limits>

using namespace magic_enum::ostream_operators;

namespace Morph {

struct CameraData
{
    PerspectiveCamera3D camera;
    TransformRotAngleAxis transform;
};

struct ObjectData
{
    TransformRotAngleAxis transform;
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

    CustomFramebuffer m_pingPongFramebuffers[2];
    Texture2D m_pingPongTextures[2];
    Renderbuffer m_pingPongRenderbuffers[2];

    float m_renderTimer = 0;
    float m_operationTimer = 0;
    CameraData m_cameraData;
    ObjectData m_objectData;
    vec4 m_envGlobalAmbient = vec4(1);
    PhongMaterial m_material;
    PhongLight m_light;
    PhongMaterialTex m_textures;
    bool m_showLight = true;
    float m_moveSpeed = 1;
    float m_rotSpeed = 1;

    bool m_singleCall = true;
    int m_operationCount = 10;
public:
    Application() {

        m_windowManager = unique<WindowManager>(WindowManager::Get());

        m_windowManagerErrorAttacher = {m_windowManager.get(), this, &Application::OnWindowManagerError};
        m_windowSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowSizeEvent};
        m_keyEventAttacher = {m_windowManager.get(), this, &Application::OnKeyEvent};
        m_scrollEventAttacher = {m_windowManager.get(), this, &Application::OnScrollEvent};

        m_windowID = value_or_panic(m_windowManager->CreateWindow(ivec2(1200,800), "3D scene test"), "failed to create window");
    }

    void Run() {
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        m_context = unique<GraphicsContext>(value_or_panic(GraphicsContext::MakeCurrent(window)));
        DefaultFramebuffer& defaultFramebuffer = m_context->GetDefaultFramebuffer();
        defaultFramebuffer.SetSwapWaitInterval(1);

        GraphicsSettings graphicsSettings = {
            MultisampleEnabledSetting(true),
            DepthTestEnabledSetting(true),
            CullFaceEnabledSetting(true),
            CullFaceSetting(CullFaceType::BACK),
            FrontFaceSetting(FrontFaceType::CCW),
            ClearColorSetting(0)
        };
        GraphicsSettingsApplier graphicsSettingsApplier = m_context->ApplySettings(graphicsSettings);

        ImGuiContext imguiContext(window);

        ResourceStorage resStorage;
        resStorage.InsertResFolder("engine", MORPH_ENGINE_RES);
        resStorage.InsertResFolder("app", MORPH_APP_RES);
        GraphicsProgramCompiler progCompiler(*m_context, resStorage);
        CommonResources commonResources(*m_context, resStorage, progCompiler);
        
        GraphicsTimer<5> renderTimer(*m_context);
        GraphicsTimer<5> operationTimer(*m_context);

        ShaderTransform shaderTransform;
        ShaderTransformUniforms shaderTransformUniforms(shaderTransform);
        PhongForwardUniforms phongForwardUniforms(m_envGlobalAmbient, m_cameraData.transform.pos, m_material, m_light);
        PhongForwardTextureUnits phongForwardTextureUnits(m_textures, commonResources.WhiteTexture2D());
        Uniform<vec4> lightColorUniform("u_color", m_light.specular);

        m_cameraData.transform.pos = vec3(-1, 0.75, 2);
        m_cameraData.transform.rotAxis = vec3(0, 1, 0);

        m_light.lightType = PhongLightType::POINT;
        m_light.position = vec4(0, 0, 1, 1);
        m_light.attenuationLin = 2;
        m_light.attenuationQuad = 3;

        TextureSettings textureSettings = {
            TextureMinFilterSetting(TextureMinFilter::NEAREST),
            TextureMagFilterSetting(TextureMagFilter::NEAREST),
            TextureWrap2DSetting(TextureWrapType::CLAMP_TO_EDGE)
        };
        for(usize i = 0; i < 2; i++) {
            m_pingPongFramebuffers[i] = m_context->CreateFramebuffer();
            m_pingPongTextures[i] = m_context->CreateTexture2D(window.GetSize(), TextureSizedFormat::RGBA8, textureSettings);
            m_pingPongRenderbuffers[i] = m_context->CreateRenderbuffer(window.GetSize(), TextureSizedFormat::DEPTH24_STENCIL8);
            m_pingPongFramebuffers[i].Attach(CustomFramebufferAttachment::COLOR_0, m_pingPongTextures[i]);
            m_pingPongFramebuffers[i].Attach(CustomFramebufferAttachment::DEPTH_STENCIL, m_pingPongRenderbuffers[i]);
        }
        RenderProgram singleOperationProgram = value_or_panic(progCompiler.CompileRenderProgramRes("app:SingleOperation.glsl"));
        RenderProgram multipleOperationProgram = value_or_panic(progCompiler.CompileRenderProgramRes("app:MultipleOperation.glsl"));
        TextureUnit frameDataUnit = TextureUnit::_0;
        Uniform<TextureUnit> frameDataUniform("u_frameData", frameDataUnit);
        Uniform<int> operationCountUniform("u_operationCount", m_operationCount);


        while(!window.ShouldClose())
        {
            float deltaTime = 1.0f / ImGui::GetIO().Framerate;
            float aspectRatio = (float) window.GetSize().x / (float) window.GetSize().y;
            mat4 V;
            mat4 P;

            // update
            {
                glm::quat cameraRot = glm::angleAxis(m_cameraData.transform.rotAngle, m_cameraData.transform.rotAxis);
                vec3 leftDir = glm::rotate(cameraRot, vec3(1,0,0));
                vec3 forwardDir = glm::rotate(cameraRot, vec3(0,0,-1));
                if(window.IsKeyPressed(Key::A)) {
                    m_cameraData.transform.pos -= leftDir * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::D)) {
                    m_cameraData.transform.pos += leftDir * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::S)) {
                    m_cameraData.transform.pos -= forwardDir * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::W)) {
                    m_cameraData.transform.pos += forwardDir * m_moveSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::Q)) {
                    m_cameraData.transform.rotAngle += m_rotSpeed * deltaTime;
                }
                if(window.IsKeyPressed(Key::E)) {
                    m_cameraData.transform.rotAngle -= m_rotSpeed * deltaTime;
                }

                V = m_cameraData.transform.ToInv();
                P = m_cameraData.camera.ToMat(aspectRatio);
                shaderTransform.Set(m_objectData.transform.ToMat(), V, P);
            }
            m_context->SetViewport(m_context->GetDefaultFramebuffer().dim());
            {
                FramebufferBinder framebufferBinder = m_context->BindFramebuffer(m_pingPongFramebuffers[0]);
                framebufferBinder.Clear();
                // draw iterations
                {
                    GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = renderTimer.Begin();
                    RenderProgramBinder programBinder = m_context->BindProgram(commonResources.PhongForwardProgram());
                    shaderTransformUniforms.SetUniforms(programBinder);
                    phongForwardUniforms.SetUniforms(programBinder);
                    TextureUnitsBinder textureUnitsBinder = phongForwardTextureUnits.Bind(*m_context, programBinder);
                    commonResources.CubeMesh().Draw(*m_context, framebufferBinder, programBinder);
                }
                // show light
                if(m_showLight) 
                {
                    TransformRotAngleAxis lightTrans;
                    lightTrans.pos = m_light.position;
                    lightTrans.scale = vec3(0.05);
                    shaderTransform.Set(lightTrans.ToMat(), V, P);
                    RenderProgramBinder programBinder = m_context->BindProgram(commonResources.MeshFillProgram());
                    shaderTransformUniforms.SetUniforms(programBinder);
                    lightColorUniform.Set(programBinder);
                    commonResources.CubeMesh().Draw(*m_context, framebufferBinder, programBinder);
                }
            }
            // operations
            {
                GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = operationTimer.Begin();
                if(m_singleCall || m_operationCount == 0)
                {
                    FramebufferBinder framebufferBinder = m_context->BindFramebuffer(defaultFramebuffer);
                    framebufferBinder.Clear();
                    RenderProgramBinder programBinder = m_context->BindProgram(multipleOperationProgram);
                    frameDataUniform.Set(programBinder);
                    operationCountUniform.Set(programBinder);
                    TextureUnitBinder textureUnitBinder = m_context->BindTextureUnit(frameDataUnit, m_pingPongTextures[0]);
                    commonResources.ScreenQuad2DMesh().Draw(*m_context, framebufferBinder, programBinder);
                } else {
                    for(int i = 0; i < m_operationCount; i++) {
                        FramebufferBinder framebufferBinder = m_context->BindFramebuffer(
                            i == m_operationCount - 1 ? (Framebuffer&)defaultFramebuffer : (Framebuffer&)m_pingPongFramebuffers[(i + 1) % 2]
                        );
                        framebufferBinder.Clear();
                        RenderProgramBinder programBinder = m_context->BindProgram(singleOperationProgram);
                        frameDataUniform.Set(programBinder);
                        TextureUnitBinder textureUnitBinder = m_context->BindTextureUnit(frameDataUnit, m_pingPongTextures[i % 2]);
                        commonResources.ScreenQuad2DMesh().Draw(*m_context, framebufferBinder, programBinder);
                    }
                }
            }
            m_renderTimer = (float)renderTimer.GetAvgTime() / 1000000.0f;
            m_operationTimer = (float)operationTimer.GetAvgTime() / 1000000.0f;
            // draw ui
            {
                ImGuiDraw imguiDraw = imguiContext.Draw();

                ImGui::Begin("3D scene test settings");

                if (ImGui::CollapsingHeader("Camera"))
                {
                    ImGui::DragFloat3("position##1", (float*)&m_cameraData.transform.pos, 0.025);
                    ImGui::DragFloat("rotation", &m_cameraData.transform.rotAngle, 0.005);
                }
                if (ImGui::CollapsingHeader("Light"))
                {
                    ImGui::Checkbox("show", &m_showLight);
                    ImGui::DragFloat3("position##2", (float*)&m_light.position, 0.01);
                    ImGui::ColorEdit3("ambient##1", (float*)&m_light.ambient);
                    ImGui::ColorEdit3("diffuse##1", (float*)&m_light.diffuse);
                    ImGui::ColorEdit3("specular##1", (float*)&m_light.specular);
                    ImGui::DragFloat3("attenuation", (float*)&m_light.attenuationLin, 0.005, 0, 100);
                }
                if (ImGui::CollapsingHeader("Material"))
                {
                    ImGui::ColorEdit3("ambient##2", (float*)&m_material.ambient);
                    ImGui::ColorEdit3("diffuse##2", (float*)&m_material.diffuse);
                    ImGui::ColorEdit3("specular##2", (float*)&m_material.specular);
                    ImGui::ColorEdit3("emission", (float*)&m_material.emission);
                    ImGui::DragFloat("shinines", &m_material.shinines, 0.1, 1, 100);
                }
                if (ImGui::CollapsingHeader("Post processing operation", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::Checkbox("single call (or multiple calls)", &m_singleCall);
                    static int sliderOperationCountMax = 300;
                    ImGui::InputInt("operation max", &sliderOperationCountMax);
                    ImGui::SliderInt("operation count", (int*)&m_operationCount, 0, sliderOperationCountMax);
                    ImGui::DragFloat3("position##1", (float*)&m_cameraData.transform.pos, 0.025);
                    ImGui::DragFloat("rotation", &m_cameraData.transform.rotAngle, 0.005);
                }
                if (ImGui::CollapsingHeader("Statistics", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    static bool vsync = true;
                    bool vsyncPrev = vsync;
                    ImGui::Checkbox("Vsync", &vsync);
                    if(vsync != vsyncPrev) {
                        if(vsync) {
                            defaultFramebuffer.SetSwapWaitInterval(1);
                        } else {
                            defaultFramebuffer.SetSwapWaitInterval(0);
                        }
                    }
                    ImGui::Text("gpu avg. render time %.3f ms", m_renderTimer);
                    ImGui::Text("gpu avg. operation time %.3f ms", m_operationTimer);
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
        for(int i = 0; i < 2; i++) {
            if(m_pingPongFramebuffers[i]) {
                m_pingPongFramebuffers[i].Resize(event.size);
            }
        }
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
    }
};

}

int main(int, char**) {
    Morph::Application app;
    app.Run();
}
