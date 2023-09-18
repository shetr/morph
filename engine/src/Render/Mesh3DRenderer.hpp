
#ifndef MORPH_MESH3D_RENDERER_HPP
#define MORPH_MESH3D_RENDERER_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>

namespace Morph {

class Mesh3DRendererProgram
{
private:
    ref<GraphicsContext> m_context;
public:
    Mesh3DRendererProgram(GraphicsContext& context);
    void DrawMesh();
};

class Mesh3DRendererScene
{
private:
    ref<GraphicsContext> m_context;
public:
    Mesh3DRendererScene(GraphicsContext& context);
    Mesh3DRendererProgram BeginProgram(RenderProgram& program);
};

class Mesh3DRenderer
{
private:
    ref<GraphicsContext> m_context;
public:
    Mesh3DRenderer(GraphicsContext& context);
    Mesh3DRendererScene BeginScene(mat4 projection, mat4 view);
};

}

#endif // MORPH_MESH3D_RENDERER_HPP