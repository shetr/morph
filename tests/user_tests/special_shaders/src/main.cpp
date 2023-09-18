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
#include <Data/PBR.hpp>
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
    vec3 m_envGlobalAmbient = vec3(0.01);
    Material m_material;
    uint m_lightCount = 1;
    Light m_light;
    MaterialTex m_textures;
    bool m_showLight = true;
    float m_moveSpeed = 1;
    float m_rotSpeed = 1;

    raw<Light> rawLights;
    PBRForwardUniforms pbrForwardUniforms;
    PBRForwardTextureUnits pbrForwardTextureUnits;
    Uniform<vec3> lightColorUniform;

    int m_shaderType = 0;
public:
    Application(const WindowAppConfig& config)
        : WindowApp(config),
        m_windowManagerErrorAttacher(&windowManager(), this, &Application::OnWindowManagerError),
        m_windowSizeEventAttacher(&windowManager(), this, &Application::OnWindowSizeEvent),
        m_keyEventAttacher(&windowManager(), this, &Application::OnKeyEvent),
        m_scrollEventAttacher(&windowManager(), this, &Application::OnScrollEvent),
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
        pbrForwardUniforms(m_envGlobalAmbient, m_cameraData.transform.pos, m_material, m_lightCount, rawLights),
        pbrForwardTextureUnits(m_textures, commonResources.WhiteTexture2D()),
        lightColorUniform("u_color", m_light.color)
    {
        m_cameraData.camera.far = 2;
        m_cameraData.transform.pos = vec3(-0.48, 0, 0.7);
        m_cameraData.transform.rotAxis = vec3(0, 1, 0);
        m_cameraData.transform.rotAngle = -0.625;
        m_objectData.transform.scale = vec3(0.5);

        m_light.lightType = LightType::POINT;
        m_light.position = vec4(0, 0, 1, 1);
        m_light.attenuationLin = 2;
        m_light.attenuationQuad = 3;
    }

    void RunIter(f64 lastIterTime, f64 lastFrameTime) override {
        DefaultFramebuffer& defaultFramebuffer = context().GetDefaultFramebuffer();
        defaultFramebuffer.SetSwapWaitInterval(1);
        
        FramebufferBinder framebufferBinder = context().BindFramebuffer(defaultFramebuffer);

        float deltaTime = 1.0f / ImGui::GetIO().Framerate;
        float aspectRatio = (float) window().GetSize().x / (float) window().GetSize().y;
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

        framebufferBinder.Clear();
        context().SetViewport(context().GetDefaultFramebuffer().dim());
        // draw iterations
        if(m_shaderType == 0) {
            Render(framebufferBinder, commonResources.PBRForwardProgram());
        } else if(m_shaderType == 1) {
            Render(framebufferBinder, commonResources.MeshNormalsProgram());
        } else if(m_shaderType == 2) {
            Render(framebufferBinder, commonResources.MeshTexCoordsProgram());
        } else if(m_shaderType == 3) {
            Render(framebufferBinder, commonResources.MeshDepthProgram());
        } else if(m_shaderType == 4) {
            Render(framebufferBinder, commonResources.MeshMaskProgram());
        } else if(m_shaderType == 5) {
            Render(framebufferBinder, commonResources.MeshLightDirProgram());
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
        m_renderTimer = (float)renderTimer.GetAvgTime() / 1000000.0f;
        // draw ui
        {
            ImGuiDraw imguiDraw = imguiContext.Draw();

            ImGui::Begin("special shaders test settings");

            {
                static const char* items[] = { 
                    "PBR", "normals", "tex coords", "depth", "mask", "light dir"
                };
                ImGui::Combo("shader type", &m_shaderType, items, IM_ARRAYSIZE(items));
            }
            if (ImGui::CollapsingHeader("Camera", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::DragFloat3("position##1", (float*)&m_cameraData.transform.pos, 0.025);
                ImGui::DragFloat("rotation", &m_cameraData.transform.rotAngle, 0.005);
            }
            if (ImGui::CollapsingHeader("Light", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("show", &m_showLight);
                ImGui::DragFloat3("position##2", (float*)&m_light.position, 0.01);
                ImGui::ColorEdit3("color", (float*)&m_light.color);
                ImGui::DragFloat3("attenuation", (float*)&m_light.attenuationLin, 0.005, 0, 100);
            }
            if (ImGui::CollapsingHeader("Material", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::ColorEdit3("albedo", (float*)&m_material.albedo);
                ImGui::DragFloat("metallic", &m_material.metallic, 0.01, 0, 1);
                ImGui::DragFloat("roughness", &m_material.roughness, 0.01, 0, 1);
                ImGui::DragFloat("ao", &m_material.ao, 0.01, 0, 1);
            }
            if (ImGui::CollapsingHeader("Statistics", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("gpu avg. render time %.3f ms", m_renderTimer);
                ImGui::Text("application avg. %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            }

            ImGui::End();
        }

    }

private:
    void OnWindowManagerError(const WindowManagerError& error) {
        spdlog::error("{}: {}", error.type, error.message);
    }
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

    void Render(FramebufferBinder& framebufferBinder, RenderProgram& program) {
        GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = renderTimer.Begin();
        RenderProgramBinder programBinder = context().BindProgram(program);
        shaderTransformUniforms.SetUniforms(programBinder);
        pbrForwardUniforms.SetUniforms(programBinder);
        TextureUnitsBinder textureUnitsBinder = pbrForwardTextureUnits.Bind(context(), programBinder);
        commonResources.SphereMesh().Draw(context(), framebufferBinder, programBinder);
    }
};

}

int main(int, char**) {
    using namespace Morph;
    WindowAppConfig appConfig = {
        ivec2(1200,800),
        "special shaders test",
        ExecutionTypes::LimitedFrames(60)
    };
    Application app(appConfig);
    app.Run();
}
