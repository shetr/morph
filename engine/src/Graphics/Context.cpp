#include "Context.hpp"

#include "OpenGL.hpp"

namespace Morph {

mutex GraphicsContext::s_instancesMutex;
unord_set<WindowID> GraphicsContext::s_contexts;
unord_map<WindowID, ref<GraphicsContext>> GraphicsContext::s_currentContexts;
unord_map<thread::id, WindowID> GraphicsContext::s_currentThreads;

GraphicsContext::GraphicsContext(const Window& window) : m_window(window.id()), m_defaultFramebuffer(window)
{
    m_settings = GraphicsSettings({
        ClearColorSetting(),
        ClearDepthSetting(),
        ClearStencilSetting(),
        BlendEnabledSetting(),
        CullFaceEnabledSetting(),
        DepthClampEnabledSetting(),
        DepthTestEnabledSetting(),
        StencilTestEnabledSetting(),
        FramebufferSRGB_EnabledSetting(),
        MultisampleEnabledSetting(),
        LineSmoothEnabledSetting(),
        PolygonSmoothEnabledSetting(),
        ProgramPointSizeEnabledSetting(),
        BlendFuncSetting(),
        CullFaceSetting(),
        FrontFaceSetting(),
        DephtFuncSetting(),
        DephtRangeSetting(),
        StencilFuncSetting(),
        StencilOpSetting(),
        LineWidthSetting(),
        WriteDepthSetting(),
        WriteStencilSetting(),
        PolygonModeSetting(),
        ViewportSetting()
    });

    m_settings.Set();

    m_textureUnitsBound.fill(false);
    m_imagesBound.fill(false);
    m_queryIndexScopes.fill(false);
}
GraphicsContext::~GraphicsContext()
{
    std::lock_guard<mutex> instanceLock(s_instancesMutex);
    s_currentContexts.erase(m_window);
    s_currentThreads.erase(std::this_thread::get_id());
    glfwMakeContextCurrent(nullptr);
}

Result<GraphicsContext*, MakeCurrentError> GraphicsContext::MakeCurrent(const Window& window)
{
    std::lock_guard<mutex> instanceLock(s_instancesMutex);
    if(s_currentContexts.find(window.id()) != s_currentContexts.end()) {
        return { MakeCurrentErrorType::CONTEXT_ALREADY_CURRENT };
    }
    if(s_currentThreads.find(std::this_thread::get_id()) != s_currentThreads.end()) {
        return { MakeCurrentErrorType::THREAD_ALREADY_CURRENT };
    }
    glfwMakeContextCurrent(window.id());
    if(s_contexts.find(window.id()) == s_contexts.end()) {
        if(!gladLoadGL()) {
            glfwMakeContextCurrent(nullptr);
            return {MakeCurrentErrorType::OPENGL_LOAD_FAIL};
        }
        s_contexts.insert(window.id());
        
        #ifdef MORPH_DEBUG
            spdlog::debug("OpenGL Info:");
            spdlog::debug("  Vendor: {0}", glGetString(GL_VENDOR));
            spdlog::debug("  Renderer: {0}", glGetString(GL_RENDERER));
            spdlog::debug("  Version: {0}", glGetString(GL_VERSION));
            int versionMajor;
            int versionMinor;
            GL(GetIntegerv(GL_MAJOR_VERSION, &versionMajor));
            GL(GetIntegerv(GL_MINOR_VERSION, &versionMinor));
            spdlog::debug("OpenGL version: {0}.{1}", versionMajor, versionMinor);
        #endif
    }
    GraphicsContext* res = new GraphicsContext(window);
    s_currentContexts.insert({window.id(), *res});
    s_currentThreads.insert({std::this_thread::get_id(), window.id()});
    return res;
}

opt<ref<GraphicsContext>> GraphicsContext::GetCurrent()
{
    std::lock_guard<mutex> instanceLock(s_instancesMutex);
    auto threadIt = s_currentThreads.find(std::this_thread::get_id());
    if(threadIt == s_currentThreads.end()) {
        return {};
    }
    WindowID window = threadIt->second;
    auto contextIt = s_currentContexts.find(window);
    if(contextIt == s_currentContexts.end()) {
        return {};
    }
    return contextIt->second;
}

GraphicsSettingsApplier GraphicsContext::ApplySettings(const GraphicsSettings& settings)
{
    return GraphicsSettingsApplier(settings, m_settings);
}
GraphicsSettingsApplier GraphicsContext::ApplySetting(const GraphicsSettingVar& setting)
{
    return GraphicsSettingsApplier(GraphicsSettings({setting}), m_settings);
}

Result<VertexShader, ShaderCompileError> GraphicsContext::CompileVertexShader(const string& source) {
    return CompileShader<VertexShader>(source);
}
Result<FragmentShader, ShaderCompileError> GraphicsContext::CompileFragmentShader(const string& source) {
    return CompileShader<FragmentShader>(source);
}
Result<GeometryShader, ShaderCompileError> GraphicsContext::CompileGeometryShader(const string& source) {
    return CompileShader<GeometryShader>(source);
}
Result<TessEvaluationShader, ShaderCompileError> GraphicsContext::CompileTessEvaluationShader(const string& source) {
    return CompileShader<TessEvaluationShader>(source);
}
Result<TessControlShader, ShaderCompileError> GraphicsContext::CompileTessControlShader(const string& source) {
    return CompileShader<TessControlShader>(source);
}
Result<ComputeShader, ShaderCompileError> GraphicsContext::CompileComputeShader(const string& source) {
    return CompileShader<ComputeShader>(source);
}


RenderProgramLinker GraphicsContext::MakeRenderProgramLinker() {
    return RenderProgramLinker();
}
ComputeProgramLinker GraphicsContext::MakeComputeProgramLinker(const ComputeShader& shader) {
    return ComputeProgramLinker(shader);
}

RenderProgramBinder GraphicsContext::BindProgram(RenderProgram& program)
{
    assert(!m_programBound);
    return RenderProgramBinder(program, m_programBound);
}
ComputeProgramBinder GraphicsContext::BindProgram(ComputeProgram& program)
{
    assert(!m_programBound);
    return ComputeProgramBinder(program, m_programBound);
}


VertexArray GraphicsContext::CreateVertexArray()
{
    return VertexArray(VertexArray::Gen());
}
VertexArrayBinder GraphicsContext::BindVertexArray(VertexArray& vertexArray)
{
    assert(!m_vertexArrayBound);
    return VertexArrayBinder(vertexArray, m_vertexArrayBound);
}


void GraphicsContext::CopyFramebuffer(Framebuffer& readFramebuffer, Framebuffer& writeFramebuffer, EnumBits<FramebufferBufferType> bufferTypes, bool linear)
{
    CopyFramebuffer(readFramebuffer, writeFramebuffer, bufferTypes, linear, {uvec2(0), readFramebuffer.dim()}, {uvec2(0), writeFramebuffer.dim()});
}
void GraphicsContext::CopyFramebuffer(Framebuffer& readFramebuffer, Framebuffer& writeFramebuffer, EnumBits<FramebufferBufferType> bufferTypes, bool linear, Rectangle readRect, Rectangle writeRect)
{
    GL(BindFramebuffer(GL_READ_FRAMEBUFFER, readFramebuffer.id()));
    GL(BindFramebuffer(GL_DRAW_FRAMEBUFFER, writeFramebuffer.id()));
    GL(BlitFramebuffer(
        readRect.from.x, readRect.from.y, readRect.from.x + readRect.size.x, readRect.from.y + readRect.size.y,
        writeRect.from.x, writeRect.from.y, writeRect.from.x + writeRect.size.x, writeRect.from.y + writeRect.size.y,
        bufferTypes.value(), 
        linear ? GL_LINEAR : GL_NEAREST));
    GL(BindFramebuffer(GL_READ_FRAMEBUFFER, 0));
    GL(BindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));
}

void GraphicsContext::SetViewport(uvec2 size)
{
    SetViewport(uvec2(0), size);
}
void GraphicsContext::SetViewport(uvec2 offset, uvec2 size)
{
    GL(Viewport(offset.x, offset.y, size.x, size.y));
}

void GraphicsContext::DispatchCompute(const ComputeProgramBinder& programBinder, uvec3 numGroups)
{
    GL(DispatchCompute(numGroups.x, numGroups.y, numGroups.z));
}

void GraphicsContext::Finish()
{
    GL(Finish());
}
void GraphicsContext::TextureBarrier()
{
    GL(TextureBarrier());
}
void GraphicsContext::MemoryBarrier(EnumBits<MemoryBarrierType> types)
{
    GL(MemoryBarrier(types.value()));
}

}