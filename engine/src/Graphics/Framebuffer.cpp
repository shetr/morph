#include "Framebuffer.hpp"

#include "OpenGL.hpp"

namespace Morph {


void Framebuffer::OnBind()
{
    OnBindImpl();
}

void DefaultFramebuffer::OnBindImpl()
{

}
void DefaultFramebuffer::SetSwapWaitInterval(int screenUpdatesCount)
{
    glfwSwapInterval(screenUpdatesCount);
}
void DefaultFramebuffer::SwapBuffers()
{
    const Window& window = m_window;
    glfwSwapBuffers(window.id());
}

CustomFramebuffer::CustomFramebuffer(CustomFramebuffer&& framebuffer)
{
    std::swap(m_id, framebuffer.m_id);
    std::swap(m_drawAttachments, framebuffer.m_drawAttachments);
    std::swap(m_textures, framebuffer.m_textures);
    std::swap(m_multisampleTextures, framebuffer.m_multisampleTextures);
    std::swap(m_renderbuffers, framebuffer.m_renderbuffers);
    std::swap(m_dim, framebuffer.m_dim);
}
CustomFramebuffer& CustomFramebuffer::operator=(CustomFramebuffer&& framebuffer)
{
    std::swap(m_id, framebuffer.m_id);
    std::swap(m_drawAttachments, framebuffer.m_drawAttachments);
    std::swap(m_textures, framebuffer.m_textures);
    std::swap(m_multisampleTextures, framebuffer.m_multisampleTextures);
    std::swap(m_renderbuffers, framebuffer.m_renderbuffers);
    std::swap(m_dim, framebuffer.m_dim);
    return *this;
}
CustomFramebuffer::~CustomFramebuffer()
{
    if(m_id != 0) {
        GL(DeleteFramebuffers(1, &m_id));
    }
}

uint CustomFramebuffer::Gen()
{
    uint id;
    GL(CreateFramebuffers(1, &id));
    return id;
}

void CustomFramebuffer::Attach(CustomFramebufferAttachment attachment, Texture2D& texture)
{
    m_dim = texture.dim();
    GL(BindFramebuffer(GL_FRAMEBUFFER, m_id));
    GL(FramebufferTexture2D(
        GL_FRAMEBUFFER,
        GraphicsEnums::customFramebufferAttachments.ToValue(attachment),
        GraphicsEnums::textureTypes.ToValue(texture.GetType()),
        texture.id(),
        0
    ));
    GL(BindFramebuffer(GL_FRAMEBUFFER, 0));
    m_textures.push_back(texture);
}
void CustomFramebuffer::Attach(CustomFramebufferAttachment attachment, TextureMultisample2D& texture)
{
    m_dim = texture.dim();
    GL(BindFramebuffer(GL_FRAMEBUFFER, m_id));
    GL(FramebufferTexture2D(
        GL_FRAMEBUFFER,
        GraphicsEnums::customFramebufferAttachments.ToValue(attachment),
        GraphicsEnums::textureTypes.ToValue(texture.GetType()),
        texture.id(),
        0
    ));
    GL(BindFramebuffer(GL_FRAMEBUFFER, 0));
    m_multisampleTextures.push_back(texture);
}
void CustomFramebuffer::Attach(CustomFramebufferAttachment attachment, Renderbuffer& renderbuffer)
{
    m_dim = renderbuffer.dim();
    GL(BindFramebuffer(GL_FRAMEBUFFER, m_id));
    GL(FramebufferRenderbuffer(
        GL_FRAMEBUFFER,
        GraphicsEnums::customFramebufferAttachments.ToValue(attachment),
        GL_RENDERBUFFER,
        renderbuffer.id()
    ));
    GL(BindFramebuffer(GL_FRAMEBUFFER, 0));
    m_renderbuffers.push_back(renderbuffer);
}


void CustomFramebuffer::Resize(uvec2 dim)
{
    m_dim = dim;
    for(Texture2D& texture: m_textures) {
        texture.ResizeUninit(dim);
    }
    for(TextureMultisample2D& texture: m_multisampleTextures) {
        texture.Resize(dim);
    }
    for(Renderbuffer& renderbuffer: m_renderbuffers) {
        renderbuffer.Resize(dim);
    }
}

void CustomFramebuffer::SetDrawAttachments(const vector<CustomFramebufferDrawAttachment>& attachments)
{
    m_drawAttachments.resize(attachments.size());
    std::transform(attachments.begin(), attachments.end(), m_drawAttachments.begin(),
        [](CustomFramebufferDrawAttachment attachment) { 
            return GraphicsEnums::customFramebufferDrawAttachments.ToValue(attachment); 
        }
    );
}

void CustomFramebuffer::OnBindImpl()
{
    if(m_drawAttachments.size() > 0) {
        GL(DrawBuffers(m_drawAttachments.size(), m_drawAttachments.data()));
    }
}


FramebufferBinder::FramebufferBinder(Framebuffer& framebuffer, bool& bound) : m_data({framebuffer, bound})
{
    GL(BindFramebuffer(GL_FRAMEBUFFER, framebuffer.id()));
    bound = true;
    framebuffer.OnBind();
}
FramebufferBinder::FramebufferBinder(FramebufferBinder&& binder)
{
    std::swap(m_data, binder.m_data);
}
FramebufferBinder& FramebufferBinder::operator=(FramebufferBinder&& binder)
{
    std::swap(m_data, binder.m_data);
    return *this;
}
FramebufferBinder::~FramebufferBinder()
{
    if(m_data) {
        bool& bound = m_data.value().bound;
        GL(BindFramebuffer(GL_FRAMEBUFFER, 0));
        bound = false;
    }
}

void FramebufferBinder::ClearColor()
{
    GL(Clear(GL_COLOR_BUFFER_BIT));
}
void FramebufferBinder::Clear(EnumBits<FramebufferBufferType> types)
{
    GL(Clear(types.value()));
}
void FramebufferBinder::Clear()
{
    GL(Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void FramebufferBinder::DrawArrays(PrimitiveType primitiveType, const RenderProgramBinder& programBinder, const VertexArrayBinder& vertexArrayBinder, uint offset, uint count) const
{
    GL(DrawArrays(GraphicsEnums::primitiveTypes.ToValue(primitiveType), offset, count));
}
void FramebufferBinder::DrawArraysInstanced(PrimitiveType primitiveType, const RenderProgramBinder& programBinder, const VertexArrayBinder& vertexArrayBinder, uint offset, uint elements, uint instances) const
{
    GL(DrawArraysInstanced(GraphicsEnums::primitiveTypes.ToValue(primitiveType), offset, elements, instances));
}

void FramebufferBinder::DrawElements(PrimitiveType primitiveType, IndexType indexType, uint count) const
{
    int primitive = GraphicsEnums::primitiveTypes.ToValue(primitiveType);
    int index = GraphicsEnums::indexTypes.ToValue(indexType);
    GL(DrawElements(primitive, count, index, 0));
}
void FramebufferBinder::DrawElementsInstanced(PrimitiveType primitiveType, IndexType indexType, uint elements, uint instances) const
{
    int primitive = GraphicsEnums::primitiveTypes.ToValue(primitiveType);
    int index = GraphicsEnums::indexTypes.ToValue(indexType);
    GL(DrawElementsInstanced(primitive, elements, index, 0, instances));
}

}