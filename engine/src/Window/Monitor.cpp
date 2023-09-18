#include "Monitor.hpp"

#include <Graphics/OpenGL.hpp>

namespace Morph {

Monitor::Monitor(MonitorID id) 
    : m_id(id)
{
    int count;
    const GLFWvidmode* modes = glfwGetVideoModes(m_id, &count);
    m_videoModes.resize(count);
    for(int i = 0; i < count; i++) {
        m_videoModes[i].resolution = ivec2(modes[i].width, modes[i].height);
        m_videoModes[i].bitDepths = ivec3(modes[i].redBits, modes[i].greenBits, modes[i].blueBits);
        m_videoModes[i].refreshRate = modes[i].refreshRate;
    }
}

ivec2 Monitor::GetPhysicalSize_mm() const
{
    ivec2 res;
    glfwGetMonitorPhysicalSize(m_id, &res.x, &res.y);
    return res;
}
vec2 Monitor::GetContentScale() const
{
    vec2 res;
    glfwGetMonitorContentScale(m_id, &res.x, &res.y);
    return res;
}
ivec2 Monitor::GetPos() const
{
    ivec2 res;
    glfwGetMonitorPos(m_id, &res.x, &res.y);
    return res;
}
MonitorWorkArea Monitor::GetWorkArea() const
{
    MonitorWorkArea res;
    glfwGetMonitorWorkarea(m_id, &res.pos.x, &res.pos.y, &res.size.x, &res.size.y);
    return res;
}
string Monitor::GetName() const
{
    return glfwGetMonitorName(m_id);
}
MonitorVideoMode Monitor::GetVideoMode() const
{
    MonitorVideoMode res;
    const GLFWvidmode* vidMode = glfwGetVideoMode(m_id);
    res.resolution = ivec2(vidMode->width, vidMode->height);
    res.bitDepths = ivec3(vidMode->redBits, vidMode->greenBits, vidMode->blueBits);
    res.refreshRate = vidMode->refreshRate;
    return res;
}
const vector<MonitorVideoMode>& Monitor::GetVideoModes() const
{
    return m_videoModes;
}

}