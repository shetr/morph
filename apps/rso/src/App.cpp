#include "App.hpp"

namespace Morph {

Application::Application(const WindowAppConfig& config)
    : WindowApp(config),
    m_windowManagerErrorAttacher(&windowManager(), this, &Application::OnWindowManagerError),
    m_windowSizeEventAttacher(&windowManager(), this, &Application::OnWindowSizeEvent),
    m_keyEventAttacher(&windowManager(), this, &Application::OnKeyEvent),
    m_scrollEventAttacher(&windowManager(), this, &Application::OnScrollEvent),
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
    Globals::resize_image(winSize.x, winSize.y);
    m_scene.build();
    Globals::method = LIGHT_SOURCE;
    Usage();
}

void Application::RunFrame(f64 lastIterTime, f64 lastFrameTime)
{
    m_screenTexture.Update(Globals::image.data());
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
            printf("Light source sampling\n");
            m_scene.render();
            break;
        case Key::B:
            Globals::method = BRDF;
            printf("BRDF sampling\n");
            m_scene.render();
            break;
        case Key::H:
            Globals::method = HALF_WEIGHT;
            printf("half weight sampling\n");
            m_scene.render();
            break;
        case Key::M:
            Globals::method = MULTIPLE_IMPORTANCE;
            printf("Multiple importance sampling\n");
            m_scene.render();
            break;
        case Key::P:
            Globals::method = PATH_TRACING;
            printf("Path tracing\n");
            m_scene.render();
            break;
        case Key::W:
        {
            printf("Writing reference file\n");
            FILE *refImage = fopen("image.bin", "wb");
            if (refImage)
            {
            fwrite(Globals::image.data(), sizeof(vec3), Globals::screenWidth * Globals::screenHeight, refImage);
            fclose(refImage);
            }
        }
        case Key::O:
        {
            printf("Writing output HDR file (extension .hdr)\n");
            FILE *fp;
            const char* filename = "test.hdr";
            switch (Globals::method)
            {
                case LIGHT_SOURCE:
                filename = "lightsource.hdr";
                break;
                case BRDF:
                filename = "brdf.hdr";
                break;
                case HALF_WEIGHT:
                filename = "half_weight.hdr";
                break;
                case MULTIPLE_IMPORTANCE:
                filename = "multiple_importance.hdr";
                break;
                case PATH_TRACING:
                filename = "path_tracing.hdr";
                break;
            }
            bool psf = false;
            
            if (event.ModKeyPressed(ModKey::SHIFT)) {
                psf = true;
            }
            SaveHDR(filename, Globals::image.data(), Globals::screenWidth, Globals::screenHeight, psf);
            break;
        }
        case Key::T:
        {
            printf("testing things\n");
            //Image image = ReadHDR("raw001.hdr");
            //SaveHDR("test.hdr", image.data.data(), image.width, image.height, false);
            //GenerateSimpleHDR();
            SaveHDR("test.hdr", Globals::testImage.data(), Globals::screenWidth, Globals::screenHeight, false);
            break;
        }
        case Key::G:
        {
            printf("generate for multiple instances\n");
            std::vector<std::string> hdrFiles = {
            "raw015",
            "raw024",
            "raw023",
            "raw020",
            //"raw004",
            "raw007",
            //"raw010",
            //"raw011",
            "raw013",
            "raw001",
            "raw016"
            };
            std::vector<Method> methods = { LIGHT_SOURCE, BRDF, MULTIPLE_IMPORTANCE };
            std::vector<std::string> methodNames = { "lightsource", "brdf", "multiple_importance" };
            int oldSamples = Globals::nTotalSamples;
            int samples = 12000;
            Globals::nTotalSamples = samples;
            std::string samplesString = std::to_string(samples);
            for (const std::string& inHdr : hdrFiles)
            {
            std::string inFile = inHdr + ".hdr";
            m_scene.build(inFile.c_str());
            for (int m = 0; m < (int)methods.size(); ++m)
            {
                std::string outFile = inHdr + "_" + methodNames[m] + samplesString + ".hdr";
                Globals::method = methods[m];
                m_scene.render();
                SaveHDR(outFile.c_str(), Globals::image.data(), Globals::screenWidth, Globals::screenHeight, false);
            }
            }
            Globals::nTotalSamples = oldSamples;
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