
#ifndef MORPH_MONITOR_HPP
#define MORPH_MONITOR_HPP

#include <Core/Core.hpp>

struct GLFWmonitor;

namespace Morph {

class WindowManager;

using MonitorID = GLFWmonitor*;

struct MonitorVideoMode
{
    ivec2 resolution;
    ivec3 bitDepths;
    int refreshRate; // Hz
};

struct MonitorWorkArea
{
    ivec2 pos;
    ivec2 size;
};

enum class MonitorState : int
{
    CONNECTED,
    DISCONNECTED
};

struct MonitorEvent
{
    MonitorID monitor;
    MonitorState state;
};

class Monitor
{
    friend class WindowManager;
private:
    MonitorID m_id;
    vector<MonitorVideoMode> m_videoModes;
private:
    Monitor(MonitorID id);
public:
    inline MonitorID id() const { return m_id; }
    ivec2 GetPhysicalSize_mm() const;
    vec2 GetContentScale() const;
    ivec2 GetPos() const;
    MonitorWorkArea GetWorkArea() const;
    string GetName() const;
    MonitorVideoMode GetVideoMode() const;
    const vector<MonitorVideoMode>& GetVideoModes() const;
};

}

#endif // MORPH_MONITOR_HPP