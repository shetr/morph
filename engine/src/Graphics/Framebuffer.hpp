
#ifndef MORPH_FRAMEBUFFER_HPP
#define MORPH_FRAMEBUFFER_HPP

#include <Core/Core.hpp>
#include <Window/Window.hpp>

#include "Enums.hpp"
#include "Texture.hpp"
#include "Renderbuffer.hpp"
#include "VertexArray.hpp"
#include "Program.hpp"

namespace Morph {

class GraphicsContext;
class FramebufferBinder;

class Framebuffer
{
    friend class FramebufferBinder;
public:
    virtual uint id() const = 0;
    virtual uvec2 dim() const = 0;
protected:
    void OnBind();
    virtual void OnBindImpl() = 0;
};

class DefaultFramebuffer : public Framebuffer
{
    friend class GraphicsContext;
    friend class FramebufferBinder;
private:
    cref<Window> m_window;
private:
    DefaultFramebuffer(const Window& window) : m_window(window) {}
public:
    inline uint id() const override { return 0; }
    inline uvec2 dim() const override { return m_window.get().GetSize(); };

    void SetSwapWaitInterval(int screenUpdatesCount);
    void SwapBuffers();
protected:
    void OnBindImpl() override;
};

class CustomFramebuffer : public Framebuffer
{
    friend class GraphicsContext;
    friend class FramebufferBinder;
private:
    uint m_id = 0;
    uvec2 m_dim = uvec2(0);
    vector<uint> m_drawAttachments;
    vector<ref<Texture2D>> m_textures;
    vector<ref<TextureMultisample2D>> m_multisampleTextures;
    vector<ref<Renderbuffer>> m_renderbuffers;
public:
    CustomFramebuffer() {}
    CustomFramebuffer(const CustomFramebuffer& framebuffer) = delete;
    CustomFramebuffer(CustomFramebuffer&& framebuffer);
    CustomFramebuffer& operator=(CustomFramebuffer&& framebuffer);
    virtual ~CustomFramebuffer();

    void Resize(uvec2 dim);
    inline uint id() const override { return m_id; }
    inline uvec2 dim() const override { return m_dim; };

    void Attach(CustomFramebufferAttachment attachment, Texture2D& texture);
    void Attach(CustomFramebufferAttachment attachment, TextureMultisample2D& texture);
    void Attach(CustomFramebufferAttachment attachment, Renderbuffer& renderbuffer);

    void SetDrawAttachments(const vector<CustomFramebufferDrawAttachment>& attachments);
    operator bool() const { return m_id != 0; }
protected:
    CustomFramebuffer(uint id) : m_id(id) {}
    static uint Gen();

    void OnBindImpl() override;
};

class FramebufferBinder
{
    friend class GraphicsContext;
    struct BinderData {
        ref<Framebuffer> framebuffer;
        ref<bool> bound;
    };
private:
    opt<BinderData> m_data;
protected:
    FramebufferBinder(Framebuffer& framebuffer, bool& bound);
public:
    FramebufferBinder() {}
    FramebufferBinder(const FramebufferBinder& binder) = delete;
    FramebufferBinder(FramebufferBinder&& binder);
    FramebufferBinder& operator=(FramebufferBinder&& binder);
    virtual ~FramebufferBinder();

    void ClearColor();
    void Clear(EnumBits<FramebufferBufferType> types);
    void Clear();

    void DrawArrays(PrimitiveType primitiveType, const RenderProgramBinder& programBinder, const VertexArrayBinder& vertexArrayBinder, uint offset, uint count) const;
    template<typename INDEX_TYPE = u32>
    void DrawElements(PrimitiveType primitiveType, const RenderProgramBinder& programBinder, const IndexedVertexArrayBinder<INDEX_TYPE>& vertexArrayBinder, uint count) const {
        DrawElements(primitiveType, GraphicsEnums::GetIndexType<INDEX_TYPE>(), count);
    }
    void DrawArraysInstanced(PrimitiveType primitiveType, const RenderProgramBinder& programBinder, const VertexArrayBinder& vertexArrayBinder, uint offset, uint elements, uint instances) const;
    template<typename INDEX_TYPE = u32>
    void DrawElementsInstanced(PrimitiveType primitiveType, const RenderProgramBinder& programBinder, const IndexedVertexArrayBinder<INDEX_TYPE>& vertexArrayBinder, uint elements, uint instances) const {
        DrawElementsInstanced(primitiveType, GraphicsEnums::GetIndexType<INDEX_TYPE>(), elements, instances);
    }

    inline operator bool() const { return m_data.has_value(); }
private:
    void DrawElements(PrimitiveType primitiveType, IndexType indexType, uint count) const;
    void DrawElementsInstanced(PrimitiveType primitiveType, IndexType indexType, uint elements, uint instances) const;
};

}

#endif // MORPH_FRAMEBUFFER_HPP