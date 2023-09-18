
#ifndef MORPH_SHADER_HPP
#define MORPH_SHADER_HPP

#include <Core/Core.hpp>

#include "Enums.hpp"

namespace Morph {

class GraphicsContext;

class ShaderCompileError
{
private:
    string m_message;
    ShaderType m_shaderType;
public:
    ShaderCompileError(string message, ShaderType shaderType) : m_message(message), m_shaderType(shaderType) {}
    const string& message() { return m_message; }
    const ShaderType& shaderType() { return m_shaderType; }
    inline friend std::ostream& operator<<(std::ostream& os, const ShaderCompileError& error) {
        os << enum_name(error.m_shaderType) << " shader compile error: " << error.m_message;
        return os;
    }
};

class Shader
{
    friend class GraphicsContext;
private:
    uint m_id = 0;
protected:
    void CreateID();
    opt<ShaderCompileError> Compile(const string& source);
public:
    Shader();
    Shader(const Shader& shader) = delete;
    Shader(Shader&& shader);
    Shader& operator=(Shader&& shader);
    virtual ~Shader();
    virtual ShaderType GetType() = 0;
    inline uint GetID() const { return m_id; }
    inline operator bool() const { return m_id != 0; }
};

class RenderShader : public Shader {
    friend class GraphicsContext;
protected:
    RenderShader() : Shader() {}
};

class VertexShader : public RenderShader {
    friend class GraphicsContext;
public:
    VertexShader() : RenderShader() {}
    inline ShaderType GetType() override { return ShaderType::VERTEX; }
};
class FragmentShader : public RenderShader {
    friend class GraphicsContext;
public:
    FragmentShader() : RenderShader() {}
    inline ShaderType GetType() override { return ShaderType::FRAGMENT; }
};
class GeometryShader : public RenderShader {
    friend class GraphicsContext;
public:
    GeometryShader() : RenderShader() {}
    inline ShaderType GetType() override { return ShaderType::GEOMETRY; }
};
class TessEvaluationShader : public RenderShader {
    friend class GraphicsContext;
public:
    TessEvaluationShader() : RenderShader() {}
    inline ShaderType GetType() override { return ShaderType::TESS_EVALUATION; }
};
class TessControlShader : public RenderShader {
    friend class GraphicsContext;
public:
    TessControlShader() : RenderShader() {}
    inline ShaderType GetType() override { return ShaderType::TESS_CONTROL; }
};

class ComputeShader : public Shader {
    friend class GraphicsContext;
public:
    ComputeShader() : Shader() {}
    inline ShaderType GetType() override { return ShaderType::COMPUTE; }
};

using RenderShaderVar = variant<
    VertexShader,
    FragmentShader,
    GeometryShader,
    TessEvaluationShader,
    TessControlShader
>;

}

#endif // MORPH_SHADER_HPP