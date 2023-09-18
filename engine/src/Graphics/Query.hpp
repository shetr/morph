
#ifndef MORPH_GRAPHICS_QUERY_HPP
#define MORPH_GRAPHICS_QUERY_HPP

#include <Core/Core.hpp>

#include "Enums.hpp"

namespace Morph {

class GraphicsContext;

class GraphicsQueryImpl
{
    friend class GraphicsContext;
private:
    uint m_id = 0;
protected:
    GraphicsQueryImpl(uint id);
    static uint Gen();
    GraphicsQueryImpl() {}
public:
    GraphicsQueryImpl(const GraphicsQueryImpl& other) = delete;
    GraphicsQueryImpl(GraphicsQueryImpl&& old);
    GraphicsQueryImpl& operator=(GraphicsQueryImpl&& old);
    virtual ~GraphicsQueryImpl();
    uint id() const { return m_id; }
    inline operator bool() const { return m_id != 0; }

    u64 GetValueBlocking() const;
    u64 GetValueNoWait() const;
    bool IsValueAvailable() const;
};

template<GraphicsQueryType TYPE>
class GraphicsQuery : public GraphicsQueryImpl
{
    friend class GraphicsContext;
protected:
    GraphicsQuery(uint id) : GraphicsQueryImpl(id) {}
public:
    GraphicsQuery() : GraphicsQueryImpl() {}
    GraphicsQueryType GetType() const { return TYPE; }
};

// time is in nanoseconds
using GraphicsTimeElapsedQuery = GraphicsQuery<GraphicsQueryType::TIME_ELAPSED>;

class GraphicsQueryScopeImpl
{
protected:
    void Begin(GraphicsQueryType queryType, uint id);
    void End(GraphicsQueryType queryType);
};

template<GraphicsQueryType TYPE>
class GraphicsQueryScope : public GraphicsQueryScopeImpl
{
    friend class GraphicsContext;
    struct ScopeData {
        ref<GraphicsQuery<TYPE>> query;
        ref<bool> used;
    };
private:
    opt<ScopeData> m_data;
protected:
    GraphicsQueryScope(GraphicsQuery<TYPE>& query, bool& used) : m_data({query, used}) {
        GraphicsQueryScopeImpl::Begin(TYPE, query.id());
        used = true;
    }
public:
    GraphicsQueryScope() {}
    GraphicsQueryScope(const GraphicsQueryScope& other) = delete;
    GraphicsQueryScope(GraphicsQueryScope&& old) {
        std::swap(m_data, old.m_data);
    }
    GraphicsQueryScope& operator=(GraphicsQueryScope&& old) {
        std::swap(m_data, old.m_data);
        return *this;
    }
    virtual ~GraphicsQueryScope() {
        if(m_data) {
            bool& used = m_data.value().used;
            GraphicsQueryScopeImpl::End(TYPE);
            used = false;
        }
    }
    uint id() const { return m_data.value().query.get().id(); }
    inline operator bool() const { return m_data.has_value(); }
};

using GraphicsTimeElapsedQueryScope = GraphicsQueryScope<GraphicsQueryType::TIME_ELAPSED>;

}

#endif // MORPH_GRAPHICS_QUERY_HPP