
#ifndef MORPH_VERTEX_DATA_HPP
#define MORPH_VERTEX_DATA_HPP

#include <Core/Core.hpp>
#include <Graphics/Context.hpp>

namespace Morph {

template<PrimitiveType PRIMITIVE_TYPE>
class VertexData
{
public:
    PrimitiveType GetPrimitiveType() const { return PRIMITIVE_TYPE; }
    virtual void Draw(GraphicsContext& context, const FramebufferBinder& framebufferBinder, const RenderProgramBinder& programBinder) = 0;
};

template<typename VERTEX_TYPE, typename INDEX_TYPE = u32>
struct IndexedVertices
{
    vector<VERTEX_TYPE> vertices;
    vector<INDEX_TYPE> indices;
};

// Storage specializations

template<PrimitiveType PRIMITIVE_TYPE>
class BasicVertexData : public VertexData<PRIMITIVE_TYPE>
{
public:
    virtual VertexArray& GetVertexArray() = 0;
    virtual uint GetOffset() const = 0;
    virtual uint GetCount() const = 0;

    inline void Draw(GraphicsContext& context, const FramebufferBinder& framebufferBinder, const RenderProgramBinder& programBinder) override {
        VertexArrayBinder vertexArrayBinder = context.BindVertexArray(GetVertexArray());
        framebufferBinder.DrawArrays(PRIMITIVE_TYPE, programBinder, vertexArrayBinder, GetOffset(), GetCount());
    }
};

template<PrimitiveType PRIMITIVE_TYPE, typename INDEX_TYPE = u32>
class IndexedVertexData : public VertexData<PRIMITIVE_TYPE>
{
public:
    virtual IndexedVertexArray<INDEX_TYPE>& GetVertexArray() = 0;
    virtual uint GetCount() const = 0;

    inline void Draw(GraphicsContext& context, const FramebufferBinder& framebufferBinder, const RenderProgramBinder& programBinder) override {
        IndexedVertexArrayBinder<INDEX_TYPE> vertexArrayBinder = context.BindIndexedVertexArray(GetVertexArray());
        framebufferBinder.DrawElements(PRIMITIVE_TYPE, programBinder, vertexArrayBinder, GetCount());
    }
};

template<PrimitiveType PRIMITIVE_TYPE>
class BasicVertexDataInstanced : public VertexData<PRIMITIVE_TYPE>
{
public:
    virtual VertexArray& GetVertexArray() = 0;
    virtual uint GetOffset() const = 0;
    virtual uint GetElementsCount() const = 0;
    virtual uint GetInstancesCount() const = 0;

    inline void Draw(GraphicsContext& context, const FramebufferBinder& framebufferBinder, const RenderProgramBinder& programBinder) override {
        VertexArrayBinder vertexArrayBinder = context.BindVertexArray(GetVertexArray());
        framebufferBinder.DrawArraysInstanced(PRIMITIVE_TYPE, programBinder, vertexArrayBinder, GetOffset(), GetElementsCount(), GetInstancesCount());
    }
};

template<PrimitiveType PRIMITIVE_TYPE, typename INDEX_TYPE = u32>
class IndexedVertexDataInstanced : public VertexData<PRIMITIVE_TYPE> 
{
public:
    virtual IndexedVertexArray<INDEX_TYPE>& GetVertexArray() = 0;
    virtual uint GetElementsCount() const = 0;
    virtual uint GetInstancesCount() const = 0;

    inline void Draw(GraphicsContext& context, const FramebufferBinder& framebufferBinder, const RenderProgramBinder& programBinder) override {
        IndexedVertexArrayBinder<INDEX_TYPE> vertexArrayBinder = context.BindIndexedVertexArray(GetVertexArray());
        framebufferBinder.DrawElementsInstanced(PRIMITIVE_TYPE, programBinder, vertexArrayBinder, GetElementsCount(), GetInstancesCount());
    }
};

// access specializations

template<PrimitiveType PRIMITIVE_TYPE, typename VERTEX_TYPE>
class BasicStaticVertexData : public BasicVertexData<PRIMITIVE_TYPE>
{
private:
    uint m_offset;
    uint m_count;
    VertexBuffer<VERTEX_TYPE> m_vertexBuffer;
    VertexArray m_vertexArray;
public:
    BasicStaticVertexData(GraphicsContext& context, const vector<VERTEX_TYPE>& data)
        : BasicStaticVertexData(context, data, 0, data.size()) {}
    BasicStaticVertexData(GraphicsContext& context, const vector<VERTEX_TYPE>& data, uint offset, uint count) 
        : m_offset(offset), m_count(count), 
        m_vertexBuffer(context.CreateVertexBuffer(data, VERTEX_TYPE::GetLayout())),
        m_vertexArray(context.CreateVertexArray())
    {
        VertexArrayBinder vertexArrayBinder = context.BindVertexArray(m_vertexArray);
        vertexArrayBinder.AddVertexBuffer(m_vertexBuffer);
    }
    VertexArray& GetVertexArray() override { return m_vertexArray; }
    uint GetOffset() const override { return m_offset; }
    uint GetCount() const override { return m_count; }
};

template<PrimitiveType PRIMITIVE_TYPE, typename VERTEX_TYPE, typename INDEX_TYPE = u32>
class IndexedStaticVertexData : public IndexedVertexData<PRIMITIVE_TYPE, INDEX_TYPE>
{
private:
    uint m_count;
    VertexBuffer<VERTEX_TYPE> m_vertexBuffer;
    IndexBuffer<INDEX_TYPE> m_indexBuffer;
    IndexedVertexArray<INDEX_TYPE> m_vertexArray;
public:
    IndexedStaticVertexData(GraphicsContext& context, const IndexedVertices<VERTEX_TYPE, INDEX_TYPE>& data)
        : m_count(data.indices.size()),
        m_vertexBuffer(context.CreateVertexBuffer(data.vertices, VERTEX_TYPE::GetLayout())),
        m_indexBuffer(context.CreateIndexBuffer<INDEX_TYPE>(data.indices)),
        m_vertexArray(context.CreateIndexedVertexArray(m_indexBuffer))
    {
        IndexedVertexArrayBinder<INDEX_TYPE> vertexArrayBinder = context.BindIndexedVertexArray(m_vertexArray);
        vertexArrayBinder.AddVertexBuffer(m_vertexBuffer);
    }
    IndexedVertexArray<INDEX_TYPE>& GetVertexArray() override { return m_vertexArray; }
    uint GetCount() const override { return m_count; }
};

}

#endif // MORPH_VERTEX_DATA_HPP