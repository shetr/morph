
#ifndef MORPH_SHADER_TRANSFORM_HPP
#define MORPH_SHADER_TRANSFORM_HPP

#include <Core/Core.hpp>
#include <Graphics/Uniforms.hpp>

namespace Morph {

class ShaderTransform
{
private:
    mat4 m_M = mat4(1);
    mat4 m_V = mat4(1);
    mat4 m_P = mat4(1);
    mat4 m_VM = mat4(1);
    mat4 m_PVM = mat4(1);
    mat3 m_N = mat4(1);
public:
    ShaderTransform() {}
    ShaderTransform(const mat4& M, const mat4& V, const mat4& P) { Set(M, V, P); }
    void Set(const mat4& M, const mat4& V, const mat4& P) {
        m_M = M;
        m_V = V;
        m_P = P;
        m_VM = V * M;
        m_PVM = P * m_VM;
        m_N = ComputeN(M);
    }
    const mat4& M() const { return m_M; }
    const mat4& V() const { return m_V; }
    const mat4& P() const { return m_P; }
    const mat4& PVM() const { return m_PVM; }
    const mat3& N() const { return m_N; }

    mat3 ComputeN(const mat4& M)
    {
        mat3 N = glm::mat3(M);
        return glm::transpose(glm::inverse(N));
    }
};

class ShaderTransformUniforms : public Uniforms
{
private:
    Uniform<mat4> m_M;
    Uniform<mat4> m_V;
    Uniform<mat4> m_P;
    Uniform<mat4> m_PVM;
    Uniform<mat3> m_N;
public:
    ShaderTransformUniforms(const ShaderTransform& shaderTransform)
        :
        Uniforms({std::cref(m_M), std::cref(m_V), std::cref(m_P), std::cref(m_PVM), std::cref(m_N)}),
        m_M("u_M", shaderTransform.M()),
        m_V("u_V", shaderTransform.V()),
        m_P("u_P", shaderTransform.P()),
        m_PVM("u_PVM", shaderTransform.PVM()),
        m_N("u_N", shaderTransform.N())
    {}
};

}

#endif // MORPH_SHADER_TRANSFORM_HPP