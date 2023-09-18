
#ifndef MORPH_GRAPHICS_CONTEXT_HPP
#define MORPH_GRAPHICS_CONTEXT_HPP

#include <Core/Core.hpp>
#include <Core/Identification.hpp>
#include <Window/Window.hpp>

#include "Enums.hpp"
#include "Settings.hpp"
#include "Shader.hpp"
#include "Program.hpp"
#include "Buffer.hpp"
#include "VertexArray.hpp"
#include "Texture.hpp"
#include "Renderbuffer.hpp"
#include "Framebuffer.hpp"
#include "Query.hpp"

namespace Morph {

class WindowManager;

class MakeCurrentError
{
private:
    MakeCurrentErrorType m_errorType;
public:
    MakeCurrentError(MakeCurrentErrorType errorType) : m_errorType(errorType) {}
    MakeCurrentErrorType errorType() { return m_errorType; }
    inline friend std::ostream& operator<<(std::ostream& os, const MakeCurrentError& error) {
        os << "failed to create graphics context: " << enum_name(error.m_errorType);
        return os;
    }
};

class GraphicsContext
{
    friend class WindowManager;
private:
    static mutex s_instancesMutex;
    static unord_set<WindowID> s_contexts;
    static unord_map<WindowID, ref<GraphicsContext>> s_currentContexts;
    static unord_map<thread::id, WindowID> s_currentThreads;
    WindowID m_window;
    GraphicsSettings m_settings;
    bool m_programBound = false;
    array<bool, enum_count<TextureUnit>()> m_textureUnitsBound;
    array<bool, enum_count<TextureUnit>()> m_imagesBound;
    bool m_vertexArrayBound = false;
    DefaultFramebuffer m_defaultFramebuffer;
    bool m_framebufferBound = false;
    array<bool, enum_count<GraphicsQueryType>()> m_queryIndexScopes;
    array<unord_map<uint, bool>, enum_count<GraphicsBufferTarget>()> m_buffersBound;
private:
    GraphicsContext(const Window& window);
public:
    ~GraphicsContext();
    static Result<GraphicsContext*, MakeCurrentError> MakeCurrent(const Window& window);
    static opt<ref<GraphicsContext>> GetCurrent();

    inline WindowID GetWindow() const { return m_window; }

    GraphicsSettingsApplier ApplySettings(const GraphicsSettings& settings);
    GraphicsSettingsApplier ApplySetting(const GraphicsSettingVar& setting);

    Result<VertexShader, ShaderCompileError> CompileVertexShader(const string& source);
    Result<FragmentShader, ShaderCompileError> CompileFragmentShader(const string& source);
    Result<GeometryShader, ShaderCompileError> CompileGeometryShader(const string& source);
    Result<TessEvaluationShader, ShaderCompileError> CompileTessEvaluationShader(const string& source);
    Result<TessControlShader, ShaderCompileError> CompileTessControlShader(const string& source);
    Result<ComputeShader, ShaderCompileError> CompileComputeShader(const string& source);

    RenderProgramLinker MakeRenderProgramLinker();
    ComputeProgramLinker MakeComputeProgramLinker(const ComputeShader& shader);

    RenderProgramBinder BindProgram(RenderProgram& program);
    ComputeProgramBinder BindProgram(ComputeProgram& program);


    template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBuffer<TARGET,DYNAMIC,MR,MW> CreateBufferRaw(usize size) { return {size}; }
    template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBuffer<TARGET,DYNAMIC,MR,MW> CreateBufferRaw(usize size, const void* data) { return {size, data}; }
    template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBufferTyped<T,TARGET,DYNAMIC,MR,MW> CreateBufferTyped(usize count) { return {count}; }
    template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBufferTyped<T,TARGET,DYNAMIC,MR,MW> CreateBufferTyped(usize count, const T* data) { return {count, data}; }
    template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBufferTyped<T,TARGET,DYNAMIC,MR,MW> CreateBufferTyped(const vector<T>& data) { return {data}; }
    template<typename T, usize COUNT, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBufferTyped<T,TARGET,DYNAMIC,MR,MW> CreateBufferTyped(const array<T, COUNT>& data) { return {data}; }
    template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBufferStruct<T,TARGET,DYNAMIC,MR,MW> CreateBufferStruct() { return {}; }
    template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    GraphicsBufferStruct<T,TARGET,DYNAMIC,MR,MW> CreateBufferStruct(const T& data) { return {data}; }
    template<typename INDEX_TYPE = u32, bool DYNAMIC=false, bool MR=false, bool MW=false>
    IndexBuffer<INDEX_TYPE, DYNAMIC,MR,MW> CreateIndexBuffer(usize count) { return {count}; }
    template<typename INDEX_TYPE = u32, bool DYNAMIC=false, bool MR=false, bool MW=false>
    IndexBuffer<INDEX_TYPE, DYNAMIC,MR,MW> CreateIndexBuffer(usize count, const INDEX_TYPE* data) { return {count, data}; }
    template<typename INDEX_TYPE = u32, bool DYNAMIC=false, bool MR=false, bool MW=false>
    IndexBuffer<INDEX_TYPE, DYNAMIC,MR,MW> CreateIndexBuffer(const vector<INDEX_TYPE>& data) { return {data}; }
    template<usize COUNT, typename INDEX_TYPE = u32, bool DYNAMIC=false, bool MR=false, bool MW=false>
    IndexBuffer<INDEX_TYPE, DYNAMIC,MR,MW> CreateIndexBuffer(const array<INDEX_TYPE, COUNT>& data) { return {data}; }
    template<typename VertexType, bool DYNAMIC=false, bool MR=false, bool MW=false>
    VertexBuffer<VertexType,DYNAMIC,MR,MW> CreateVertexBuffer(usize count, cref<VertexLayout> layout) { return {count, layout}; }
    template<typename VertexType, bool DYNAMIC=false, bool MR=false, bool MW=false>
    VertexBuffer<VertexType,DYNAMIC,MR,MW> CreateVertexBuffer(usize count, const VertexType* data, cref<VertexLayout> layout) { return {count, data, layout}; }
    template<typename VertexType, bool DYNAMIC=false, bool MR=false, bool MW=false>
    VertexBuffer<VertexType,DYNAMIC,MR,MW> CreateVertexBuffer(const vector<VertexType>& data, cref<VertexLayout> layout) { return {data, layout}; }
    template<typename VertexType, usize COUNT, bool DYNAMIC=false, bool MR=false, bool MW=false>
    VertexBuffer<VertexType,DYNAMIC,MR,MW> CreateVertexBuffer(const array<VertexType, COUNT>& data, cref<VertexLayout> layout) { return {data, layout}; }

    inline Texture1D CreateTexture1D(uint dim, TextureSizedFormat format) { return Texture1D(dim, format); }
    inline Texture1D CreateTexture1D(uint dim, TextureSizedFormat format, const TextureSettings& settings) { return Texture1D(dim, format, settings); }
    inline Texture1D CreateTexture1D(uint dim, const void* data, TextureSizedFormat format) { return Texture1D(dim, data, format); }
    inline Texture1D CreateTexture1D(uint dim, const void* data, TextureSizedFormat format, const TextureSettings& settings) { return Texture1D(dim, data, format, settings); }

    inline Texture2D CreateTexture2D(uvec2 dim, TextureSizedFormat format) { return Texture2D(dim, format); }
    inline Texture2D CreateTexture2D(uvec2 dim, TextureSizedFormat format, const TextureSettings& settings) { return Texture2D(dim, format, settings); }
    inline Texture2D CreateTexture2D(uvec2 dim, const void* data, TextureSizedFormat format) { return Texture2D(dim, data, format); }
    inline Texture2D CreateTexture2D(uvec2 dim, const void* data, TextureSizedFormat format, const TextureSettings& settings) { return Texture2D(dim, data, format, settings); }

    inline Texture3D CreateTexture3D(uvec3 dim, TextureSizedFormat format) { return Texture3D(dim, format); }
    inline Texture3D CreateTexture3D(uvec3 dim, TextureSizedFormat format, const TextureSettings& settings) { return Texture3D(dim, format, settings); }
    inline Texture3D CreateTexture3D(uvec3 dim, const void* data, TextureSizedFormat format) { return Texture3D(dim, data, format); }
    inline Texture3D CreateTexture3D(uvec3 dim, const void* data, TextureSizedFormat format, const TextureSettings& settings) { return Texture3D(dim, data, format, settings); }

    inline TextureCubeMap CreateTextureCubeMap(uint dim, TextureSizedFormat format) { return TextureCubeMap(dim, format); }
    inline TextureCubeMap CreateTextureCubeMap(uint dim, TextureSizedFormat format, const TextureSettings& settings) { return TextureCubeMap(dim, format, settings); }
    inline TextureCubeMap CreateTextureCubeMap(uint dim, array<const void*, 6> data, TextureSizedFormat format) { return TextureCubeMap(dim, data, format); }
    inline TextureCubeMap CreateTextureCubeMap(uint dim, array<const void*, 6> data, TextureSizedFormat format, const TextureSettings& settings) { return TextureCubeMap(dim, data, format, settings); }

    inline TextureMultisample2D CreateTextureMultisample2D(uvec2 dim, TextureSizedFormat format) { return TextureMultisample2D(dim, format); }
    inline TextureMultisample2D CreateTextureMultisample2D(uvec2 dim, TextureSizedFormat format, const TextureSettings& settings) { return TextureMultisample2D(dim, format, settings); }
    inline TextureMultisample2D CreateTextureMultisample2D(uvec2 dim, uint samples, TextureSizedFormat format) { return TextureMultisample2D(dim, samples, format); }
    inline TextureMultisample2D CreateTextureMultisample2D(uvec2 dim, uint samples, TextureSizedFormat format, const TextureSettings& settings) { return TextureMultisample2D(dim, samples, format, settings); }

    inline Renderbuffer CreateRenderbuffer(uvec2 dim, TextureSizedFormat format) { return Renderbuffer(dim, format); }
    inline Renderbuffer CreateRenderbuffer(uvec2 dim, TextureSizedFormat format, uint samples) { return Renderbuffer(dim, format, samples); }

    inline DefaultFramebuffer& GetDefaultFramebuffer() { return m_defaultFramebuffer; }
    inline CustomFramebuffer CreateFramebuffer() { return CustomFramebuffer(CustomFramebuffer::Gen()); }

    template<GraphicsQueryType TYPE>
    inline GraphicsQuery<TYPE> CreateQuery() { return GraphicsQuery<TYPE>(GraphicsQueryImpl::Gen()); }
    template<GraphicsQueryType TYPE>
    inline GraphicsQueryScope<TYPE> BeginQuery(GraphicsQuery<TYPE>& query) {
        bool& used = m_queryIndexScopes[enum_index(TYPE).value()];
        assert(!used);
        return GraphicsQueryScope<TYPE>(query, used);
    }

    template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=false, bool MW=false>
    inline GraphicsBufferBinder<TARGET,DYNAMIC,MR,MW> BindBuffer(GraphicsBuffer<TARGET,DYNAMIC,MR,MW>& buffer) {
        unord_map<uint, bool>& buffersBound = m_buffersBound[enum_index(TARGET).value()];
        auto boundIt = buffersBound.find(buffer.id());
        if(boundIt == buffersBound.end()) {
            buffersBound.insert({buffer.id(), false});
            boundIt = buffersBound.find(buffer.id());
        }
        bool& bound = boundIt->second;
        assert(!bound);
        return GraphicsBufferBinder<TARGET,DYNAMIC,MR,MW>(buffer, bound);
    }

    template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MR=true, bool MW=true>
    inline GraphicsBufferMapper<TARGET,DYNAMIC,MR,MW> MapBuffer(GraphicsBuffer<TARGET,DYNAMIC,MR,MW>& buffer) {
        return GraphicsBufferMapper<TARGET,DYNAMIC,MR,MW>(buffer);
    }

    inline TextureUnitBinder BindTextureUnit(TextureUnit unit, Texture& texture) {
        bool& bound = m_textureUnitsBound[enum_index(unit).value()];
        assert(!bound);
        return TextureUnitBinder(unit, texture, bound);
    }
    template<bool R=true, bool W=true>
    inline ImageBinder<R, W> BindImage(TextureUnit unit, Texture& texture, bool layered = false) {
        bool& bound = m_imagesBound[enum_index(unit).value()];
        assert(!bound);
        return ImageBinder<R, W>(unit, texture, bound, layered);
    }

    inline FramebufferBinder BindFramebuffer(Framebuffer& framebuffer) {
        assert(!m_framebufferBound);
        return FramebufferBinder(framebuffer, m_framebufferBound);
    }

    template<GraphicsBufferTarget TARGET1, GraphicsBufferTarget TARGET2, bool DYNAMIC1=false, bool DYNAMIC2=false, bool MR1=false, bool MW1=false, bool MR2=false, bool MW2=false>
    void CopyBufferCrossTarget(GraphicsBuffer<TARGET1,DYNAMIC1,MR1,MW1>& readBuffer, GraphicsBuffer<TARGET2,DYNAMIC2,MR2,MW2>& writeBuffer, usize readOffset, usize writeOffset, usize size) {
        GraphicsBufferImpl::Copy(readBuffer.id(), writeBuffer.id(), readOffset, writeOffset, size);
    }
    template<GraphicsBufferTarget TARGET, bool DYNAMIC1=false, bool DYNAMIC2=false, bool MR1=false, bool MW1=false, bool MR2=false, bool MW2=false>
    void CopyBufferRaw(GraphicsBuffer<TARGET,DYNAMIC1,MR1,MW1>& readBuffer, GraphicsBuffer<TARGET,DYNAMIC2,MR2,MW2>& writeBuffer, usize readOffset, usize writeOffset, usize size) {
        GraphicsBufferImpl::Copy(readBuffer.id(), writeBuffer.id(), readOffset, writeOffset, size);
    }
    template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC1=false, bool DYNAMIC2=false, bool MR1=false, bool MW1=false, bool MR2=false, bool MW2=false>
    void CopyBufferTyped(GraphicsBufferTyped<T,TARGET,DYNAMIC1,MR1,MW1>& readBuffer, GraphicsBufferTyped<T,TARGET,DYNAMIC2,MR2,MW2>& writeBuffer, usize readOffset, usize writeOffset, usize count) {
        GraphicsBufferImpl::Copy(readBuffer.id(), writeBuffer.id(), readOffset * sizeof(T), writeOffset * sizeof(T), count * sizeof(T));
    }

    void CopyFramebuffer(Framebuffer& readFramebuffer, Framebuffer& writeFramebuffer, EnumBits<FramebufferBufferType> bufferTypes, bool linear = false);
    void CopyFramebuffer(Framebuffer& readFramebuffer, Framebuffer& writeFramebuffer, EnumBits<FramebufferBufferType> bufferTypes, bool linear, Rectangle readRect, Rectangle writeRect);

    VertexArray CreateVertexArray();
    VertexArrayBinder BindVertexArray(VertexArray& vertexArray);
    template<typename INDEX_TYPE = u32, bool DYNAMIC=false, bool MR=false, bool MW=false>
    IndexedVertexArray<INDEX_TYPE> CreateIndexedVertexArray(IndexBuffer<INDEX_TYPE, DYNAMIC,MR,MW>& indexBuffer) {
        return IndexedVertexArray<INDEX_TYPE>(IndexedVertexArray<INDEX_TYPE>::Gen(), indexBuffer);
    }
    template<typename INDEX_TYPE = u32>
    IndexedVertexArrayBinder<INDEX_TYPE> BindIndexedVertexArray(IndexedVertexArray<INDEX_TYPE>& vertexArray) {
        assert(!m_vertexArrayBound);
    return IndexedVertexArrayBinder<INDEX_TYPE>(vertexArray, m_vertexArrayBound);
    }

    void SetViewport(uvec2 size);
    void SetViewport(uvec2 offset, uvec2 size);

    void DispatchCompute(const ComputeProgramBinder& programBinder, uvec3 numGroups);

    void Finish();
    void TextureBarrier();
    void MemoryBarrier(EnumBits<MemoryBarrierType> types);

private:
    template<typename ShaderDerivedType>
    Result<ShaderDerivedType, ShaderCompileError> CompileShader(const string& source) {
        ShaderDerivedType shader;
        shader.CreateID();
        opt<ShaderCompileError> optError = shader.Compile(source);
        if(!optError) return shader; else return optError.value();
    }
};

}

#endif // MORPH_GRAPHICS_CONTEXT_HPP