#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <App/WindowApp.hpp>
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

class Application : public WindowApp
{
private:
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<WindowFramebufferSizeEvent, Application> m_windowFramebufferSizeEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;

    GraphicsSettings graphicsSettings;
    GraphicsSettingsApplier graphicsSettingsApplier;
    ImGuiContext imguiContext;
    ResourceStorage resStorage;
    GraphicsProgramCompiler progCompiler;
    CommonResources commonResources;
    GraphicsTimer<5> renderTimer;
    ShaderTransform shaderTransform;
    ShaderTransformUniforms shaderTransformUniforms;

    

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

    raw<PhongLight> rawLights;
    PhongForwardUniforms phongForwardUniforms;
    PhongForwardTextureUnits phongForwardTextureUnits;
    Uniform<vec3> lightColorUniform;

    uvec2 m_sceneTextureSize;
    CustomFramebuffer m_sceneFramebuffer;
    Texture2D m_sceneTexture;
    Renderbuffer m_sceneRenderbuffer;
    Void d_attachTexture;
    Void d_attachRenderbuffer;
public:
    Application(const WindowAppConfig& config) 
        : WindowApp(config),
        m_windowSizeEventAttacher({&windowManager(), this, &Application::OnWindowSizeEvent}),
        m_keyEventAttacher({&windowManager(), this, &Application::OnKeyEvent}),
        m_scrollEventAttacher({&windowManager(), this, &Application::OnScrollEvent}),
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
        resStorage(unord_map<string, string>({{"engine", MORPH_ENGINE_RES}, {"app", MORPH_APP_RES}})),
        progCompiler(context(), resStorage),
        commonResources(context(), resStorage, progCompiler),
        renderTimer(context()),
        shaderTransformUniforms(shaderTransform),
        rawLights(1, &m_light),
        phongForwardUniforms(m_envGlobalAmbient, m_cameraData.transform.pos, m_material, m_lightCount, rawLights),
        phongForwardTextureUnits(m_textures, commonResources.WhiteTexture2D()),
        lightColorUniform("u_color", *(const vec3*)&m_light.specular),

        m_sceneTextureSize(500, 500),
        m_sceneFramebuffer(context().CreateFramebuffer()),
        m_sceneTexture(context().CreateTexture2D(
            m_sceneTextureSize, TextureSizedFormat::RGB8, {
                TextureMinFilterSetting(TextureMinFilter::NEAREST),
                TextureMagFilterSetting(TextureMagFilter::NEAREST),
                TextureWrap2DSetting(TextureWrapType::CLAMP_TO_EDGE)
        })),
        m_sceneRenderbuffer(context().CreateRenderbuffer(m_sceneTextureSize, TextureSizedFormat::DEPTH24_STENCIL8)),
        d_attachTexture(MORPH_VOID(m_sceneFramebuffer.Attach(CustomFramebufferAttachment::COLOR_0, m_sceneTexture))),
        d_attachRenderbuffer(MORPH_VOID(m_sceneFramebuffer.Attach(CustomFramebufferAttachment::DEPTH_STENCIL, m_sceneRenderbuffer)))
    {
        m_cameraData.transform.pos = vec3(-1, 0.75, 2);
        m_cameraData.transform.rotAxis = vec3(0, 1, 0);

        m_light.lightType = PhongLightType::POINT;
        m_light.position = vec4(0, 0, 1, 1);
        m_light.attenuationLin = 2;
        m_light.attenuationQuad = 3;

        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowRounding = 0;
    }

public:

    void RunFrame(f64 lastIterTime, f64 lastFrameTime) override {
        float deltaTime = 1.0f / ImGui::GetIO().Framerate;
        float aspectRatio = (float) m_sceneTextureSize.x / (float) m_sceneTextureSize.y;
        mat4 V;
        mat4 P;

        // update
        {
            glm::quat cameraRot = glm::angleAxis(m_cameraData.transform.rotAngle, m_cameraData.transform.rotAxis);
            vec3 leftDir = glm::rotate(cameraRot, vec3(1,0,0));
            vec3 forwardDir = glm::rotate(cameraRot, vec3(0,0,-1));
            if(window().IsKeyPressed(Key::A)) {
                m_cameraData.transform.pos -= leftDir * m_moveSpeed * deltaTime;
            }
            if(window().IsKeyPressed(Key::D)) {
                m_cameraData.transform.pos += leftDir * m_moveSpeed * deltaTime;
            }
            if(window().IsKeyPressed(Key::S)) {
                m_cameraData.transform.pos -= forwardDir * m_moveSpeed * deltaTime;
            }
            if(window().IsKeyPressed(Key::W)) {
                m_cameraData.transform.pos += forwardDir * m_moveSpeed * deltaTime;
            }
            if(window().IsKeyPressed(Key::Q)) {
                m_cameraData.transform.rotAngle += m_rotSpeed * deltaTime;
            }
            if(window().IsKeyPressed(Key::E)) {
                m_cameraData.transform.rotAngle -= m_rotSpeed * deltaTime;
            }

            V = m_cameraData.transform.ToInv();
            P = m_cameraData.camera.ToMat(aspectRatio);
            shaderTransform.Set(m_objectData.transform.ToMat(), V, P);
        }

        if(m_sceneTextureSize != m_sceneFramebuffer.dim()) {
            m_sceneFramebuffer.Resize(m_sceneTextureSize);
        }
        
        {
            FramebufferBinder framebufferBinder = context().BindFramebuffer(m_sceneFramebuffer);

            framebufferBinder.Clear();
            context().SetViewport(m_sceneFramebuffer.dim());

            GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = renderTimer.Begin();
            // draw scene
            {
                RenderProgramBinder programBinder = context().BindProgram(commonResources.PhongForwardProgram());
                shaderTransformUniforms.SetUniforms(programBinder);
                phongForwardUniforms.SetUniforms(programBinder);
                TextureUnitsBinder textureUnitsBinder = phongForwardTextureUnits.Bind(context(), programBinder);
                commonResources.CubeMesh().Draw(context(), framebufferBinder, programBinder);
            }
            // show light
            if(m_showLight) 
            {
                TransformRotAngleAxis lightTrans;
                lightTrans.pos = m_light.position;
                lightTrans.scale = vec3(0.05);
                shaderTransform.Set(lightTrans.ToMat(), V, P);
                RenderProgramBinder programBinder = context().BindProgram(commonResources.MeshFillProgram());
                shaderTransformUniforms.SetUniforms(programBinder);
                lightColorUniform.Set(programBinder);
                commonResources.CubeMesh().Draw(context(), framebufferBinder, programBinder);
            }
        }
        m_renderTimer = (float)renderTimer.GetAvgTime() / 1000000.0f;
        // draw ui
        {
            DefaultFramebuffer& defaultFramebuffer = context().GetDefaultFramebuffer();
            FramebufferBinder framebufferBinder = context().BindFramebuffer(defaultFramebuffer);

            //framebufferBinder.Clear();
            context().SetViewport(context().GetDefaultFramebuffer().dim());

            ImGuiDraw imguiDraw = imguiContext.Draw();

            ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->Pos);
            ImGui::SetNextWindowSize(viewport->Size);
            ImGui::SetNextWindowViewport(viewport->ID);

            ImGui::DockSpaceOverViewport(viewport);

            ImGui::Begin("Setting", (bool*)0);
            if (ImGui::BeginTabBar("##TabBar"))
            {
                if (ImGui::BeginTabItem("Main tab")) {
                    if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::DragFloat3("position##1", (float*)&m_cameraData.transform.pos, 0.025);
                        ImGui::DragFloat("rotation", &m_cameraData.transform.rotAngle, 0.005);
                    }
                    if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::Checkbox("show", &m_showLight);
                        ImGui::DragFloat3("position##2", (float*)&m_light.position, 0.01);
                        ImGui::ColorEdit3("ambient##1", (float*)&m_light.ambient);
                        ImGui::ColorEdit3("diffuse##1", (float*)&m_light.diffuse);
                        ImGui::ColorEdit3("specular##1", (float*)&m_light.specular);
                        ImGui::DragFloat3("attenuation", (float*)&m_light.attenuationLin, 0.005, 0, 100);
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
                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Test tab")) {
                    ImGui::Text("Hello there :)");
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
            ImGui::End();

            ImGui::Begin("Scene", (bool*)0);

            ImTextureID sceneTextureId = reinterpret_cast<ImTextureID>((usize)m_sceneTexture.id());
            ImVec2 sceneTexture_p0 = ImGui::GetCursorScreenPos();      // ImDrawList API uses screen coordinates!
            ImVec2 sceneTexture_sz = ImGui::GetContentRegionAvail();   // Resize texture to what's available
            if(sceneTexture_sz.x < 50) sceneTexture_sz.x = 50;
            if(sceneTexture_sz.y < 50) sceneTexture_sz.y = 50;
            ImVec2 sceneTexture_p1 = ImVec2(sceneTexture_p0.x + sceneTexture_sz.x, sceneTexture_p0.y + sceneTexture_sz.y);

            m_sceneTextureSize = uvec2(sceneTexture_sz.x, sceneTexture_sz.y);

            ImDrawList* draw_list = ImGui::GetWindowDrawList();
            draw_list->AddImage(sceneTextureId, sceneTexture_p0, sceneTexture_p1);

            ImGui::End();
            
        }
    };

private:
    void OnWindowSizeEvent(const WindowSizeEvent& event) {
        spdlog::trace("window resize {}", event.size);
    }
    void OnKeyEvent(const KeyEvent& event) {
        spdlog::trace("key {} {}", event.key, event.action);
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
        ivec2(900,500),
        "docking imgui test",
        ExecutionTypes::AdaptiveVsyncFrames()
    };
    Application app(appConfig);
    app.Run();
}
