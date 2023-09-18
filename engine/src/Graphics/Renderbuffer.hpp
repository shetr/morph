
#ifndef MORPH_RENDERBUFFER_HPP
#define MORPH_RENDERBUFFER_HPP

#include <Core/Core.hpp>

#include "Enums.hpp"

namespace Morph {

class GraphicsContext;

class Renderbuffer
{
    friend class GraphicsContext;
private:
    uint m_id = 0;
    TextureSizedFormat m_format = TextureSizedFormat::R8;
    uvec2 m_dim = uvec2(0,0);
    uint m_samples = 0;
public:
    Renderbuffer() {}
    Renderbuffer(const Renderbuffer& renderbuffer) = delete;
    Renderbuffer(Renderbuffer&& renderbuffer);
    Renderbuffer& operator=(Renderbuffer&& renderbuffer);
    virtual ~Renderbuffer();

    void Resize(uvec2 dim);
    void ChangeSamples(uint samples);
    void ResizeChangeSamples(uvec2 dim, uint samples);
    void SetNoSamples();

    uint id() const { return m_id; }
    TextureSizedFormat format() const { return m_format; }
    uvec2 dim() const { return m_dim; }
protected:
    Renderbuffer(uvec2 dim, TextureSizedFormat format);
    Renderbuffer(uvec2 dim, TextureSizedFormat format, uint samples);

    void SetStorage(uvec2 dim, TextureSizedFormat format);
    void SetStorage(uvec2 dim, TextureSizedFormat format, uint samples);
};

}

#endif // MORPH_RENDERBUFFER_HPP