
#ifndef MORPH_GRAPHICS_PROGRAM_COMPILER_HPP
#define MORPH_GRAPHICS_PROGRAM_COMPILER_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>

#include "Storage.hpp"

namespace Morph {

class GraphicsProgramPreprocessorError
{
private:
    string m_message;
public:
    GraphicsProgramPreprocessorError(string message) : m_message(message) {}
    const string& message() { return m_message; }
    inline friend std::ostream& operator<<(std::ostream& os, const GraphicsProgramPreprocessorError& error) {
        os << "graphics program preprocessor error: " << error.m_message;
        return os;
    }
};

using GraphicsProgramCompileErrorVar = variant<ShaderCompileError, GraphicsProgramLinkError, GraphicsProgramPreprocessorError>;

class GraphicsProgramCompileError
{
private:
    GraphicsProgramCompileErrorVar m_error;
public:
    GraphicsProgramCompileError(GraphicsProgramCompileErrorVar error) : m_error(error) {}
    const GraphicsProgramCompileErrorVar& get() const { return m_error; }
    GraphicsProgramCompileErrorVar& get() { return m_error; }
    inline friend std::ostream& operator<<(std::ostream& os, const GraphicsProgramCompileError& error) {
        match(error.m_error,
            [&](const ShaderCompileError& specError) {
                os << specError;
            },
            [&](const GraphicsProgramLinkError& specError) {
                os << specError;
            },
            [&](const GraphicsProgramPreprocessorError& specError) {
                os << specError;
            }
        );
        return os;
    }
};

struct GraphicsProgramDefine
{
    string str;
    GraphicsProgramDefine(string a_str) : str("#define " + a_str + "\n") {}
    GraphicsProgramDefine(string name, string value) : GraphicsProgramDefine(name + " " + value) {}
    GraphicsProgramDefine(string name, int value) : GraphicsProgramDefine(name, std::to_string(value)) {}
    GraphicsProgramDefine(string name, float value) : GraphicsProgramDefine(name, std::to_string(value)) {}
};

class GraphicsProgramCompiler
{
private:
    static string s_shaderTypeDirective;
    static unord_map<string, RenderShaderType> s_shaderNamesToTypes;
    ref<GraphicsContext> m_context;
    cref<ResourceStorage> m_storage;
    vector<GraphicsProgramDefine> m_defines;
    unord_set<string> m_tempIncludes;
    vector<string> m_stackIncludes;
public:
    GraphicsProgramCompiler(GraphicsContext& context, const ResourceStorage& storage);
    GraphicsProgramCompiler(GraphicsContext& context, const ResourceStorage& storage, const vector<GraphicsProgramDefine>& defines);

    Result<ComputeProgram, GraphicsProgramCompileError> CompileComputeProgramRes(const string& resFolderRelPath);
    Result<ComputeProgram, GraphicsProgramCompileError> CompileComputeProgram(const string& path);
    Result<RenderProgram, GraphicsProgramCompileError> CompileRenderProgramRes(const string& resFolderRelPath);
    Result<RenderProgram, GraphicsProgramCompileError> CompileRenderProgram(const string& path);

    void SetDefines(const vector<GraphicsProgramDefine>& defines) { m_defines = defines; }

private:
    Result<string, GraphicsProgramPreprocessorError> PreprocessComputeProgram(const string& path);
    Result<array<string, enum_count<RenderShaderType>()>, GraphicsProgramPreprocessorError> PreprocessRenderProgram(const string& path);
    template<typename RenderShaderVarType>
    inline Result<RenderShaderVar, ShaderCompileError> ShaderCompileResVar(Result<RenderShaderVarType, ShaderCompileError> res) {
        using ShaderCompileRes = Result<RenderShaderVar, ShaderCompileError>;
        return match(std::move(res),
            [](RenderShaderVarType shader) -> ShaderCompileRes { return RenderShaderVar(std::move(shader)); },
            [](ShaderCompileError error) -> ShaderCompileRes { return error; }
        );
    }
    opt<GraphicsProgramPreprocessorError> ProcessLine(std::stringstream& outStream, const string& line, const string& programDir);
    opt<GraphicsProgramPreprocessorError> ReadInclude(std::stringstream& outStream, const string& path, const string& programDir);
    opt<string> GetIncludePath(const string& includeLine, const string& programDir) const;
    string GetParentDir(const string& path) const;
};

}

#endif // MORPH_GRAPHICS_PROGRAM_COMPILER_HPP