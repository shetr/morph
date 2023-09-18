#ifndef MORPH_WINDOW_APP_CONFIG_HPP
#define MORPH_WINDOW_APP_CONFIG_HPP

#include <Window/Hint.hpp>

namespace Morph {

namespace ExecutionTypes
{
    struct RunOnce{};
    struct RunNumberOfFrames{ not_zero<uint> numberOfFrames = 1; };
    struct UnlimitedFrames{};
    struct VsyncFrames{ u64 screenUpdatesCount = 1; };
    struct AdaptiveVsyncFrames{ uint screenUpdatesCount = 1; };
    struct EventBasedFrames{};
    struct EventBasedFramesTimeout{ f64 timeoutSeconds = 0; };
    struct LimitedFrames{
    private:
        f64 m_maxFPS;
        f64 m_minFrameTime;
    public:
        LimitedFrames(f64 maxFPS = 60) 
        : m_maxFPS(maxFPS), m_minFrameTime(1 / maxFPS) {}
        
        inline f64 maxFPS() const { return m_maxFPS; }
        // time in seconds
        inline f64 minFrameTime() const { return m_minFrameTime; }
    };
}

using ExecutionTypeVar = variant<
    ExecutionTypes::RunOnce,
    ExecutionTypes::RunNumberOfFrames,
    ExecutionTypes::UnlimitedFrames,
    ExecutionTypes::VsyncFrames,
    ExecutionTypes::AdaptiveVsyncFrames,
    ExecutionTypes::LimitedFrames,
    ExecutionTypes::EventBasedFrames,
    ExecutionTypes::EventBasedFramesTimeout
>;

struct WindowAppConfig
{
    ivec2 size = ivec2(1200,800);
    string title = "";
    ExecutionTypeVar executionType = ExecutionTypes::VsyncFrames();
    WindowHints hints;

    WindowAppConfig CreateOffscreen(
        ivec2 _size = ivec2(1200,800), 
        string _title = "",
        ExecutionTypeVar _executionType = ExecutionTypes::VsyncFrames())
    {
        return {_size, _title, _executionType, WindowHints({WindowBoolHint(WindowBoolSetting::VISIBLE, false)})};
    }
};

}

#endif // MORPH_WINDOW_APP_CONFIG_HPP