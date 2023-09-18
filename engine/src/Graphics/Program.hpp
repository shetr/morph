
#ifndef MORPH_GRAPHICS_PROGRAM_HPP
#define MORPH_GRAPHICS_PROGRAM_HPP

#include <Core/Core.hpp>

#include "Shader.hpp"

namespace Morph {

class GraphicsContext;

class GraphicsProgramLinkError
{
private:
    string m_message;
public:
    GraphicsProgramLinkError(string message) : m_message(message) {}
    const string& message() { return m_message; }
    inline friend std::ostream& operator<<(std::ostream& os, const GraphicsProgramLinkError& error) {
        os << "graphics program link error: " << error.m_message;
        return os;
    }
};

class GraphicsProgram
{
private:
    uint m_id;
protected:
    GraphicsProgram(uint id) : m_id(id) {}
public:
    GraphicsProgram() : m_id(0) {}
    GraphicsProgram(const GraphicsProgram& program) = delete;
    GraphicsProgram(GraphicsProgram&& old);
    GraphicsProgram& operator=(GraphicsProgram&& old);
    virtual ~GraphicsProgram();
    uint GetID() const { return m_id; }
    inline operator bool() const { return m_id != 0; }
};

class RenderProgramLinker;
class RenderProgram : public GraphicsProgram
{
    friend class RenderProgramLinker;
protected:
    RenderProgram(uint id) : GraphicsProgram(id) {}
public:
    RenderProgram() : GraphicsProgram() {}
};

class GeometryProgramLinker;
class GeometryProgram : public RenderProgram
{
    friend class GeometryProgramLinker;
protected:
    GeometryProgram(uint id) : RenderProgram(id) {}
public:
    GeometryProgram() : RenderProgram() {}
};

class ComputeProgramLinker;
class ComputeProgram : public GraphicsProgram
{
    friend class ComputeProgramLinker;
protected:
    ComputeProgram(uint id) : GraphicsProgram(id) {}
public:
    ComputeProgram() : GraphicsProgram() {}
};

class GraphicsProgramLinker
{
private:
    vector<cref<Shader>> m_shaders;
protected:
    GraphicsProgramLinker() : m_shaders() {}
    void AddShader(const Shader& shader);
    Result<uint, GraphicsProgramLinkError> LinkProgram();
};

class RenderProgramLinker : public GraphicsProgramLinker
{
    friend class GraphicsContext;
protected:
    RenderProgramLinker();
public:
    void Add(const RenderShader& shader);
    Result<RenderProgram, GraphicsProgramLinkError> Link();
};

class GeometryProgramLinker : public RenderProgramLinker
{
    friend class GraphicsContext;
private:
    GeometryProgramLinker(const VertexShader& vertShader, const FragmentShader& fragShader, const GeometryShader& geomShader);
    using RenderProgramLinker::Link;
public:
    Result<GeometryProgram, GraphicsProgramLinkError> Link();
};

class ComputeProgramLinker : public GraphicsProgramLinker
{
    friend class GraphicsContext;
private:
    ComputeProgramLinker(const ComputeShader& shader);
public:
    void Add(const ComputeShader& shader);
    Result<ComputeProgram, GraphicsProgramLinkError> Link();
};

class GraphicsProgramBinder
{
    struct BinderData {
        ref<GraphicsProgram> program;
        ref<bool> bound;
    };
private:
    opt<BinderData> m_data;
protected:
    GraphicsProgramBinder(GraphicsProgram& program, bool& bound);
    GraphicsProgramBinder();
public:
    GraphicsProgramBinder(const GraphicsProgramBinder& binder) = delete;
    GraphicsProgramBinder(GraphicsProgramBinder&& old);
    GraphicsProgramBinder& operator=(GraphicsProgramBinder&& old);
    virtual ~GraphicsProgramBinder();
    uint id() const { return m_data.value().program.get().GetID(); }
    inline operator bool() const { return m_data.has_value(); }
};

class RenderProgramBinder : public GraphicsProgramBinder
{
    friend class GraphicsContext;
private:
    RenderProgramBinder(RenderProgram& program, bool& bound) : GraphicsProgramBinder(program, bound) {}
public:
    RenderProgramBinder() : GraphicsProgramBinder() {}
};
class ComputeProgramBinder : public GraphicsProgramBinder
{
    friend class GraphicsContext;
private:
    ComputeProgramBinder(ComputeProgram& program, bool& bound) : GraphicsProgramBinder(program, bound) {}
public:
    ComputeProgramBinder() : GraphicsProgramBinder() {}
};

}

#endif // MORPH_GRAPHICS_PROGRAM_HPP