
#ifndef MORPH_GENERAL_RENDERER_HPP
#define MORPH_GENERAL_RENDERER_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>
#include <Graphics/Uniforms.hpp>
#include <Data/VertexData.hpp>
#include <Data/TextureUnits.hpp>

namespace Morph {

class GeneralRendererProgram
{
private:
    ref<GraphicsContext> m_context;
    cref<Uniforms> m_generalUniforms;
    cref<Uniforms> m_programUniforms;
public:
    GeneralRendererProgram(GraphicsContext& context, const Uniforms& programUniforms);
    void Draw(VertexData& vertexData, const FramebufferBinder& framebufferBinder);
    void Draw(VertexData& vertexData, const Uniforms& uniforms, const FramebufferBinder& framebufferBinder);
    void Draw(VertexData& vertexData, const Uniforms& uniforms, TextureUnits& textures, const FramebufferBinder& framebufferBinder);
};

class GeneralRendererScene
{
private:
    ref<GraphicsContext> m_context;
    cref<Uniforms> m_generalUniforms;
public:
    GeneralRendererScene(GraphicsContext& context, const Uniforms& generalUniforms);
    GeneralRendererProgram BeginProgram(RenderProgram& program, const Uniforms& programUniforms);
};

class GeneralRenderer
{
private:
    ref<GraphicsContext> m_context;
public:
    GeneralRenderer(GraphicsContext& context);
    GeneralRendererScene BeginScene(const Uniforms& generalUniforms);
};

}

#endif // MORPH_GENERAL_RENDERER_HPP