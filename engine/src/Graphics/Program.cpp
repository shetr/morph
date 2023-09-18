#include "Program.hpp"

#include "OpenGL.hpp"

namespace Morph {

GraphicsProgram::GraphicsProgram(GraphicsProgram&& old) : m_id(old.m_id)
{
    old.m_id = 0;
}

GraphicsProgram& GraphicsProgram::operator=(GraphicsProgram&& old)
{
    m_id = old.m_id;
    old.m_id = 0;
    return *this;
}

GraphicsProgram::~GraphicsProgram()
{
    if(m_id != 0) {
        GL(DeleteProgram(m_id));
    }
}

void GraphicsProgramLinker::AddShader(const Shader& shader)
{
    m_shaders.push_back(shader);
}

Result<uint, GraphicsProgramLinkError> GraphicsProgramLinker::LinkProgram()
{
    uint programId = GL(CreateProgram());
    for(const Shader& shader: m_shaders){
        GL(AttachShader(programId, shader.GetID()));
    }
    GL(LinkProgram(programId));

    int isLinked = 0;
    GL(GetProgramiv(programId, GL_LINK_STATUS, &isLinked));
    if (!isLinked)
    {
        int maxLength = 0;
        GL(GetProgramiv(programId, GL_INFO_LOG_LENGTH, &maxLength));
        std::vector<char> infoLog(maxLength + 1);
        GL(GetProgramInfoLog(programId, maxLength, &maxLength, infoLog.data()));
        infoLog[maxLength] = 0;
        for (const Shader& shader: m_shaders) {
            GL(DetachShader(programId, shader.GetID()));
        }
        GL(DeleteProgram(programId));
        return GraphicsProgramLinkError(string(infoLog.data()));
    }
    
    for (const Shader& shader: m_shaders) {
        GL(DetachShader(programId, shader.GetID()));
    }
    return programId;
}

RenderProgramLinker::RenderProgramLinker()
    : GraphicsProgramLinker()
{
}
void RenderProgramLinker::Add(const RenderShader& shader)
{
    AddShader(shader);
}
Result<RenderProgram, GraphicsProgramLinkError> RenderProgramLinker::Link()
{
    using ResType = Result<RenderProgram, GraphicsProgramLinkError>;
    Result<uint, GraphicsProgramLinkError> linkRes = LinkProgram();
    return match(linkRes,
        [](uint id) -> ResType { return RenderProgram(id); },
        [](GraphicsProgramLinkError error) -> ResType { return error; }
    );
}


GeometryProgramLinker::GeometryProgramLinker(const VertexShader& vertShader, const FragmentShader& fragShader, const GeometryShader& geomShader)
{
    AddShader(vertShader);
    AddShader(fragShader);
    AddShader(geomShader);
}
Result<GeometryProgram, GraphicsProgramLinkError> GeometryProgramLinker::Link()
{
    using ResType = Result<GeometryProgram, GraphicsProgramLinkError>;
    Result<uint, GraphicsProgramLinkError> linkRes = LinkProgram();
    return match(linkRes,
        [](uint id) -> ResType { return GeometryProgram(id); },
        [](GraphicsProgramLinkError error) -> ResType { return error; }
    );
}

ComputeProgramLinker::ComputeProgramLinker(const ComputeShader& shader)
    : GraphicsProgramLinker()
{
    AddShader(shader);
}
void ComputeProgramLinker::Add(const ComputeShader& shader)
{
    AddShader(shader);
}
Result<ComputeProgram, GraphicsProgramLinkError> ComputeProgramLinker::Link()
{
    using ResType = Result<ComputeProgram, GraphicsProgramLinkError>;
    Result<uint, GraphicsProgramLinkError> linkRes = LinkProgram();
    return match(linkRes,
        [](uint id) -> ResType { return ComputeProgram(id); },
        [](GraphicsProgramLinkError error) -> ResType { return error; }
    );
}


GraphicsProgramBinder::GraphicsProgramBinder(GraphicsProgram& program, bool& bound)
    : m_data({program, bound})
{
    GL(UseProgram(program.GetID()));
    bound = true;
}
GraphicsProgramBinder::GraphicsProgramBinder() {}
GraphicsProgramBinder::GraphicsProgramBinder(GraphicsProgramBinder&& old)
{
    std::swap(m_data, old.m_data);
}
GraphicsProgramBinder& GraphicsProgramBinder::operator=(GraphicsProgramBinder&& old)
{
    std::swap(m_data, old.m_data);
    return *this;
}
GraphicsProgramBinder::~GraphicsProgramBinder()
{
    if(m_data) {
        bool& bound = m_data.value().bound;
        bound = false;
        GL(UseProgram(0));
    }
}

}