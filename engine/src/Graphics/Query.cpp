#include "Query.hpp"

#include "OpenGL.hpp"

namespace Morph {

GraphicsQueryImpl::GraphicsQueryImpl(uint id)
    : m_id(id)
{
}
uint GraphicsQueryImpl::Gen()
{
    uint id;
    GL(GenQueries(1, &id));
    return id;
}

GraphicsQueryImpl::GraphicsQueryImpl(GraphicsQueryImpl&& old)
{
    std::swap(m_id, old.m_id);
}
GraphicsQueryImpl& GraphicsQueryImpl::operator=(GraphicsQueryImpl&& old)
{
    std::swap(m_id, old.m_id);
    return *this;
}
GraphicsQueryImpl::~GraphicsQueryImpl()
{
    if(m_id != 0) {
        GL(DeleteQueries(1, &m_id));
    }
}

u64 GraphicsQueryImpl::GetValueBlocking() const
{
    u64 value;
    GL(GetQueryObjectui64v(m_id, GL_QUERY_RESULT, &value));
    return value;
}
u64 GraphicsQueryImpl::GetValueNoWait() const
{
    u64 value;
    GL(GetQueryObjectui64v(m_id, GL_QUERY_RESULT_NO_WAIT, &value));
    return value;
}
bool GraphicsQueryImpl::IsValueAvailable() const
{
    int available;
    GL(GetQueryObjectiv(m_id, GL_QUERY_RESULT_AVAILABLE, &available));
    return available == GL_TRUE;
}


void GraphicsQueryScopeImpl::Begin(GraphicsQueryType queryType, uint id)
{
    GL(BeginQuery(GraphicsEnums::queryTypes.ToValue(queryType), id));
}
void GraphicsQueryScopeImpl::End(GraphicsQueryType queryType)
{
    GL(EndQuery(GraphicsEnums::queryTypes.ToValue(queryType)));
}

}