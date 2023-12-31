#include "App.hpp"

namespace Morph {

Application::Application(const WindowAppConfig& config)
    : WindowApp(config),
    m_windowManagerErrorAttacher(&windowManager(), this, &Application::OnWindowManagerError),
    m_windowSizeEventAttacher(&windowManager(), this, &Application::OnWindowSizeEvent),
    m_keyEventAttacher(&windowManager(), this, &Application::OnKeyEvent),
    m_scrollEventAttacher(&windowManager(), this, &Application::OnScrollEvent),
    m_scene(m_jobManager),
    m_graphicsSettings({
        MultisampleEnabledSetting(true),
        DepthTestEnabledSetting(true),
        CullFaceEnabledSetting(true),
        CullFaceSetting(CullFaceType::BACK),
        FrontFaceSetting(FrontFaceType::CCW),
        ClearColorSetting(0)
    }),
    m_graphicsSettingsApplier(context().ApplySettings(m_graphicsSettings)),
    m_resStorage(unord_map<string, string>({{"engine", MORPH_ENGINE_RES}, {"app", MORPH_APP_RES}})),
    m_progCompiler(context(), m_resStorage),
    m_commonResources(context(), m_resStorage, m_progCompiler),
    m_screenTextureSettings({
        TextureMinFilterSetting(TextureMinFilter::NEAREST),
        TextureMagFilterSetting(TextureMagFilter::NEAREST),
        TextureWrap2DSetting(TextureWrapType::CLAMP_TO_EDGE)
    }),
    m_screenTexture(context().CreateTexture2D(window().GetSize(), TextureSizedFormat::RGB32F, m_screenTextureSettings)),
    m_screenTextureSamplerUnit(TextureUnit::_0),
    m_screenTextureSamplerUniform("u_textureSampler", m_screenTextureSamplerUnit)
{
    uvec2 winSize = window().GetSize();
    Globals::resize_image(winSize);
    Globals::init();
    m_scene.build();
    Globals::method = LIGHT_SOURCE;
    Usage();
}

void Application::RunFrame(f64 lastIterTime, f64 lastFrameTime)
{
    m_scene.renderIteration();
    m_screenTexture.Update(Globals::ldrImage.data());
    FramebufferBinder framebufferBinder = context().BindFramebuffer(context().GetDefaultFramebuffer());
    framebufferBinder.Clear();
    context().SetViewport(context().GetDefaultFramebuffer().dim());
    RenderProgramBinder programBinder = context().BindProgram(m_commonResources.ScreenFillProgram());
    TextureUnitBinder textureUnitBinder = context().BindTextureUnit(m_screenTextureSamplerUnit, m_screenTexture);
    m_screenTextureSamplerUniform.Set(programBinder);
    m_commonResources.ScreenQuad2DMesh().Draw(context(), framebufferBinder, programBinder);
}

void Application::OnWindowManagerError(const WindowManagerError& error)
{
    spdlog::error("{}: {}", enum_name(error.type), error.message);
}

void Application::OnWindowSizeEvent(const WindowSizeEvent& event)
{
    spdlog::trace("window resize {}", event.size);
}

void Application::OnKeyEvent(const KeyEvent& event)
{
    spdlog::trace("key {} {}", enum_name(event.key), enum_name(event.action));
    if(event.action == KeyAction::PRESS) {
        if(event.key == Key::O) {
            spdlog::set_level(spdlog::level::trace);
        }
        if(event.key == Key::P) {
            spdlog::set_level(spdlog::level::info);
        }
        switch (event.key)
        {
        case Key::L:
            Globals::method = LIGHT_SOURCE;
            Globals::weight = 1;
            printf("Light source sampling\n");
            //m_scene.render();
            Globals::clear();
            break;
        case Key::B:
            Globals::method = BRDF;
            Globals::weight = 0;
            printf("BRDF sampling\n");
            //m_scene.render();
            Globals::clear();
            break;
        case Key::H:
            Globals::method = HALF_WEIGHT;
            Globals::weight = 0.5;
            printf("half weight sampling\n");
            //m_scene.render();
            Globals::clear();
            break;
        case Key::M:
            Globals::method = MULTIPLE_IMPORTANCE;
            Globals::weight = 0.5;
            printf("Multiple importance sampling\n");
            //m_scene.render();
            Globals::clear();
            break;
        case Key::P:
            Globals::method = PATH_TRACING;
            Globals::weight = 1;
            printf("Path tracing\n");
            //m_scene.render();
            Globals::clear();
            break;
        case Key::W:
        {
            printf("Writing reference file\n");
            FILE *refImage = fopen("image.bin", "wb");
            if (refImage)
            {
            fwrite(Globals::hdrImage.data(), sizeof(vec3), Globals::screenSize.x * Globals::screenSize.y, refImage);
            fclose(refImage);
            }
        }
        case Key::O:
        {
            printf("Writing output HDR file (extension .hdr)\n");
            FILE *fp;
            const char* hdrFilename = "test.hdr";
            const char* tgaFilename = "test.tga";
            switch (Globals::method)
            {
                case LIGHT_SOURCE:
                hdrFilename = "lightsource.hdr";
                tgaFilename = "lightsource.tga";
                break;
                case BRDF:
                hdrFilename = "brdf.hdr";
                tgaFilename = "brdf.tga";
                break;
                case HALF_WEIGHT:
                hdrFilename = "half_weight.hdr";
                tgaFilename = "half_weight.tga";
                break;
                case MULTIPLE_IMPORTANCE:
                hdrFilename = "multiple_importance.hdr";
                tgaFilename = "multiple_importance.tga";
                break;
                case PATH_TRACING:
                hdrFilename = "path_tracing.hdr";
                tgaFilename = "path_tracing.tga";
                break;
            }

            SaveHDR(hdrFilename, Globals::hdrImage);
            SaveTGA(tgaFilename, Globals::ldrImage);
            break;
        }
        } // switch (key)
    }
}

void Application::OnScrollEvent(const ScrollEvent& event)
{
    spdlog::trace("scroll {}", event.offset);
}

void Application::Usage()
{
    printf("Usage:\n");
    printf(" 'b': BRDF sampling \n");
    printf(" 'l': light source sampling \n");
    printf(" 'm': multiple importance sampling \n");
    printf(" 'p': path tracing \n");
    printf(" 't': testing \n");
    printf(" 'g': generate multiple images \n");
    printf(" 'r': Show reference\n");
    printf(" 'w': Print current as a ground truth reference file for the future\n");
    printf(" 'o': Write output HDR file of rendered image\n\n");
    printf(" 'O': Write output HDR file render+pseudocolor visualization\n\n");
}

}