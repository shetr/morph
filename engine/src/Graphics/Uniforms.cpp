#include "Uniforms.hpp"

#include "OpenGL.hpp"

namespace Morph {

int IUniformSingle::GetLocation(const GraphicsProgramBinder& programBinder) const
{
    int loc = -1;
    auto progLocIt = m_locs.find(programBinder.id()); 
    if(progLocIt == m_locs.end()) {
        loc = GL(GetUniformLocation(programBinder.id(), m_name.c_str()));
        m_locs.insert_or_assign(programBinder.id(), loc);
        #ifdef MORPH_DEBUG
            if(loc == -1) {
                spdlog::debug("uniform {} not set in program {}", m_name, programBinder.id());
            }
        #endif
    } else {
        loc = progLocIt->second;
    }
    return loc;
}

void IUniformSingle::Set(int loc, const TextureUnit& v) const
{
    GL(Uniform1i(loc, enum_index(v).value()));
}
void IUniformSingle::Set(int loc, const bool& v) const
{
    GL(Uniform1i(loc, v));
}
void IUniformSingle::Set(int loc, const int& v) const
{
    GL(Uniform1i(loc, v));
}
void IUniformSingle::Set(int loc, const uint& v) const
{
    GL(Uniform1ui(loc, v));
}
void IUniformSingle::Set(int loc, const float& v) const
{
    GL(Uniform1f(loc, v));
}
void IUniformSingle::Set(int loc, const ivec1& v) const
{
    GL(Uniform1i(loc, v.x));
}
void IUniformSingle::Set(int loc, const ivec2& v) const
{
    GL(Uniform2i(loc, v.x, v.y));
}
void IUniformSingle::Set(int loc, const ivec3& v) const
{
    GL(Uniform3i(loc, v.x, v.y, v.z));
}
void IUniformSingle::Set(int loc, const ivec4& v) const
{
    GL(Uniform4i(loc, v.x, v.y, v.z, v.w));
}
void IUniformSingle::Set(int loc, const uvec1& v) const
{
    GL(Uniform1ui(loc, v.x));
}
void IUniformSingle::Set(int loc, const uvec2& v) const
{
    GL(Uniform2ui(loc, v.x, v.y));
}
void IUniformSingle::Set(int loc, const uvec3& v) const
{
    GL(Uniform3ui(loc, v.x, v.y, v.z));
}
void IUniformSingle::Set(int loc, const uvec4& v) const
{
    GL(Uniform4ui(loc, v.x, v.y, v.z, v.w));
}
void IUniformSingle::Set(int loc, const vec1& v) const
{
    GL(Uniform1f(loc, v.x));
}
void IUniformSingle::Set(int loc, const vec2& v) const
{
    GL(Uniform2f(loc, v.x, v.y));
}
void IUniformSingle::Set(int loc, const vec3& v) const
{
    GL(Uniform3f(loc, v.x, v.y, v.z));
}
void IUniformSingle::Set(int loc, const vec4& v) const
{
    GL(Uniform4f(loc, v.x, v.y, v.z, v.w));
}
void IUniformSingle::Set(int loc, const mat2& v) const
{
    GL(UniformMatrix2fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat3& v) const
{
    GL(UniformMatrix3fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat4& v) const
{
    GL(UniformMatrix4fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat2x3& v) const
{
    GL(UniformMatrix2x3fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat3x2& v) const
{
    GL(UniformMatrix3x2fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat2x4& v) const
{
    GL(UniformMatrix2x4fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat4x2& v) const
{
    GL(UniformMatrix4x2fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat3x4& v) const
{
    GL(UniformMatrix3x4fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const mat4x3& v) const
{
    GL(UniformMatrix4x3fv(loc, 1, false, glm::value_ptr(v)));
}
void IUniformSingle::Set(int loc, const vector<int>& v) const
{
    GL(Uniform1iv(loc, v.size(), v.data()));
}
void IUniformSingle::Set(int loc, const vector<uint>& v) const
{
    GL(Uniform1uiv(loc, v.size(), v.data()));
}
void IUniformSingle::Set(int loc, const vector<float>& v) const
{
    GL(Uniform1fv(loc, v.size(), v.data()));
}
void IUniformSingle::Set(int loc, const vector<ivec1>& v) const
{
    GL(Uniform1iv(loc, v.size(), reinterpret_cast<const int*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<ivec2>& v) const
{
    GL(Uniform2iv(loc, v.size(), reinterpret_cast<const int*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<ivec3>& v) const
{
    GL(Uniform3iv(loc, v.size(), reinterpret_cast<const int*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<ivec4>& v) const
{
    GL(Uniform4iv(loc, v.size(), reinterpret_cast<const int*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<uvec1>& v) const
{
    GL(Uniform1uiv(loc, v.size(), reinterpret_cast<const uint*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<uvec2>& v) const
{
    GL(Uniform2uiv(loc, v.size(), reinterpret_cast<const uint*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<uvec3>& v) const
{
    GL(Uniform3uiv(loc, v.size(), reinterpret_cast<const uint*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<uvec4>& v) const
{
    GL(Uniform4uiv(loc, v.size(), reinterpret_cast<const uint*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<vec1>& v) const
{
    GL(Uniform1fv(loc, v.size(), reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<vec2>& v) const
{
    GL(Uniform2fv(loc, v.size(), reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<vec3>& v) const
{
    GL(Uniform3fv(loc, v.size(), reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<vec4>& v) const
{
    GL(Uniform4fv(loc, v.size(), reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat2>& v) const
{
    GL(UniformMatrix2fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat3>& v) const
{
    GL(UniformMatrix3fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat4>& v) const
{
    GL(UniformMatrix4fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat2x3>& v) const
{
    GL(UniformMatrix2x3fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat3x2>& v) const
{
    GL(UniformMatrix3x2fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat2x4>& v) const
{
    GL(UniformMatrix2x4fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat4x2>& v) const
{
    GL(UniformMatrix4x2fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat3x4>& v) const
{
    GL(UniformMatrix3x4fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}
void IUniformSingle::Set(int loc, const vector<mat4x3>& v) const
{
    GL(UniformMatrix4x3fv(loc, v.size(), false, reinterpret_cast<const float*>(v.data())));
}


UniformStruct::UniformStruct(const string& structName)
    : m_structName(structName)
{}
void UniformStruct::Set(const GraphicsProgramBinder& programBinder) const
{
    for(const IUniformSingle& uniform : m_uniforms) {
        uniform.Set(programBinder);
    }
}


void Uniforms::SetUniforms(const GraphicsProgramBinder& programBinder) const
{
    for(const IUniform& uniform : m_uniforms) {
        uniform.Set(programBinder);
    }
}
void Uniforms::AddUniform(const IUniform& uniform)
{
    m_uniforms.push_back(uniform);
}

}