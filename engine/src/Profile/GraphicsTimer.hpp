
#ifndef MORPH_PROFILE_GRAPHICS_TIMER_HPP
#define MORPH_PROFILE_GRAPHICS_TIMER_HPP

#include <Graphics/Context.hpp>

namespace Morph {

template<uint QUERIES_COUNT>
class GraphicsTimer
{
private:
    usize m_useIndex = 0;
    ref<GraphicsContext> m_context;
    array<GraphicsTimeElapsedQuery, QUERIES_COUNT> m_timerQueries;
    array<bool, QUERIES_COUNT> m_timerQueriesUsed;
public:
    GraphicsTimer(GraphicsContext& context) : m_context(context) {
        for(GraphicsTimeElapsedQuery& timerQuery : m_timerQueries) {
            timerQuery = context.CreateQuery<GraphicsQueryType::TIME_ELAPSED>();
        }
        m_timerQueriesUsed.fill(false);
    }

    GraphicsTimeElapsedQueryScope Begin() {
        GraphicsContext& context = m_context;
        GraphicsTimeElapsedQueryScope scope = context.BeginQuery(m_timerQueries[m_useIndex]);
        m_timerQueriesUsed[m_useIndex] = true;
        m_useIndex = (m_useIndex + 1) % QUERIES_COUNT;
        return scope;
    }

    // time in nanoseconds
    u64 GetAvgTime() const {
        u64 avg = 0;
        u64 count = 0;
        usize i = 0;
        for(const GraphicsTimeElapsedQuery& timerQuery : m_timerQueries) {
            if(m_timerQueriesUsed[m_useIndex] && timerQuery.IsValueAvailable()) {
                avg += timerQuery.GetValueNoWait();
                count++;
            }
            i++;
        }
        if(count == 0) {
            return 0;
        }
        return avg / count;
    }
};

}

#endif // MORPH_PROFILE_GRAPHICS_TIMER_HPP