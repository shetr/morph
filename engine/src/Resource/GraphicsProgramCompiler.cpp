#include "GraphicsProgramCompiler.hpp"

#include <Core/StringUtils.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace Morph {

string GraphicsProgramCompiler::s_shaderTypeDirective = "#type";

unord_map<string, RenderShaderType> GraphicsProgramCompiler::s_shaderNamesToTypes = {
    pair<string, RenderShaderType>("fs", RenderShaderType::FRAGMENT),
    pair<string, RenderShaderType>("frag", RenderShaderType::FRAGMENT),
    pair<string, RenderShaderType>("fragment", RenderShaderType::FRAGMENT),
    pair<string, RenderShaderType>("vs", RenderShaderType::VERTEX),
    pair<string, RenderShaderType>("vert", RenderShaderType::VERTEX),
    pair<string, RenderShaderType>("vertex", RenderShaderType::VERTEX),
    pair<string, RenderShaderType>("gs", RenderShaderType::GEOMETRY),
    pair<string, RenderShaderType>("geom", RenderShaderType::GEOMETRY),
    pair<string, RenderShaderType>("geometry", RenderShaderType::GEOMETRY),
    pair<string, RenderShaderType>("tesc", RenderShaderType::TESS_CONTROL),
    pair<string, RenderShaderType>("tess_control", RenderShaderType::TESS_CONTROL),
    pair<string, RenderShaderType>("tessellation_control", RenderShaderType::TESS_CONTROL),
    pair<string, RenderShaderType>("tese", RenderShaderType::TESS_EVALUATION),
    pair<string, RenderShaderType>("tess_evaluation", RenderShaderType::TESS_EVALUATION),
    pair<string, RenderShaderType>("tessellation_evaluation", RenderShaderType::TESS_EVALUATION),
};

GraphicsProgramCompiler::GraphicsProgramCompiler(GraphicsContext& context, const ResourceStorage& storage)
    : m_context(context), m_storage(storage)
{

}

GraphicsProgramCompiler::GraphicsProgramCompiler(GraphicsContext& context, const ResourceStorage& storage, const vector<GraphicsProgramDefine>& defines)
    : m_context(context), m_storage(storage), m_defines(defines)
{
}

Result<ComputeProgram, GraphicsProgramCompileError> GraphicsProgramCompiler::CompileComputeProgramRes(const string& resFolderRelPath)
{
    const ResourceStorage& storage = m_storage;
    opt<string> maybePath = storage.GetPath(resFolderRelPath);
    if(maybePath) {
        return CompileComputeProgram(maybePath.value());
    }
    return GraphicsProgramCompileError(GraphicsProgramPreprocessorError("res path does not exist"));
}
Result<ComputeProgram, GraphicsProgramCompileError> GraphicsProgramCompiler::CompileComputeProgram(const string& path)
{
    using ResType = Result<ComputeProgram, GraphicsProgramCompileError>;
    const ResourceStorage& storage = m_storage;
    GraphicsContext& context = m_context;
    return match(PreprocessComputeProgram(path),
        [](GraphicsProgramPreprocessorError error) -> ResType {
            return GraphicsProgramCompileError(error);
        },
        [&](string programSrc) -> ResType {
            return match(context.CompileComputeShader(programSrc),
                [] (ShaderCompileError error) -> ResType {
                    return GraphicsProgramCompileError(error);
                },
                [&] (ComputeShader shader) -> ResType {
                    ComputeProgramLinker linker = context.MakeComputeProgramLinker(shader);
                    return match(linker.Link(),
                        [] (GraphicsProgramLinkError error) -> ResType {
                            return GraphicsProgramCompileError(error);
                        },
                        [] (ComputeProgram program) -> ResType {
                            return program;
                        }
                    );
                }
            );
        }
    );
}
Result<RenderProgram, GraphicsProgramCompileError> GraphicsProgramCompiler::CompileRenderProgramRes(const string& resFolderRelPath)
{
    const ResourceStorage& storage = m_storage;
    GraphicsContext& context = m_context;
    opt<string> maybePath = storage.GetPath(resFolderRelPath);
    if(maybePath) {
        return CompileRenderProgram(maybePath.value());
    }
    return GraphicsProgramCompileError(GraphicsProgramPreprocessorError("res path does not exist"));
}
Result<RenderProgram, GraphicsProgramCompileError> GraphicsProgramCompiler::CompileRenderProgram(const string& path)
{
    using ResType = Result<RenderProgram, GraphicsProgramCompileError>;
    const ResourceStorage& storage = m_storage;
    GraphicsContext& context = m_context;
    return match(PreprocessRenderProgram(path),
        [](GraphicsProgramPreprocessorError error) -> ResType {
            return GraphicsProgramCompileError(error);
        },
        [&](array<string, enum_count<RenderShaderType>()> programSrc) -> ResType {
            array<RenderShaderVar, enum_count<RenderShaderType>()> shaders;
            RenderProgramLinker linker = context.MakeRenderProgramLinker();
            for(int i = 0; i < enum_count<RenderShaderType>(); ++i) {
                if(programSrc[i].empty()) {
                    continue;
                }
                using ShaderCompileRes = Result<RenderShaderVar, ShaderCompileError>;
                ShaderCompileRes shaderCompileRes = ShaderCompileError("invalid shader type", ShaderType::VERTEX);
                switch (enum_value<RenderShaderType>(i))
                {
                case RenderShaderType::FRAGMENT:
                    shaderCompileRes = ShaderCompileResVar(context.CompileFragmentShader(programSrc[i]));
                    break;
                case RenderShaderType::VERTEX:
                    shaderCompileRes = ShaderCompileResVar(context.CompileVertexShader(programSrc[i]));
                    break;
                case RenderShaderType::GEOMETRY:
                    shaderCompileRes = ShaderCompileResVar(context.CompileGeometryShader(programSrc[i]));
                    break;
                case RenderShaderType::TESS_CONTROL:
                    shaderCompileRes = ShaderCompileResVar(context.CompileTessControlShader(programSrc[i]));
                    break;
                case RenderShaderType::TESS_EVALUATION:
                    shaderCompileRes = ShaderCompileResVar(context.CompileTessEvaluationShader(programSrc[i]));
                    break;
                }
                if(std::holds_alternative<ShaderCompileError>(shaderCompileRes)) {
                    return GraphicsProgramCompileError(std::move(std::get<ShaderCompileError>(shaderCompileRes)));
                }
                shaders[i] = std::move(std::get<RenderShaderVar>(shaderCompileRes));
                match(shaders[i],
                    [&](RenderShader& shader) { linker.Add(shader); }
                );
            }

            return match(linker.Link(),
                [] (GraphicsProgramLinkError error) -> ResType {
                    return GraphicsProgramCompileError(error);
                },
                [] (RenderProgram program) -> ResType {
                    return program;
                }
            );
        }
    );
}


Result<string, GraphicsProgramPreprocessorError> GraphicsProgramCompiler::PreprocessComputeProgram(const string& path)
{
    std::ifstream programFile(path);
    string programDir = GetParentDir(path);
    m_tempIncludes.clear();
    m_stackIncludes.clear();
    m_stackIncludes.push_back(path);
    if(programFile) {
        std::stringstream programPreprocessedSrc;
        bool isShader = false;
        string line;
        uint lineNum = 1;
        while(std::getline(programFile, line)) {
            string trimedLine = trim_copy(line);
            if(lineNum == 1) {
                if(string_starts_with(trimedLine, "#version")) {
                    programPreprocessedSrc << line << '\n';
                    for(GraphicsProgramDefine& define: m_defines) {
                        programPreprocessedSrc << define.str;
                    }
                } else {
                    return GraphicsProgramPreprocessorError("first line must contain #version");
                }
            } else {
                opt<GraphicsProgramPreprocessorError> optError = ProcessLine(programPreprocessedSrc, line, programDir);
                if(optError) { return optError.value(); }
            }
            lineNum++;
        }
        return programPreprocessedSrc.str();
    }
    return GraphicsProgramPreprocessorError("cannot read file");
}
Result<array<string, enum_count<RenderShaderType>()>, GraphicsProgramPreprocessorError> GraphicsProgramCompiler::PreprocessRenderProgram(const string& path)
{
    array<string, enum_count<RenderShaderType>()> shadersSrc;
    std::ifstream programFile(path);
    string programDir = GetParentDir(path);
    m_tempIncludes.clear();
    m_stackIncludes.clear();
    m_stackIncludes.push_back(path);
    if(programFile) {
        std::stringstream programSharedSrcPart;
        array<std::stringstream, enum_count<RenderShaderType>()> shadersSrc;
        RenderShaderType actualShader = RenderShaderType::FRAGMENT;
        bool isShader = false;
        string line;
        uint lineNum = 1;
        while(std::getline(programFile, line)) {
            string trimedLine = trim_copy(line);
            if(string_starts_with(trimedLine, s_shaderTypeDirective)) {
                m_tempIncludes.clear();
                if(lineNum == 1) {
                    return GraphicsProgramPreprocessorError("first line must contain #version");
                }
                string shaderName = trim_copy(trimedLine.substr(s_shaderTypeDirective.size()));
                auto shaderNamesToTypesPairIt = s_shaderNamesToTypes.find(shaderName);
                if(shaderNamesToTypesPairIt == s_shaderNamesToTypes.end()) {
                    return GraphicsProgramPreprocessorError("invalid shader type: " + shaderName);
                }
                isShader = true;
                actualShader = shaderNamesToTypesPairIt->second;
                std::stringstream& actualShaderSrc = shadersSrc[enum_index(actualShader).value()];
                actualShaderSrc << programSharedSrcPart.str();
            } else if(isShader) {
                if(lineNum == 1) {
                    return GraphicsProgramPreprocessorError("first line must contain #version");
                }
                std::stringstream& actualShaderSrc = shadersSrc[enum_index(actualShader).value()];
                opt<GraphicsProgramPreprocessorError> optError = ProcessLine(actualShaderSrc, line, programDir);
                if(optError) { return optError.value(); }
            } else {
                if(lineNum == 1) {
                    if(string_starts_with(trimedLine, "#version")) {
                        programSharedSrcPart << line << '\n';
                        for(GraphicsProgramDefine& define: m_defines) {
                            programSharedSrcPart << define.str;
                        }
                    } else {
                        return GraphicsProgramPreprocessorError("first line must contain #version");
                    }
                } else {
                    opt<GraphicsProgramPreprocessorError> optError = ProcessLine(programSharedSrcPart, line, programDir);
                    if(optError) { return optError.value(); }
                }
            }
            lineNum++;
        }
        array<string, enum_count<RenderShaderType>()> shadersSrcRes;
        std::transform(shadersSrc.begin(), shadersSrc.end(), shadersSrcRes.begin(),
            [](const std::stringstream& src) -> string { return src.str(); }
        );
        return shadersSrcRes;
    }
    return GraphicsProgramPreprocessorError("cannot read file");
}

opt<GraphicsProgramPreprocessorError> GraphicsProgramCompiler::ProcessLine(std::stringstream& outStream, const string& line, const string& programDir)
{
    string trimedLine = trim_copy(line);
    if(string_starts_with(trimedLine, s_shaderTypeDirective)) {
        return GraphicsProgramPreprocessorError("#type directive is not alowed here");
    } if(string_starts_with(trimedLine, "#include")) {
        opt<string> optPath = GetIncludePath(trimedLine, programDir);
        if(!optPath) {
            return GraphicsProgramPreprocessorError("include path does not exist");
        }
        string& path = optPath.value();
        for(string& includePath: m_stackIncludes) {
            if(includePath == path) {
                return GraphicsProgramPreprocessorError("recursive include is not alowed");
            }
        }
        if(m_tempIncludes.count(path) > 0) {
            return {};
        }
        m_tempIncludes.insert(path);
        m_stackIncludes.push_back(path);
        opt<GraphicsProgramPreprocessorError> optError = ReadInclude(outStream, path, programDir);
        m_stackIncludes.pop_back();
        return optError;
    }
    outStream << line << '\n';
    return {};
}
opt<GraphicsProgramPreprocessorError> GraphicsProgramCompiler::ReadInclude(std::stringstream& outStream, const string& path, const string& programDir)
{
    std::ifstream includeFile(path);
    if(includeFile) {
        string line;
        uint lineNum = 1;
        while(std::getline(includeFile, line)) {
            opt<GraphicsProgramPreprocessorError> optError = ProcessLine(outStream, line, programDir);
            if(optError) { return optError; }
            lineNum++;
        }
        return {};
    }
    return GraphicsProgramPreprocessorError("cannot read include file");
}
opt<string> GraphicsProgramCompiler::GetIncludePath(const string& includeLine, const string& programDir) const
{
    string includeDirective = "#include";
    string closedPathStr = trim_copy(includeLine.substr(includeDirective.size()));
    if(closedPathStr.size() < 3) {
        return {};
    }
    if(closedPathStr[0] == '\"' && closedPathStr[closedPathStr.size()-1] == '\"') {
        string pathText = closedPathStr.substr(1, closedPathStr.size() - 2);
        return programDir + "/" + pathText;
    }
    if(closedPathStr[0] == '<' && closedPathStr[closedPathStr.size()-1] == '>') {
        string pathText = closedPathStr.substr(1, closedPathStr.size() - 2);
        const ResourceStorage& storage = m_storage;
        return storage.GetPath(pathText);
    }
    return {};
}
string GraphicsProgramCompiler::GetParentDir(const string& path) const
{
    std::filesystem::path filePath = path;
    return filePath.parent_path().string();
}

}