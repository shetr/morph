
#ifndef MORPH_GRAPHICS_BUFFER_HPP
#define MORPH_GRAPHICS_BUFFER_HPP

#include <Core/Core.hpp>

#include "Enums.hpp"

namespace Morph {

class GraphicsContext;

struct GraphicsBufferFlags
{
    bool dynamic;
    bool mapRead;
    bool mapWrite;
};

class GraphicsBufferImpl
{
    friend class GraphicsContext;
protected:
    uint m_id = 0;
protected:
    GraphicsBufferImpl(uint id) : m_id(id) {}
    virtual ~GraphicsBufferImpl();
    static uint Gen(GraphicsBufferTarget target);
    void Bind(GraphicsBufferTarget target);
    void UnBind(GraphicsBufferTarget target);
    void Storage(GraphicsBufferTarget target, usize size, const void* data, GraphicsBufferFlags flags);
    static void Copy(uint readID, uint writeID, usize readOffset, usize writeOffset, usize size);
    void SubData(GraphicsBufferTarget target, usize offset, usize size, const void* data);
    void SetBindBase(GraphicsBufferTarget target, uint bindBase);
public:
    GraphicsBufferImpl() : m_id(0) {}
    GraphicsBufferImpl(const GraphicsBufferImpl& buffer) = delete;
    GraphicsBufferImpl(GraphicsBufferImpl&& buffer);
    GraphicsBufferImpl& operator=(GraphicsBufferImpl&& buffer);
    uint id() const { return m_id; }
    operator bool() const { return m_id != 0; }
};

template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false>
class GraphicsBuffer : public GraphicsBufferImpl
{
    friend class GraphicsContext;
protected:
    usize m_size;
protected:
    GraphicsBuffer(usize size) : GraphicsBuffer(size, nullptr) {}
    GraphicsBuffer(usize size, const void* data) : GraphicsBufferImpl(Gen(TARGET)), m_size(size) {
        Bind(TARGET);
        Storage(TARGET, size, data, {DYNAMIC, MAP_READ, MAP_WRITE});
        UnBind(TARGET);
    }
public:
    GraphicsBuffer() : GraphicsBufferImpl() {}

    void SetData(usize offset, usize size, const void* data) {
        Bind(TARGET);
        GraphicsBufferImpl::SubData(TARGET, offset, size, data);
        UnBind(TARGET);
    }
    void SetBindBase(uint bindBase) {
        static_assert(TARGET == GraphicsBufferTarget::ATOMIC_COUNTER || 
        TARGET == GraphicsBufferTarget::TRANSFORM_FEEDBACK || 
        TARGET == GraphicsBufferTarget::UNIFORM || 
        TARGET == GraphicsBufferTarget::SHADER_STORAGE);
        Bind(TARGET);
        GraphicsBufferImpl::SetBindBase(TARGET, bindBase);
        UnBind(TARGET);
    }

    GraphicsBufferTarget GetTarget() const { return TARGET; }
    bool IsDynamic() const { return DYNAMIC; }
    bool HasMapRead() const { return MAP_READ; }
    bool HasMapWrite() const { return MAP_WRITE; }
    bool IsMappable() const { return MAP_READ || MAP_WRITE; }
    constexpr bool HasBindBase() const { return 
        TARGET == GraphicsBufferTarget::ATOMIC_COUNTER || 
        TARGET == GraphicsBufferTarget::TRANSFORM_FEEDBACK || 
        TARGET == GraphicsBufferTarget::UNIFORM || 
        TARGET == GraphicsBufferTarget::SHADER_STORAGE;
    }
    usize size() const { return m_size; }
};


class GraphicsBufferMapperImpl
{
    friend class GraphicsContext;
protected:
    void Bind(GraphicsBufferTarget target, uint id);
    void UnBind(GraphicsBufferTarget target);
    void* Map(GraphicsBufferTarget target, GraphicsAccess access);
    bool UnMap(GraphicsBufferTarget target);
};

template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MAP_READ=true, bool MAP_WRITE=true>
class GraphicsBufferMapper : public GraphicsBufferMapperImpl {
    friend class GraphicsContext;
    using GraphicsBufferType = GraphicsBuffer<TARGET, DYNAMIC, MAP_READ, MAP_WRITE>;
    static_assert(MAP_READ || MAP_WRITE);
private:
    opt<ref<GraphicsBufferType>> m_buffer;
    void* m_data = nullptr;
protected:
    GraphicsBufferMapper(GraphicsBufferType& buffer) : m_buffer({buffer}) {
        Bind(TARGET, buffer.id());
        m_data = Map(TARGET, GetAccess());
        UnBind(TARGET);
        assert(m_data != nullptr);
    }
public:
    GraphicsBufferMapper() {}
    GraphicsBufferMapper(const GraphicsBufferMapper& mapper) = delete;
    GraphicsBufferMapper(GraphicsBufferMapper&& old) { 
        std::swap(m_buffer, old.m_buffer); 
        std::swap(m_data, old.m_data); 
    }
    GraphicsBufferMapper& operator=(GraphicsBufferMapper&& old) {
        std::swap(m_buffer, old.m_buffer); 
        std::swap(m_data, old.m_data);
        return *this;
    }
    virtual ~GraphicsBufferMapper() {
        if(m_buffer) {
            Bind(TARGET, GetRaw().id());
            bool res = UnMap(TARGET);
            assert(res);
            UnBind(TARGET);
        }
    }
    inline GraphicsAccess GetAccess() const { 
        return GraphicsEnums::ConvertAccess(MAP_READ, MAP_WRITE);
    }
    GraphicsBufferType& GetRaw() { return m_buffer.value(); }
    void* GetDataRaw() { return m_data; }
    usize size() const { return GetRaw().size(); }
    inline operator bool() const { return m_buffer.has_value(); }
};

class GraphicsBufferBinderImpl
{
    friend class GraphicsContext;
protected:
    void Bind(GraphicsBufferTarget target, uint id);
    void UnBind(GraphicsBufferTarget target);
};

template<GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false>
class GraphicsBufferBinder : public GraphicsBufferBinderImpl {
    friend class GraphicsContext;
    using GraphicsBufferType = GraphicsBuffer<TARGET, DYNAMIC, MAP_READ, MAP_WRITE>;
    struct BinderData {
        ref<GraphicsBufferType> buffer;
        ref<bool> bound;
    };
private:
    opt<BinderData> m_data;
protected:
    GraphicsBufferBinder(GraphicsBufferType& buffer, bool& bound) : m_data({buffer, bound}) {
        Bind(TARGET, buffer.id());
        bound = true;
    }
public:
    GraphicsBufferBinder() {}
    GraphicsBufferBinder(const GraphicsBufferBinder& binder) = delete;
    GraphicsBufferBinder(GraphicsBufferBinder&& old) { 
        std::swap(m_data, old.m_data); 
    }
    GraphicsBufferBinder& operator=(GraphicsBufferBinder&& old) {
        std::swap(m_data, old.m_data);
        return *this;
    }
    virtual ~GraphicsBufferBinder() {
        if(m_data) {
            bool& bound = m_data.value().bound;
            bound = false;
            UnBind(TARGET);
        }
    }
    inline operator bool() const { return m_data.has_value(); }
};


template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false>
class GraphicsBufferTyped : public GraphicsBuffer<TARGET, DYNAMIC, MAP_READ, MAP_WRITE>
{
    friend class GraphicsContext;
    using GraphicsBufferBase = GraphicsBuffer<TARGET, DYNAMIC, MAP_READ, MAP_WRITE>;
private:
    usize m_count;
protected:
    GraphicsBufferTyped(usize count) : GraphicsBufferBase(count * sizeof(T)), m_count(count) {}
    GraphicsBufferTyped(usize count, const T* data) : GraphicsBufferBase(count * sizeof(T), data), m_count(count) {}
    GraphicsBufferTyped(const vector<T>& data) : GraphicsBufferTyped(data.size(), data.data()) {}
    template<usize COUNT>
    GraphicsBufferTyped(const array<T, COUNT>& data) : GraphicsBufferTyped(COUNT, data.data()) {}
public:
    void SetData(usize offset, usize count, const T* data) {
        GraphicsBufferImpl::Bind(TARGET);
        GraphicsBufferImpl::SubData(TARGET, offset * sizeof(T), count * sizeof(T), data);
        GraphicsBufferImpl::UnBind(TARGET);
    }

    usize count() const { return m_count; }
};

template<typename T, GraphicsBufferTarget TARGET, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false>
class GraphicsBufferStruct : public GraphicsBuffer<TARGET, DYNAMIC, MAP_READ, MAP_WRITE>
{
    friend class GraphicsContext;
    using GraphicsBufferBase = GraphicsBuffer<TARGET, DYNAMIC, MAP_READ, MAP_WRITE>;
protected:
    GraphicsBufferStruct() : GraphicsBufferBase(sizeof(T)) {}
    GraphicsBufferStruct(const T& data) : GraphicsBufferBase(sizeof(T), &data) {}

public:
    void SubData(const T& data) {
        GraphicsBufferImpl::Bind(TARGET);
        GraphicsBufferImpl::SubData(TARGET, 0, sizeof(T), &data);
        GraphicsBufferImpl::UnBind(TARGET);
    }
};

class VertexElement
{
private:
    int m_typeID;
    uint m_count;
    uint m_byteSize;
    bool m_normalized;
    uint m_offset = 0;
    uint m_divisor = 0;
public:
    VertexElement(VertexType type, VertexElementCount count)
        : m_typeID(TypeIdentifier(type)), m_count(static_cast<uint>(count)), m_byteSize(m_count*TypeSize(type)), m_normalized(false)
    {}
    VertexElement(VertexType type, VertexElementCount count, uint divisor)
        : m_typeID(TypeIdentifier(type)), m_count(static_cast<uint>(count)), m_byteSize(m_count*TypeSize(type)), m_normalized(false), m_divisor(divisor)
    {}
    VertexElement(VertexType type, VertexElementCount count, bool normalized)
        : m_typeID(TypeIdentifier(type)), m_count(static_cast<uint>(count)), m_byteSize(m_count*TypeSize(type)), m_normalized(normalized)
    {}
    VertexElement(VertexType type, VertexElementCount count, bool normalized, uint divisor)
        : m_typeID(TypeIdentifier(type)), m_count(static_cast<uint>(count)), m_byteSize(m_count*TypeSize(type)), m_normalized(normalized), m_divisor(divisor)
    {}

    VertexElement(VertexPackedType type)
        : m_typeID(TypeIdentifier(type)), m_count(TypeCount(type)), m_byteSize(m_count*TypeSize(type)), m_normalized(false)
    {}
    VertexElement(VertexPackedType type, uint divisor)
        : m_typeID(TypeIdentifier(type)), m_count(TypeCount(type)), m_byteSize(m_count*TypeSize(type)), m_normalized(false), m_divisor(divisor)
    {}
    VertexElement(VertexPackedType type, bool normalized)
        : m_typeID(TypeIdentifier(type)), m_count(TypeCount(type)), m_byteSize(m_count*TypeSize(type)), m_normalized(normalized)
    {}
    VertexElement(VertexPackedType type, bool normalized, uint divisor)
        : m_typeID(TypeIdentifier(type)), m_count(TypeCount(type)), m_byteSize(m_count*TypeSize(type)), m_normalized(normalized), m_divisor(divisor)
    {}

    inline uint Count() const { return m_count; }
    inline bool IsNormalized() const { return m_normalized; }
    inline uint ByteSize() const { return m_byteSize; }
    inline void SetOffset(uint offset) { m_offset = offset; }
    inline uint GetOffset() const { return m_offset; }
    inline void SetDivisor(uint divisor) { m_divisor = divisor; }
    inline uint GetDivisor() const { return m_divisor; }
    int TypeIdentifier() const { return m_typeID; }

    static uint TypeSize(VertexType type);
    static uint TypeSize(VertexPackedType type);
    static uint TypeCount(VertexPackedType type);
    static int TypeIdentifier(VertexType type);
    static int TypeIdentifier(VertexPackedType type);
};

class VertexLayout
{
private:
    vector<VertexElement> m_elements;
    uint m_stride = 0;
public:
    VertexLayout() : m_elements() {}
    VertexLayout(init_list<VertexElement> init);
    VertexLayout(const vector<VertexElement>& elements);

    void AddElement(const VertexElement& element);
    inline uint ElementsCount() const { return m_elements.size(); }
    inline const vector<VertexElement>& Elements() const { return m_elements; }
    int ByteSize() const;
    uint GetStride() const { return m_stride; }
};


template<typename VERTEX_TYPE, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false>
class VertexBuffer : public GraphicsBufferTyped<VERTEX_TYPE, GraphicsBufferTarget::ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>
{
    friend class GraphicsContext;
    using GraphicsBufferBase = GraphicsBufferTyped<VERTEX_TYPE, GraphicsBufferTarget::ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
private:
    cref<VertexLayout> m_layout;
protected:
    VertexBuffer(usize count, cref<VertexLayout> layout) : GraphicsBufferBase(count), m_layout(layout) {}
    VertexBuffer(usize count, const VERTEX_TYPE* data, cref<VertexLayout> layout) : GraphicsBufferBase(count, data), m_layout(layout) {}
    VertexBuffer(const vector<VERTEX_TYPE>& data, cref<VertexLayout> layout) : GraphicsBufferBase(data), m_layout(layout) {}
    template<usize COUNT>
    VertexBuffer(const array<VERTEX_TYPE, COUNT>& data, cref<VertexLayout> layout) : GraphicsBufferBase(data), m_layout(layout) {}
public:
    inline const VertexLayout& GetLayout() const { return m_layout; }
};

template<typename INDEX_TYPE = u32, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false>
class IndexBuffer : public GraphicsBufferTyped<INDEX_TYPE, GraphicsBufferTarget::ELEMENT_ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>
{
    friend class GraphicsContext;
    using GraphicsBufferBase = GraphicsBufferTyped<INDEX_TYPE, GraphicsBufferTarget::ELEMENT_ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
protected:
    IndexBuffer(usize count) : GraphicsBufferBase(count) {}
    IndexBuffer(usize count, const INDEX_TYPE* data) : GraphicsBufferBase(count, data) {}
    IndexBuffer(const vector<INDEX_TYPE>& data) : GraphicsBufferBase(data) {}
    template<usize COUNT>
    IndexBuffer(const array<INDEX_TYPE, COUNT>& data) : GraphicsBufferBase(data) {}
public:
    IndexType GetType() {
        if(type_index(typeid(INDEX_TYPE)) == type_index(typeid(u32))) {
            return IndexType::UINT32;
        }
        if(type_index(typeid(INDEX_TYPE)) == type_index(typeid(u16))) {
            return IndexType::UINT16;
        }
        if(type_index(typeid(INDEX_TYPE)) == type_index(typeid(u8))) {
            return IndexType::UINT8;
        }   
        panic("Index buffer type not supported");
        return IndexType::UINT32;
    }
};

template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ArrayBuffer = GraphicsBuffer<GraphicsBufferTarget::ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ElementArrayBuffer = GraphicsBuffer<GraphicsBufferTarget::ELEMENT_ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using UniformBuffer = GraphicsBuffer<GraphicsBufferTarget::UNIFORM, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ShaderStorageBuffer = GraphicsBuffer<GraphicsBufferTarget::SHADER_STORAGE, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using AtomicCounterBuffer = GraphicsBuffer<GraphicsBufferTarget::ATOMIC_COUNTER, DYNAMIC, MAP_READ, MAP_WRITE>;

template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ArrayBufferMapper = GraphicsBufferMapper<GraphicsBufferTarget::ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ElementArrayBufferMapper = GraphicsBufferMapper<GraphicsBufferTarget::ELEMENT_ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using UniformBufferMapper = GraphicsBufferMapper<GraphicsBufferTarget::UNIFORM, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ShaderStorageBufferMapper = GraphicsBufferMapper<GraphicsBufferTarget::SHADER_STORAGE, DYNAMIC, MAP_READ, MAP_WRITE>;
template<bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using AtomicCounterBufferMapper = GraphicsBufferMapper<GraphicsBufferTarget::ATOMIC_COUNTER, DYNAMIC, MAP_READ, MAP_WRITE>;

template<typename T, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ArrayBufferTyped = GraphicsBufferTyped<T, GraphicsBufferTarget::ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
template<typename T, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using ElementArrayBufferTyped = GraphicsBufferTyped<T, GraphicsBufferTarget::ELEMENT_ARRAY, DYNAMIC, MAP_READ, MAP_WRITE>;
template<typename T, bool DYNAMIC=false, bool MAP_READ=false, bool MAP_WRITE=false> using UniformBufferStruct = GraphicsBufferStruct<T, GraphicsBufferTarget::UNIFORM, DYNAMIC, MAP_READ, MAP_WRITE>;

}

#endif // MORPH_GRAPHICS_BUFFER_HPP