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
#include <Profile/GraphicsTimer.hpp>

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

    float m_renderTimer = 0;
    CameraData m_cameraData;
    ObjectData m_objectData;
    vec4 m_envGlobalAmbient = vec4(1);
    PhongMaterial m_material;
    uint m_lightCount = 1;
    PhongLight m_light;
    PhongMaterialTex m_textures;
    bool m_showLight = true;
    float m_moveSpeed = 1;
    float m_rotSpeed = 1;
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

        ShaderTransform shaderTransform;
        ShaderTransformUniforms shaderTransformUniforms(shaderTransform);
        raw<PhongLight> rawLights(1, &m_light);
        PhongForwardUniforms phongForwardUniforms(m_envGlobalAmbient, m_cameraData.transform.pos, m_material, m_lightCount, rawLights);
        PhongForwardTextureUnits phongForwardTextureUnits(m_textures, commonResources.WhiteTexture2D());
        Uniform<vec3> lightColorUniform("u_color", *(const vec3*)&m_light.specular);

        m_cameraData.camera.near = 0.01f;

        m_cameraData.transform.pos = vec3(-1, 0.75, 2);
        m_cameraData.transform.rotAxis = vec3(0, 1, 0);

        m_objectData.transform.scale = vec3(0.5);

        m_light.lightType = PhongLightType::POINT;
        m_light.position = vec4(0, 0, 1, 1);
        m_light.attenuationLin = 2;
        m_light.attenuationQuad = 3;

        FramebufferBinder framebufferBinder = m_context->BindFramebuffer(defaultFramebuffer);

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

            framebufferBinder.Clear();
            m_context->SetViewport(m_context->GetDefaultFramebuffer().dim());
            // draw iterations
            {
                GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = renderTimer.Begin();
                RenderProgramBinder programBinder = m_context->BindProgram(commonResources.PhongForwardProgram());
                shaderTransformUniforms.SetUniforms(programBinder);
                phongForwardUniforms.SetUniforms(programBinder);
                TextureUnitsBinder textureUnitsBinder = phongForwardTextureUnits.Bind(*m_context, programBinder);
                commonResources.SphereMesh().Draw(*m_context, framebufferBinder, programBinder);
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
            m_renderTimer = (float)renderTimer.GetAvgTime() / 1000000.0f;
            // draw ui
            {
                ImGuiDraw imguiDraw = imguiContext.Draw();

                ImGui::Begin("3D scene test settings");

                if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::DragFloat3("position##1", (float*)&m_cameraData.transform.pos, 0.025);
                    ImGui::DragFloat("rotation", &m_cameraData.transform.rotAngle, 0.005);
                }
                if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::ColorEdit3("ambient", (float*)&m_envGlobalAmbient);
                    static const char* ligth_types[] = {
                        "none", "dir", "point", "spot"
                    };
                    ImGui::Combo("light type", &(int&)m_light.lightType, ligth_types, IM_ARRAYSIZE(ligth_types));
                    if(m_light.lightType != PhongLightType::NONE) {
                        ImGui::Checkbox("show", &m_showLight);
                        ImGui::ColorEdit3("ambient##1", (float*)&m_light.ambient);
                        ImGui::ColorEdit3("diffuse##1", (float*)&m_light.diffuse);
                        ImGui::ColorEdit3("specular##1", (float*)&m_light.specular);
                        ImGui::DragFloat3("position##2", (float*)&m_light.position, 0.01);
                        ImGui::DragFloat3("attenuation", (float*)&m_light.attenuationConst, 0.005, 0, 100);
                        if(m_light.lightType != PhongLightType::POINT) {
                            ImGui::DragFloat3("direction", (float*)&m_light.spotDir, 0.01);
                        }
                        if(m_light.lightType == PhongLightType::SPOT) {
                            ImGui::DragFloat("exponent", &m_light.spotExponent, 0.01);
                            float angeleDeg = glm::degrees(m_light.spotCutoff);
                            if(ImGui::DragFloat("cutoff", &angeleDeg, 0.25, 0, 180)) {
                                m_light.spotCutoff = glm::radians(angeleDeg);
                            }
                        }
                    }
                }
                if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::ColorEdit3("ambient##2", (float*)&m_material.ambient);
                    ImGui::ColorEdit3("diffuse##2", (float*)&m_material.diffuse);
                    ImGui::ColorEdit3("specular##2", (float*)&m_material.specular);
                    ImGui::ColorEdit3("emission", (float*)&m_material.emission);
                    ImGui::DragFloat("shinines", &m_material.shinines, 0.1, 1, 100);
                }
                if (ImGui::CollapsingHeader("Statistics", ImGuiTreeNodeFlags_DefaultOpen))
                {
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
