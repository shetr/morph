#include "Renderbuffer.hpp"

#include "OpenGL.hpp"

namespace Morph {


Renderbuffer::Renderbuffer(Renderbuffer&& renderbuffer)
{
    std::swap(m_id, renderbuffer.m_id);
    std::swap(m_format, renderbuffer.m_format);
    std::swap(m_dim, renderbuffer.m_dim);
    std::swap(m_samples, renderbuffer.m_samples);
}
Renderbuffer& Renderbuffer::operator=(Renderbuffer&& renderbuffer)
{
    std::swap(m_id, renderbuffer.m_id);
    std::swap(m_format, renderbuffer.m_format);
    std::swap(m_dim, renderbuffer.m_dim);
    std::swap(m_samples, renderbuffer.m_samples);
    return *this;
}
Renderbuffer::~Renderbuffer()
{
    if(m_id != 0) {
        GL(DeleteRenderbuffers(1, &m_id));
    }
}

void Renderbuffer::Resize(uvec2 dim)
{
    m_dim = dim;
    if(m_samples == 0) {
        SetStorage(dim, m_format);
    } else {
        SetStorage(dim, m_format, m_samples);
    }
}
void Renderbuffer::ChangeSamples(uint samples)
{
    m_samples = samples;
    SetStorage(m_dim, m_format, m_samples);
}
void Renderbuffer::ResizeChangeSamples(uvec2 dim, uint samples)
{
    m_dim = dim;
    m_samples = samples;
    SetStorage(dim, m_format, m_samples);
}
void Renderbuffer::SetNoSamples()
{
    m_samples = 0;
    SetStorage(m_dim, m_format);
}

Renderbuffer::Renderbuffer(uvec2 dim, TextureSizedFormat format) : m_format(format), m_dim(dim)
{
    GL(CreateRenderbuffers(1, &m_id));
    SetStorage(dim, format);
}
Renderbuffer::Renderbuffer(uvec2 dim, TextureSizedFormat format, uint samples) : m_format(format), m_dim(dim), m_samples(samples)
{
    GL(CreateRenderbuffers(1, &m_id));
    SetStorage(dim, format, samples);
}

void Renderbuffer::SetStorage(uvec2 dim, TextureSizedFormat format)
{
    GL(BindRenderbuffer(GL_RENDERBUFFER, m_id));
    GL(RenderbufferStorage(GL_RENDERBUFFER, GraphicsEnums::textureSizedFormats.ToValue(format), dim.x, dim.y));
    GL(BindRenderbuffer(GL_RENDERBUFFER, 0));
}
void Renderbuffer::SetStorage(uvec2 dim, TextureSizedFormat format, uint samples)
{
    GL(BindRenderbuffer(GL_RENDERBUFFER, m_id));
    GL(RenderbufferStorageMultisample(GL_RENDERBUFFER, samples, GraphicsEnums::textureSizedFormats.ToValue(format), dim.x, dim.y));
    GL(BindRenderbuffer(GL_RENDERBUFFER, 0));
}

}