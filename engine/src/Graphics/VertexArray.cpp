#include "VertexArray.hpp"

#include "OpenGL.hpp"

namespace Morph {


VertexArray::VertexArray(VertexArray&& other) : m_id(other.m_id), m_elementCount(other.m_elementCount)
{
    other.m_id = 0;
}
VertexArray& VertexArray::operator=(VertexArray&& other)
{
    m_id = other.m_id;
    m_elementCount = other.m_elementCount;
    other.m_id = 0;
    return *this;
}
VertexArray::~VertexArray()
{
    if(m_id != 0) {
        GL(DeleteVertexArrays(1, &m_id));
    }
}

uint VertexArray::Gen()
{
    uint id;
    GL(GenVertexArrays(1, &id));
    return id;
}

void VertexArray::BindVertexArray(uint id)
{
    GL(BindVertexArray(id));
}
void VertexArray::BindIndexBuffer(uint id)
{
    GL(BindBuffer(GL_ELEMENT_ARRAY_BUFFER, id));
}
void VertexArray::BindVertexBuffer(uint id)
{
    GL(BindBuffer(GL_ARRAY_BUFFER, id));
}


VertexArrayBinder::VertexArrayBinder(VertexArray& vertexArray, bool& bound) : m_binderData({vertexArray, bound})
{
    GL(BindVertexArray(vertexArray.id()));
    bound = true;
}
VertexArrayBinder::VertexArrayBinder() {}
VertexArrayBinder::VertexArrayBinder(VertexArrayBinder&& other)
{
    std::swap(m_binderData, other.m_binderData);
}
VertexArrayBinder& VertexArrayBinder::operator=(VertexArrayBinder&& other)
{
    std::swap(m_binderData, other.m_binderData);
    return *this;
}
VertexArrayBinder::~VertexArrayBinder()
{
    if(m_binderData) {
        GL(BindVertexArray(0));
        bool& bound = m_binderData.value().bound;
        bound = false;
    }
}

void VertexArrayBinder::EnableVertexAttribArray(uint i)
{
    GL(EnableVertexAttribArray(i));
}
void VertexArrayBinder::VertexAttribPointer(uint i, uint count, uint type, bool normalized, uint stride, const void* offset)
{
    GL(VertexAttribPointer(
        i,
        count,
        type,
        normalized,
        stride,
        offset
    ));
}
void VertexArrayBinder::VertexAttribDivisor(uint i, uint divisor)
{
    GL(VertexAttribDivisor(i, divisor));
}

}