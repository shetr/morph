#include "Buffer.hpp"

#include "OpenGL.hpp"

namespace Morph {

GraphicsBufferImpl::GraphicsBufferImpl(GraphicsBufferImpl&& buffer) : m_id(buffer.m_id)
{
    buffer.m_id = 0;
}
GraphicsBufferImpl& GraphicsBufferImpl::operator=(GraphicsBufferImpl&& buffer)
{
    m_id = buffer.m_id;
    buffer.m_id = 0;
    return *this;
}
GraphicsBufferImpl::~GraphicsBufferImpl()
{
    if(m_id != 0) {
        GL(DeleteBuffers(1, &m_id));
    }
}
uint GraphicsBufferImpl::Gen(GraphicsBufferTarget target)
{
    uint id;
    GL(GenBuffers(1, &id));
    return id;
}
void GraphicsBufferImpl::Bind(GraphicsBufferTarget target)
{
    GL(BindBuffer(GraphicsEnums::bufferTargets.ToValue(target), m_id));
}
void GraphicsBufferImpl::UnBind(GraphicsBufferTarget target)
{
    GL(BindBuffer(GraphicsEnums::bufferTargets.ToValue(target), 0));
}
void GraphicsBufferImpl::Storage(GraphicsBufferTarget target, usize size, const void* data, GraphicsBufferFlags flags)
{
    GL(BufferStorage(GraphicsEnums::bufferTargets.ToValue(target), size, data, 
        (flags.dynamic * GL_DYNAMIC_STORAGE_BIT) |
        (flags.mapRead * GL_MAP_READ_BIT) |
        (flags.mapWrite * GL_MAP_WRITE_BIT)
    ));
}
void GraphicsBufferImpl::Copy(uint readID, uint writeID, usize readOffset, usize writeOffset, usize size)
{
    GL(BindBuffer(GL_COPY_READ_BUFFER, readID));
    GL(BindBuffer(GL_COPY_WRITE_BUFFER, writeID));
    GL(CopyBufferSubData(GL_COPY_READ_BUFFER, GL_COPY_WRITE_BUFFER, readOffset, writeOffset, size));
}
void GraphicsBufferImpl::SubData(GraphicsBufferTarget target, usize offset, usize size, const void* data)
{
    GL(BufferSubData(GraphicsEnums::bufferTargets.ToValue(target), offset, size, data));
}
void GraphicsBufferImpl::SetBindBase(GraphicsBufferTarget target, uint bindBase)
{
    GL(BindBufferBase(GraphicsEnums::bufferTargets.ToValue(target), bindBase, m_id));
}

void GraphicsBufferMapperImpl::Bind(GraphicsBufferTarget target, uint id)
{
    GL(BindBuffer(GraphicsEnums::bufferTargets.ToValue(target), id));
}
void GraphicsBufferMapperImpl::UnBind(GraphicsBufferTarget target)
{
    GL(BindBuffer(GraphicsEnums::bufferTargets.ToValue(target), 0));
}
void* GraphicsBufferMapperImpl::Map(GraphicsBufferTarget target, GraphicsAccess access)
{
    void* res = GL(MapBuffer(GraphicsEnums::bufferTargets.ToValue(target), GraphicsEnums::accessValues.ToValue(access)));
    return res;
}
bool GraphicsBufferMapperImpl::UnMap(GraphicsBufferTarget target)
{
    bool res = GL(UnmapBuffer(GraphicsEnums::bufferTargets.ToValue(target)));
    return res;
}

void GraphicsBufferBinderImpl::Bind(GraphicsBufferTarget target, uint id)
{
    GL(BindBuffer(GraphicsEnums::bufferTargets.ToValue(target), id));
}
void GraphicsBufferBinderImpl::UnBind(GraphicsBufferTarget target)
{
    GL(BindBuffer(GraphicsEnums::bufferTargets.ToValue(target), 0));
}

uint VertexElement::TypeSize(VertexType type)
{
    switch (type)
    {
    case VertexType::INT8:
        return 1;
    case VertexType::INT16:
        return 2;
    case VertexType::INT32:
        return 4;
    case VertexType::UINT8:
        return 1;
    case VertexType::UINT16:
        return 2;
    case VertexType::UINT32:
        return 4;
    case VertexType::FLOAT16:
        return 2;
    case VertexType::FLOAT32:
        return 4;
    case VertexType::FLOAT64:
        return 8;
    case VertexType::FIXED:
        return 1;
    
    default:
        return 0;
    }
}
uint VertexElement::TypeSize(VertexPackedType type)
{
    return 4;
}

uint VertexElement::TypeCount(VertexPackedType type)
{
    switch (type)
    {
    case VertexPackedType::INT_2_10_10_10_REV:
        return 4;
    case VertexPackedType::UINT_2_10_10_10_REV:
        return 4;
    case VertexPackedType::UINT_10F_11F_11F_REV:
        return 3;
    default:
        return 0;
    }
}

int VertexElement::TypeIdentifier(VertexType type)
{
    int res = GraphicsEnums::vertexTypes.ToValue(type);
    return res;
}
int VertexElement::TypeIdentifier(VertexPackedType type)
{
    return GraphicsEnums::vertexPackedTypes.ToValue(type);
}

VertexLayout::VertexLayout(init_list<VertexElement> init) {
    for(const VertexElement& element: init) {
        AddElement(element);
    }
}
VertexLayout::VertexLayout(const vector<VertexElement>& elements) : m_elements(elements)
{
    for(VertexElement& element: m_elements){
        element.SetOffset(m_stride);
        m_stride += element.ByteSize();
    }
}

void VertexLayout::AddElement(const VertexElement& element)
{ 
    m_elements.push_back(element);
    m_elements.back().SetOffset(m_stride);
    m_stride += element.ByteSize();
}

int VertexLayout::ByteSize() const
{
    int size = 0;
    for(const VertexElement& element : m_elements) {
        size += element.ByteSize();
    }
    return size;
}

}