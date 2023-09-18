
#ifndef MORPH_VERTEX_ARRAY_HPP
#define MORPH_VERTEX_ARRAY_HPP

#include <Core/Core.hpp>

#include "Buffer.hpp"

namespace Morph {

class GraphicsContext;
class VertexArrayBinder;

class VertexArray
{
    friend class GraphicsContext;
    friend class VertexArrayBinder;
protected:
    uint m_id = 0;
    uint m_elementCount = 0;
protected:
    VertexArray(uint id) : m_id(id) {}
public:
    VertexArray() {}
    VertexArray(const VertexArray& other) = delete;
    VertexArray(VertexArray&& other);
    VertexArray& operator=(VertexArray&& other);
    virtual ~VertexArray();

    inline uint id() const { return m_id; }
    inline operator bool() const { return m_id != 0; }
protected:
    static uint Gen();
    void BindVertexArray(uint id);
    void BindIndexBuffer(uint id);
    void BindVertexBuffer(uint id);
};

class VertexArrayBinder
{
    friend class GraphicsContext;
    struct BinderData {
        ref<VertexArray> vertexArray;
        ref<bool> bound;
    };
protected:
    opt<BinderData> m_binderData;
protected:
    VertexArrayBinder(VertexArray& vertexArray, bool& bound);
public:
    VertexArrayBinder();
    VertexArrayBinder(const VertexArrayBinder& other) = delete;
    VertexArrayBinder(VertexArrayBinder&& other);
    VertexArrayBinder& operator=(VertexArrayBinder&& other);
    virtual ~VertexArrayBinder();

    template<typename VertexType, bool DYNAMIC=false, bool MR=false, bool MW=false>
    void AddVertexBuffer(VertexBuffer<VertexType, DYNAMIC,MR,MW>& vertexBuffer) {
        Get().BindVertexBuffer(vertexBuffer.id());
        VertexArray& vertexArray = Get();
        const VertexLayout& layout = vertexBuffer.GetLayout();
        for(const VertexElement& element: layout.Elements()) {
            EnableVertexAttribArray(vertexArray.m_elementCount);
            VertexAttribPointer(
                vertexArray.m_elementCount,
                element.Count(),
                element.TypeIdentifier(),
                element.IsNormalized(),
                layout.GetStride(),
                (const void*)(intptr_t)element.GetOffset()
            );
            VertexAttribDivisor(vertexArray.m_elementCount, element.GetDivisor());
            vertexArray.m_elementCount++;
        }
        Get().BindVertexBuffer(0);
    }
    VertexArray& Get() { return m_binderData.value().vertexArray; }
    inline operator bool() const { return m_binderData.has_value(); }
protected:
    void EnableVertexAttribArray(uint i);
    void VertexAttribPointer(uint i, uint count, uint type, bool normalized, uint stride, const void* offset);
    void VertexAttribDivisor(uint i, uint divisor);
};

template<typename INDEX_TYPE = u32>
class IndexedVertexArray : public VertexArray
{
friend class GraphicsContext;
protected:
    template<bool DYNAMIC=false, bool MR=false, bool MW=false>
    IndexedVertexArray(uint id, IndexBuffer<INDEX_TYPE, DYNAMIC,MR,MW>& indexBuffer) : VertexArray(id) {
        BindVertexArray(id);
        BindIndexBuffer(indexBuffer.id());
        BindVertexArray(0);
        BindIndexBuffer(0);
    }
public:
    IndexedVertexArray() : VertexArray() {}
};

template<typename INDEX_TYPE = u32>
class IndexedVertexArrayBinder : public VertexArrayBinder
{
    friend class GraphicsContext;
protected:
    IndexedVertexArrayBinder(IndexedVertexArray<INDEX_TYPE>& vertexArray, bool& bound) : VertexArrayBinder(vertexArray, bound) {}
public:
    IndexedVertexArrayBinder() : VertexArrayBinder() {}

    IndexedVertexArray<INDEX_TYPE>& GetIndexed() { return dynamic_cast<IndexedVertexArray<INDEX_TYPE>&>(Get()); }
};

}

#endif // MORPH_VERTEX_ARRAY_HPP