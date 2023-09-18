#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <App/WindowApp.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>
#include <Resource/GraphicsProgramCompiler.hpp>
#include <Resource/Common.hpp>
#include <Resource/ResourceManager.hpp>
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

    IndexedVerticesMesh3D<u32> m_modelMeshData;
    IndexedStaticMesh3D<u32> m_modelMesh;
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
        m_modelMeshData(
            value_or_panic(ResourceManager::LoadMesh3D_OBJ(
            value_or_panic(resStorage.GetPath("app:bunny.obj"), 
            "resource path app:bunny.obj does not exist")), 
            "failed to open file app:bunny.obj")
        ),
        m_modelMesh(context(), m_modelMeshData)
    {
        m_cameraData.transform.pos = vec3(-1, 0.75, 2);
        m_cameraData.transform.rotAxis = vec3(0, 1, 0);

        m_light.lightType = PhongLightType::POINT;
        m_light.position = vec4(0, 0, 1, 1);
        m_light.attenuationLin = 2;
        m_light.attenuationQuad = 3;

        m_objectData.transform.scale = vec3(1);
    }

public:

    void RunIter(f64 lastIterTime, f64 lastFrameTime) override {
        DefaultFramebuffer& defaultFramebuffer = context().GetDefaultFramebuffer();
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
        {
            GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED> timeQueryScope = renderTimer.Begin();
            RenderProgramBinder programBinder = context().BindProgram(commonResources.PhongForwardProgram());
            shaderTransformUniforms.SetUniforms(programBinder);
            phongForwardUniforms.SetUniforms(programBinder);
            TextureUnitsBinder textureUnitsBinder = phongForwardTextureUnits.Bind(context(), programBinder);
            m_modelMesh.Draw(context(), framebufferBinder, programBinder);
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

            ImGui::Begin("3D scene test settings");

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
        ivec2(1200,800),
        "model loading test",
        ExecutionTypes::LimitedFrames(60)
    };
    Application app(appConfig);
    app.Run();
}
