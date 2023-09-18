#include "Shader.hpp"

#include "OpenGL.hpp"

namespace Morph {

Shader::Shader() : m_id(0) {}

Shader::~Shader()
{
    if(m_id != 0) {
        GL(DeleteShader(m_id));
    }
}

Shader::Shader(Shader&& shader) : m_id(shader.m_id)
{
    shader.m_id = 0;
}

Shader& Shader::operator=(Shader&& shader)
{
    m_id = shader.m_id;
    shader.m_id = 0;
    return (*this);
}

void Shader::CreateID()
{
    m_id = GL(CreateShader(GraphicsEnums::shaderTypes.ToValue(GetType())));
}

 opt<ShaderCompileError> Shader::Compile(const string& source)
{
    const char* cSource = source.c_str();
    GL(ShaderSource(m_id, 1, &cSource, 0));
    GL(CompileShader(m_id));

    int isCompiled = 0;
    GL(GetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled));
    if (!isCompiled)
    {
        int maxLength = 0;
        GL(GetShaderiv(m_id, GL_INFO_LOG_LENGTH, &maxLength));
        vector<char> infoLog(maxLength + 1);
        GL(GetShaderInfoLog(m_id, maxLength, &maxLength, infoLog.data()));
        infoLog[maxLength] = 0;
        return ShaderCompileError(string(infoLog.data()), GetType());
    }
    return {};
}

}