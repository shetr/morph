#include <iostream>

#include <spdlog/spdlog.h>

#include <Morph.hpp>
#include <Window/Manager.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>

using namespace magic_enum::ostream_operators;

namespace Morph {

string g_vertSrc = R"(
    #version 330 core
			
    layout(location = 0) in vec2 a_position;
    layout(location = 1) in vec2 a_offset;

    void main()
    {
        gl_Position = vec4(a_position + a_offset, 0, 1.0);	
    }
)";
string g_fragSrc = R"(
    #version 330 core
			
    layout(location = 0) out vec4 color;
    
    uniform vec3 u_color = vec3(0, 0, 0);

    void main()
    {
        color = vec4(u_color, 1.0);
    }
)";

string g_vertSrc2 = R"(
    #version 330 core
			
    layout(location = 0) in vec2 a_position;
    layout(location = 1) in vec3 a_color;
    layout(location = 2) in vec2 a_uv;

    out vec3 f_color;
    out vec2 f_uv;

    void main()
    {
        f_color = a_color;
        f_uv = a_uv;
        gl_Position = vec4(a_position, 0, 1.0);	
    }
)";
string g_fragSrc2 = R"(
    #version 330 core
			
    layout(location = 0) out vec4 color;

    uniform sampler2D u_textureSampler;

    in vec3 f_color;
    in vec2 f_uv;
    
    void main()
    {
        //color = vec4(f_color, 1.0);
        color = texture2D(u_textureSampler, f_uv);
    }
)";

string g_vertSrc3 = R"(
    #version 330 core
			
    layout(location = 0) in vec2 a_position;
    layout(location = 1) in vec2 a_uv;

    out vec2 f_uv;

    void main()
    {
        f_uv = a_uv;
        gl_Position = vec4(a_position, 0, 1.0);	
    }
)";
string g_fragSrc3 = R"(
    #version 330 core
			
    layout(location = 0) out vec4 color;

    uniform sampler2D u_textureSampler;

    in vec2 f_uv;
    
    void main()
    {
        color = vec4(vec3(1) - texture2D(u_textureSampler, f_uv).xyz, 1);
    }
)";

array<int,4> g_testArray = { 0, 1, 2, 3 };

array<vec2,4> g_vertices = { 
    vec2(-0.2, -0.2),
    vec2( 0.2, -0.2),
    vec2( 0.2,  0.2),
    vec2(-0.2,  0.2)
};

array<vec2,4> g_unitSqareVerts = { 
    vec2(-1, -1),
    vec2( 1, -1),
    vec2( 1,  1),
    vec2(-1,  1)
};

array<vec3,4> g_colors = { 
    vec3(0.1, 0.3, 0.5),
    vec3(0.7, 0.5, 0.1),
    vec3(0.9, 0.5, 0.7),
    vec3(0.5, 0.3, 0.9)
};


array<vec2,4> g_uvs = { 
    vec2(0, 0),
    vec2(1, 0),
    vec2(1, 1),
    vec2(0, 1)
};

array<vec2,4> g_positions = { 
    vec2(-0.5, -0.5),
    vec2(0.5, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
};

array<u32,6> g_indices = {
    0, 1, 2,
    0, 2, 3
};

vec3 g_flatColor = vec3(0.2, 0.8, 0.3);


array<u8,16> g_textureData = { 
    0, 43, 87, 127,
    43, 87, 127, 170,
    87, 127, 170, 214,
    127, 170, 214, 255
};


class Application
{
private:
    unique<WindowManager> m_windowManager;
    MethodAttacher<WindowManagerError, Application> m_windowManagerErrorAttacher;
    MethodAttacher<MonitorEvent, Application> m_monitorEventAttacher;
    MethodAttacher<WindowCloseEvent, Application> m_windowCloseEventAttacher;
    MethodAttacher<WindowSizeEvent, Application> m_windowSizeEventAttacher;
    MethodAttacher<WindowFramebufferSizeEvent, Application> m_windowFramebufferSizeEventAttacher;
    MethodAttacher<WindowContentScaleEvent, Application> m_windowContentScaleEventAttacher;
    MethodAttacher<WindowMinimizeEvent, Application> m_windowMinimizeEventAttacher;
    MethodAttacher<WindowMaximizeEvent, Application> m_windowMaximizeEventAttacher;
    MethodAttacher<WindowRestoreEvent, Application> m_windowRestoreEventAttacher;
    MethodAttacher<WindowFocusEvent, Application> m_windowFocusEventAttacher;
    MethodAttacher<KeyEvent, Application> m_keyEventAttacher;
    MethodAttacher<CharEvent, Application> m_charEventAttacher;
    MethodAttacher<CursorPosEvent, Application> m_cursorPosEventAttacher;
    MethodAttacher<CursorEnterEvent, Application> m_cursorEnterEventAttacher;
    MethodAttacher<MouseButtonEvent, Application> m_couseButtonEventAttacher;
    MethodAttacher<ScrollEvent, Application> m_scrollEventAttacher;
    MethodAttacher<WindowDropEvent, Application> m_windowDropEventAttacher;
    WindowID m_windowID;
    unique<GraphicsContext> context;

    VertexLayout g_verticesLayout = {
        { VertexType::FLOAT32, VertexElementCount::_2 }
    };
    VertexLayout g_positionsLayout = {
        { VertexType::FLOAT32, VertexElementCount::_2, 1u }
    };
    VertexLayout g_colorsLayout = {
        { VertexType::FLOAT32, VertexElementCount::_3}
    };
    VertexLayout g_uvsLayout = {
        { VertexType::FLOAT32, VertexElementCount::_2}
    };

    CustomFramebuffer offscreenFramebuffer;
public:
    Application() {

        m_windowManager = unique<WindowManager>(WindowManager::Get());

        m_windowManagerErrorAttacher = {m_windowManager.get(), this, &Application::OnWindowManagerError};
        m_monitorEventAttacher = {m_windowManager.get(), this, &Application::OnMonitorEvent};
        m_windowCloseEventAttacher = {m_windowManager.get(), this, &Application::OnWindowCloseEvent};
        m_windowSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowSizeEvent};
        m_windowFramebufferSizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowFramebufferSizeEvent};
        m_windowContentScaleEventAttacher = {m_windowManager.get(), this, &Application::OnWindowContentScaleEvent};
        m_windowMinimizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowMinimizeEvent};
        m_windowMaximizeEventAttacher = {m_windowManager.get(), this, &Application::OnWindowMaximizeEvent};
        m_windowRestoreEventAttacher = {m_windowManager.get(), this, &Application::OnWindowRestoreEvent};
        m_windowFocusEventAttacher = {m_windowManager.get(), this, &Application::OnWindowFocusEvent};
        m_keyEventAttacher = {m_windowManager.get(), this, &Application::OnKeyEvent};
        m_charEventAttacher = {m_windowManager.get(), this, &Application::OnCharEvent};
        m_cursorPosEventAttacher = {m_windowManager.get(), this, &Application::OnCursorPosEvent};
        m_cursorEnterEventAttacher = {m_windowManager.get(), this, &Application::OnCursorEnterEvent};
        m_couseButtonEventAttacher = {m_windowManager.get(), this, &Application::OnMouseButtonEvent};
        m_scrollEventAttacher = {m_windowManager.get(), this, &Application::OnScrollEvent};
        m_windowDropEventAttacher = {m_windowManager.get(), this, &Application::OnWindowDropEvent};

        m_windowID = value_or_panic(m_windowManager->CreateWindow(ivec2(400,400), "sandbox"), "failed to create window");
    }

    void Run() {
        Window& window = value_or_panic(m_windowManager->GetWindow(m_windowID), "window unexpectedly destroyed");
        context = unique<GraphicsContext>(value_or_panic(GraphicsContext::MakeCurrent(window)));
        DefaultFramebuffer& defaultFramebuffer = context->GetDefaultFramebuffer();
        defaultFramebuffer.SetSwapWaitInterval(1);
        GraphicsSettings graphicsSettings = {
            ClearColorSetting(vec3(0.2, 0.3, 0.8))
        };
        GraphicsSettingsApplier graphicsSettingsApplier = context->ApplySettings(graphicsSettings);

        VertexShader vertShader = value_or_panic(context->CompileVertexShader(g_vertSrc));
        FragmentShader fragShader = value_or_panic(context->CompileFragmentShader(g_fragSrc));
        RenderProgramLinker programLinker = context->MakeRenderProgramLinker();
        programLinker.Add(vertShader);
        programLinker.Add(fragShader);
        RenderProgram program = value_or_panic(programLinker.Link());

        VertexShader vertShader2 = value_or_panic(context->CompileVertexShader(g_vertSrc2));
        FragmentShader fragShader2 = value_or_panic(context->CompileFragmentShader(g_fragSrc2));
        RenderProgramLinker programLinker2 = context->MakeRenderProgramLinker();
        programLinker2.Add(vertShader2);
        programLinker2.Add(fragShader2);
        RenderProgram program2 = value_or_panic(programLinker2.Link());

        VertexShader vertShader3 = value_or_panic(context->CompileVertexShader(g_vertSrc3));
        FragmentShader fragShader3 = value_or_panic(context->CompileFragmentShader(g_fragSrc3));
        RenderProgramLinker programLinker3 = context->MakeRenderProgramLinker();
        programLinker3.Add(vertShader3);
        programLinker3.Add(fragShader3);
        RenderProgram program3 = value_or_panic(programLinker3.Link());

        ArrayBufferTyped<int,true,true> arrayBuffer1 = context->CreateBufferTyped<int, GraphicsBufferTarget::ARRAY, true, true>(4);
        ArrayBufferTyped<int,true> arrayBuffer2 = context->CreateBufferTyped<int, GraphicsBufferTarget::ARRAY, true>(4);
        {
            arrayBuffer1.SetData(0, 4, g_testArray.data());
            context->CopyBufferTyped(arrayBuffer1, arrayBuffer2, 0, 0, arrayBuffer1.count());
            spdlog::info("buffer count = {}", arrayBuffer1.count());
            spdlog::info("buffer size = {}", arrayBuffer1.size());
            GraphicsBufferMapper<GraphicsBufferTarget::ARRAY,1,1,0> mapper = context->MapBuffer(arrayBuffer1);
            int* begin = (int*)mapper.GetDataRaw();
            int* end = begin + 4;
            IterableMaker<int*> iterable(begin, end);
            spdlog::info("test buffer data = {}", iterable);
        }

        VertexBuffer<vec2> vertexBuffer = context->CreateVertexBuffer(g_vertices, g_verticesLayout);
        VertexBuffer<vec2> unitSqareVertsBuffer = context->CreateVertexBuffer(g_unitSqareVerts, g_verticesLayout);
        VertexBuffer<vec2> positionsBuffer = context->CreateVertexBuffer(g_positions, g_positionsLayout);
        VertexBuffer<vec3> colorsBuffer = context->CreateVertexBuffer(g_colors, g_colorsLayout);
        VertexBuffer<vec2> uvsBuffer = context->CreateVertexBuffer(g_uvs, g_uvsLayout);
        IndexBuffer indexBuffer = context->CreateIndexBuffer(g_indices);
        IndexedVertexArray vertexArray = context->CreateIndexedVertexArray(indexBuffer);
        {
            IndexedVertexArrayBinder vertexArrayBinder = context->BindIndexedVertexArray(vertexArray);
            vertexArrayBinder.AddVertexBuffer(vertexBuffer);
            vertexArrayBinder.AddVertexBuffer(positionsBuffer);
        }
        IndexedVertexArray vertexArray2 = context->CreateIndexedVertexArray(indexBuffer);
        {
            IndexedVertexArrayBinder vertexArrayBinder2 = context->BindIndexedVertexArray(vertexArray2);
            vertexArrayBinder2.AddVertexBuffer(vertexBuffer);
            vertexArrayBinder2.AddVertexBuffer(colorsBuffer);
            vertexArrayBinder2.AddVertexBuffer(uvsBuffer);
        }
        IndexedVertexArray vertexArray3 = context->CreateIndexedVertexArray(indexBuffer);
        {
            IndexedVertexArrayBinder vertexArrayBinder3 = context->BindIndexedVertexArray(vertexArray3);
            vertexArrayBinder3.AddVertexBuffer(unitSqareVertsBuffer);
            vertexArrayBinder3.AddVertexBuffer(uvsBuffer);
        }

        TextureSettings textureSettings = {
            TextureMinFilterSetting(TextureMinFilter::NEAREST),
            TextureMagFilterSetting(TextureMagFilter::NEAREST)
        };
        Texture2D testTexture = context->CreateTexture2D(uvec2(4, 4), g_textureData.data(), TextureSizedFormat::R8, textureSettings);

        {
            array<u8,16> testReadData = {0};
            testTexture.GetData(testReadData.data());
            array<uint,16> testReadDataUint = {0};
            std::copy(testReadData.begin(), testReadData.end(), testReadDataUint.begin());
            spdlog::info("texture data = {}", testReadDataUint);
        }

        Uniform<vec3> colorUniform("u_color", g_flatColor);
        TextureUnit textureSamplerUnit = TextureUnit::_0;
        Uniform<TextureUnit> textureSamplerUniform("u_textureSampler", textureSamplerUnit);

        TextureUnit offTextureSamplerUnit = TextureUnit::_0;
        Uniform<TextureUnit> offTextureSamplerUniform("u_textureSampler", offTextureSamplerUnit);

        // offscreen framebuffer

        offscreenFramebuffer = context->CreateFramebuffer();
        Texture2D offscreenTexture = context->CreateTexture2D(window.GetSize(), TextureSizedFormat::RGBA8, textureSettings);
        Renderbuffer offscreenRenderbuffer = context->CreateRenderbuffer(window.GetSize(), TextureSizedFormat::DEPTH24_STENCIL8);
        offscreenFramebuffer.Attach(CustomFramebufferAttachment::COLOR_0, offscreenTexture);
        offscreenFramebuffer.Attach(CustomFramebufferAttachment::DEPTH_STENCIL, offscreenRenderbuffer);

        while(!window.ShouldClose())
        {
            {
                FramebufferBinder framebufferBinder = context->BindFramebuffer(offscreenFramebuffer);
                framebufferBinder.ClearColor();
                {
                    RenderProgramBinder programBinder = context->BindProgram(program2);
                    IndexedVertexArrayBinder vertexArrayBinder = context->BindIndexedVertexArray(vertexArray2);
                    textureSamplerUniform.Set(programBinder);
                    TextureUnitBinder textureUnitBinder = context->BindTextureUnit(textureSamplerUnit, testTexture);
                    framebufferBinder.DrawElements(PrimitiveType::TRIANGLES, programBinder, vertexArrayBinder, 6);
                }
                {
                    RenderProgramBinder programBinder = context->BindProgram(program);
                    IndexedVertexArrayBinder vertexArrayBinder = context->BindIndexedVertexArray(vertexArray);
                    colorUniform.Set(programBinder);
                    framebufferBinder.DrawElementsInstanced(PrimitiveType::TRIANGLES, programBinder, vertexArrayBinder, 6, 4);
                }
            }
            {
                FramebufferBinder framebufferBinder = context->BindFramebuffer(defaultFramebuffer);
                RenderProgramBinder programBinder = context->BindProgram(program3);
                IndexedVertexArrayBinder vertexArrayBinder = context->BindIndexedVertexArray(vertexArray3);
                offTextureSamplerUniform.Set(programBinder);
                TextureUnitBinder textureUnitBinder = context->BindTextureUnit(offTextureSamplerUnit, offscreenTexture);
                framebufferBinder.DrawElements(PrimitiveType::TRIANGLES, programBinder, vertexArrayBinder, 6);
            }
            defaultFramebuffer.SwapBuffers();
            m_windowManager->PollEvents();
        }

    };

private:
    void OnWindowManagerError(const WindowManagerError& error) {
        spdlog::error("{}: {}", error.type, error.message);
    }
    void OnMonitorEvent(const MonitorEvent& event) {
        spdlog::trace("monitor {} {}", (void*)event.monitor, event.state == MonitorState::CONNECTED ? "connected" : "disconnected");
    }
    void OnWindowCloseEvent(const WindowCloseEvent& event) {
        spdlog::trace("closing window");
    }
    void OnWindowSizeEvent(const WindowSizeEvent& event) {
        spdlog::trace("window resize {}", event.size);
        context->GetDefaultFramebuffer().Resize(event.size);
        offscreenFramebuffer.Resize(event.size);
    }
    void OnWindowFramebufferSizeEvent(const WindowFramebufferSizeEvent& event) {
        spdlog::trace("framebuffer resize {}", event.size);
    }
    void OnWindowContentScaleEvent(const WindowContentScaleEvent& event) {
        spdlog::trace("content scale {}", event.scale);
    }
    void OnWindowMinimizeEvent(const WindowMinimizeEvent& event) {
        spdlog::trace(event.minimized ? "window minimized" : "window restored from minimization");
    }
    void OnWindowMaximizeEvent(const WindowMaximizeEvent& event) {
        spdlog::trace(event.maximized ? "window maximized" : "window restored from maximization");
    }
    void OnWindowRestoreEvent(const WindowRestoreEvent& event) {
        spdlog::trace("window restored");
    }
    void OnWindowFocusEvent(const WindowFocusEvent& event) {
        spdlog::trace(event.focused ? "window focused" : "window unfocused");
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
    void OnCharEvent(const CharEvent& event) {
        spdlog::trace("char codepoint {}", event.codepoint);
    }
    void OnCursorPosEvent(const CursorPosEvent& event) {
        spdlog::trace("cursor screen pos {} eye pos {} eye dir {}", event.screenPos, event.eyePos, event.eyeDir);
    }
    void OnCursorEnterEvent(const CursorEnterEvent& event) {
        spdlog::trace(event.entered ? "cursor entered window" : "cursor exited window");
    }
    void OnMouseButtonEvent(const MouseButtonEvent& event) {
        spdlog::trace("mouse button {} {}", event.button, event.pressed ? "pressed" : "released");
    }
    void OnScrollEvent(const ScrollEvent& event) {
        spdlog::trace("scroll {}", event.offset);
    }
    void OnWindowDropEvent(const WindowDropEvent& event) {
        spdlog::trace("path drop");
    }
};

}

int main(int, char**) {
    Morph::Application app;
    app.Run();
}
