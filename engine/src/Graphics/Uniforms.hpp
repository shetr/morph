
#ifndef MORPH_UNIROMS_HPP
#define MORPH_UNIROMS_HPP

#include <Core/Core.hpp>

#include "Program.hpp"

namespace Morph {

class IUniform
{
public:
    virtual void Set(const GraphicsProgramBinder& programBinder) const = 0;
};

class IUniformSingle : public IUniform
{
private:
    string m_name;
    mutable unord_map<uint, int> m_locs;
public:
    IUniformSingle() : m_name("")  {}
    IUniformSingle(const string& name) : m_name(name) {}
    virtual void Set(const GraphicsProgramBinder& programBinder) const = 0;
    string& name() { return m_name; }
    const string& name() const { return m_name; }
    int location(const GraphicsProgramBinder& programBinder) const { return find_or(m_locs, programBinder.id(), -1); }
protected:
    int GetLocation(const GraphicsProgramBinder& programBinder) const;

    void Set(int loc, const TextureUnit& v) const;
    void Set(int loc, const bool& v) const;
    void Set(int loc, const int& v) const;
    void Set(int loc, const uint& v) const;
    void Set(int loc, const float& v) const;
    void Set(int loc, const ivec1& v) const;
    void Set(int loc, const ivec2& v) const;
    void Set(int loc, const ivec3& v) const;
    void Set(int loc, const ivec4& v) const;
    void Set(int loc, const uvec1& v) const;
    void Set(int loc, const uvec2& v) const;
    void Set(int loc, const uvec3& v) const;
    void Set(int loc, const uvec4& v) const;
    void Set(int loc, const vec1& v) const;
    void Set(int loc, const vec2& v) const;
    void Set(int loc, const vec3& v) const;
    void Set(int loc, const vec4& v) const;
    void Set(int loc, const mat2& v) const;
    void Set(int loc, const mat3& v) const;
    void Set(int loc, const mat4& v) const;
    void Set(int loc, const mat2x3& v) const;
    void Set(int loc, const mat3x2& v) const;
    void Set(int loc, const mat2x4& v) const;
    void Set(int loc, const mat4x2& v) const;
    void Set(int loc, const mat3x4& v) const;
    void Set(int loc, const mat4x3& v) const;
    void Set(int loc, const vector<int>& v) const;
    void Set(int loc, const vector<uint>& v) const;
    void Set(int loc, const vector<float>& v) const;
    void Set(int loc, const vector<ivec1>& v) const;
    void Set(int loc, const vector<ivec2>& v) const;
    void Set(int loc, const vector<ivec3>& v) const;
    void Set(int loc, const vector<ivec4>& v) const;
    void Set(int loc, const vector<uvec1>& v) const;
    void Set(int loc, const vector<uvec2>& v) const;
    void Set(int loc, const vector<uvec3>& v) const;
    void Set(int loc, const vector<uvec4>& v) const;
    void Set(int loc, const vector<vec1>& v) const;
    void Set(int loc, const vector<vec2>& v) const;
    void Set(int loc, const vector<vec3>& v) const;
    void Set(int loc, const vector<vec4>& v) const;
    void Set(int loc, const vector<mat2>& v) const;
    void Set(int loc, const vector<mat3>& v) const;
    void Set(int loc, const vector<mat4>& v) const;
    void Set(int loc, const vector<mat2x3>& v) const;
    void Set(int loc, const vector<mat3x2>& v) const;
    void Set(int loc, const vector<mat2x4>& v) const;
    void Set(int loc, const vector<mat4x2>& v) const;
    void Set(int loc, const vector<mat3x4>& v) const;
    void Set(int loc, const vector<mat4x3>& v) const;
};

template<typename T>
class Uniform : public IUniformSingle
{
private:
    cref<T> m_value;
public:
    Uniform(const T& v) : IUniformSingle(), m_value(v) {}
    Uniform(const string& name, const T& v) : IUniformSingle(name), m_value(v) {}
    void Set(const GraphicsProgramBinder& programBinder) const override {
        IUniformSingle::Set(GetLocation(programBinder), m_value);
    }
};

template<typename T>
class UniformArray : public IUniform
{
private:
    Uniform<uint> m_count;
    Uniform<vector<T>> m_values;
public:
    UniformArray(const string& name, const uint& count, const vector<T>& values)
        : m_count(name+"_count", count), m_values(name, values)
    {}
    void Set(const GraphicsProgramBinder& programBinder) const override {
        m_count.Set(programBinder);
        m_values.Set(programBinder);
    }
};

class UniformStruct : public IUniform
{
private:
    vector<cref<IUniformSingle>> m_uniforms;
    string m_structName;
public:
    UniformStruct(const string& structName);
    void Set(const GraphicsProgramBinder& programBinder) const override;
protected:
    template<typename T>
    inline void AddUniform(Uniform<T>& uniform, const string& name) {
        uniform.name() = m_structName + "." + name;
        m_uniforms.push_back(uniform);
    }
};

class Uniforms
{
private:
    vector<cref<IUniform>> m_uniforms;
public:
    Uniforms()  {}
    Uniforms(init_list<cref<IUniform>> uniforms) : m_uniforms(uniforms)  {}
    Uniforms(vector<cref<IUniform>> uniforms) : m_uniforms(uniforms)  {}
    void SetUniforms(const GraphicsProgramBinder& programBinder) const;
protected:
    void AddUniform(const IUniform& uniform);
};

}

#endif // MORPH_UNIROMS_HPP