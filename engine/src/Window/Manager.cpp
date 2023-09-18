#include "Manager.hpp"

#include <Graphics/OpenGL.hpp>

namespace Morph {


EnumMap<WindowManagerErrorType> WindowManager::s_errorTypeValues = {
    GLFW_NO_ERROR,
    GLFW_NOT_INITIALIZED,
    GLFW_NO_CURRENT_CONTEXT,
    GLFW_INVALID_ENUM,
    GLFW_INVALID_VALUE,
    GLFW_OUT_OF_MEMORY,
    GLFW_API_UNAVAILABLE,
    GLFW_VERSION_UNAVAILABLE,
    GLFW_PLATFORM_ERROR,
    GLFW_FORMAT_UNAVAILABLE,
    GLFW_NO_WINDOW_CONTEXT
};
EnumMap<MonitorState> WindowManager::s_monitorStateValues = {
    GLFW_CONNECTED,
    GLFW_DISCONNECTED
};

mutex WindowManager::s_instanceMutex;
WindowManager* WindowManager::s_instance = nullptr;

WindowManager* WindowManager::Get()
{
    std::lock_guard<mutex> instanceLock(s_instanceMutex);
    if(s_instance == nullptr && glfwInit()) {
        s_instance = new WindowManager();
    }
    return s_instance;
}
WindowManager::~WindowManager()
{
    std::lock_guard<mutex> instanceLock(s_instanceMutex);
    glfwTerminate();
    s_instance = nullptr;
}
WindowManager::WindowManager()
    : m_windowsIterators(m_windows), m_monitorsIterators(m_monitors)
{
    m_defaultHints.Add(WindowClientAPIHint(WindowClientAPI::OPENGL));
    m_defaultHints.Add(WindowContextVersionHint(4,5));
    SetupMonitors();
    glfwSetErrorCallback(&ErrorCallback);
    glfwSetMonitorCallback(&MonitorCallback);
}

WindowManagerError WindowManager::GetError() const
{
    WindowManagerError error;
    const char* description;
    error.type = static_cast<WindowManagerErrorType>(glfwGetError(&description));
    error.message = description;
    return error;
}

opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title)
{
    m_defaultHints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const Monitor& monitor)
{
    m_defaultHints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), monitor.id(), nullptr);
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const Window& share)
{
    m_defaultHints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, share.id());
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const Monitor& monitor, const Window& share)
{
    m_defaultHints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), monitor.id(), share.id());
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const WindowHints& hints)
{
    m_defaultHints.Set();
    hints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const Monitor& monitor, const WindowHints& hints)
{
    m_defaultHints.Set();
    hints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), monitor.id(), nullptr);
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const Window& share, const WindowHints& hints)
{
    m_defaultHints.Set();
    hints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, share.id());
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
opt<WindowID> WindowManager::CreateWindow(ivec2 size, const string& title, const Monitor& monitor, const Window& share, const WindowHints& hints)
{
    m_defaultHints.Set();
    hints.Set();
    WindowID id = glfwCreateWindow(size.x, size.y, title.c_str(), monitor.id(), share.id());
    if(id) {
        AddWindow(id);
        return id;
    }
    return {};
}
bool WindowManager::DestroyWindow(WindowID id)
{
    if(m_windows.find(id) != m_windows.end()) {
        glfwDestroyWindow(id);
        m_windows.erase(id);
        return true;
    }
    return false;
}
opt<ref<Window>> WindowManager::GetWindow(WindowID id)
{
    auto it = m_windows.find(id);
    if(it != m_windows.end()) {
        return it->second;
    }
    return {};
}
opt<cref<Window>> WindowManager::GetWindow(WindowID id) const
{
    auto it = m_windows.find(id);
    if(it != m_windows.end()) {
        return it->second;
    }
    return {};
}


MonitorID WindowManager::GetPrimaryMonitorID()
{
    return glfwGetPrimaryMonitor();
}
opt<ref<Monitor>> WindowManager::GetPrimaryMonitor()
{
    MonitorID id = glfwGetPrimaryMonitor();
    return GetMonitor(id);
}
opt<ref<Monitor>> WindowManager::GetMonitor(MonitorID id)
{
    auto it = m_monitors.find(id);
    if(it != m_monitors.end()) {
        return it->second;
    }
    return {};
}
opt<cref<Monitor>> WindowManager::GetMonitor(MonitorID id) const
{
    auto it = m_monitors.find(id);
    if(it != m_monitors.end()) {
        return it->second;
    }
    return {};
}
void WindowManager::PollEvents()
{
    glfwPollEvents();
}
void WindowManager::WaitEvents()
{
    glfwWaitEvents();
}
void WindowManager::WaitEventsWithTimeout(double timeoutSeconds)
{
    glfwWaitEventsTimeout(timeoutSeconds);
}
void WindowManager::WakeUpFromWaitEvents()
{
    glfwPostEmptyEvent();
}

string WindowManager::GetKeyName(Key key, int scancode)
{
    const char* name = glfwGetKeyName(static_cast<int>(key), scancode);
    return string(name == nullptr ? "no name" : name);
}

void WindowManager::AddWindow(WindowID id)
{
    m_windows.insert({id, Window(id)});
    glfwSetWindowCloseCallback(id, &CloseCallback);
    glfwSetWindowSizeCallback(id, &SizeCallback);
    glfwSetFramebufferSizeCallback(id, &FramebufferSizeCallback);
    glfwSetWindowContentScaleCallback(id, &ContentScaleCallback);
    glfwSetWindowIconifyCallback(id, &MinimizeCallback);
    glfwSetWindowMaximizeCallback(id, &MaximizeCallback);
    glfwSetWindowFocusCallback(id, &FocusCallback);
    glfwSetKeyCallback(id, &KeyCallback);
    glfwSetCharCallback(id, &CharacterCallback);
    glfwSetCursorPosCallback(id, &CursorPositionCallback);
    glfwSetCursorEnterCallback(id, &CursorEnterCallback);
    glfwSetMouseButtonCallback(id, &MouseButtonCallback);
    glfwSetScrollCallback(id, &ScrollCallback);
    glfwSetDropCallback(id, &DropCallback);
}
void WindowManager::SetupMonitors()
{
    int monitorsCount;
    GLFWmonitor** glfwMonitors = glfwGetMonitors(&monitorsCount);
    for (usize i = 0; i < monitorsCount; i++) {
        m_monitors.insert({glfwMonitors[i], Monitor(glfwMonitors[i])});
    }  
}


void WindowManager::ErrorCallback(int a_code, const char* a_message)
{
    WindowManagerError event;
    event.type = s_errorTypeValues.ToEnum(a_code);
    event.message = a_message;
    ExecuteEvent(event);
}
void WindowManager::MonitorCallback(MonitorID a_id, int a_event)
{
    MonitorEvent event;
    event.monitor = a_id;
    if (a_event == GLFW_CONNECTED) {
        event.state = MonitorState::CONNECTED;
        s_instance->m_monitors.insert({a_id, Monitor(a_id)});
    }
    else if (a_event == GLFW_DISCONNECTED) {
        event.state = MonitorState::DISCONNECTED;
        s_instance->m_monitors.erase(a_id);
    }
    ExecuteEvent(event);
}
void WindowManager::CloseCallback(WindowID a_id)
{
    WindowCloseEvent event;
    event.window = a_id;
    ExecuteEvent(a_id, event);
}
void WindowManager::SizeCallback(WindowID a_id, int a_width, int a_height)
{
    WindowSizeEvent event;
    event.window = a_id;
    event.size = {a_width, a_height};
    ExecuteEvent(a_id, event);
}
void WindowManager::FramebufferSizeCallback(WindowID a_id, int a_width, int a_height)
{
    WindowFramebufferSizeEvent event;
    event.window = a_id;
    event.size = {a_width, a_height};
    ExecuteEvent(a_id, event);
}
void WindowManager::ContentScaleCallback(WindowID a_id, float a_xscale, float a_yscale)
{
    WindowContentScaleEvent event;
    event.window = a_id;
    event.scale = {a_xscale, a_yscale};
    ExecuteEvent(a_id, event);
}
void WindowManager::MinimizeCallback(WindowID a_id, int a_minimized)
{
    WindowMinimizeEvent event;
    event.window = a_id;
    event.minimized = a_minimized;
    ExecuteEvent(a_id, event);
    if(!event.minimized) {
        WindowRestoreEvent restEvent;
        restEvent.window = a_id;
        ExecuteEvent(a_id, restEvent);
    }
}
void WindowManager::MaximizeCallback(WindowID a_id, int a_maximized)
{
    WindowMaximizeEvent event;
    event.window = a_id;
    event.maximized = a_maximized;
    ExecuteEvent(a_id, event);
    if(!event.maximized) {
        WindowRestoreEvent restEvent;
        restEvent.window = a_id;
        ExecuteEvent(a_id, restEvent);
    }
}
void WindowManager::FocusCallback(WindowID a_id, int a_focused)
{
    WindowFocusEvent event;
    event.window = a_id;
    event.focused = a_focused;
    ExecuteEvent(a_id, event);
}
void WindowManager::KeyCallback(WindowID a_id, int key, int scancode, int action, int mods)
{
    KeyEvent event;
    event.window = a_id;
    event.key = Window::s_keyValues.ToEnum(key);
    event.scancode = scancode;
    event.action = static_cast<KeyAction>(action);
    event.mods = mods;
    ExecuteEvent(a_id, event);
}
void WindowManager::CharacterCallback(WindowID a_id, uint codepoint)
{
    CharEvent event;
    event.window = a_id;
    event.codepoint = codepoint;
    ExecuteEvent(a_id, event);
}
void WindowManager::CursorPositionCallback(WindowID a_id, double xpos, double ypos)
{
    CursorPosEvent event;
    event.window = a_id;
    event.screenPos = dvec2(xpos, ypos);
    Window& window = s_instance->m_windows.find(a_id)->second;
    event.eyePos = window.ScreenPosToEyePos(event.screenPos);
    if(window.m_lastCursorPosValid) {
        window.m_lastCursorDir = event.eyePos - window.m_lastCursorPos;
        window.m_lastCursorPos = event.eyePos;
    }
    window.m_lastCursorPosValid = true;
    event.eyeDir = window.m_lastCursorDir;
    ExecuteEvent(a_id, event);
}
void WindowManager::CursorEnterCallback(WindowID a_id, int entered)
{
    CursorEnterEvent event;
    event.window = a_id;
    event.entered = entered;
    Window& window = s_instance->m_windows.find(a_id)->second;
    window.m_lastCursorPosValid = false;
    ExecuteEvent(a_id, event);
}
void WindowManager::MouseButtonCallback(WindowID a_id, int button, int action, int mods)
{
    MouseButtonEvent event;
    event.window = a_id;
    event.button = static_cast<MouseButton>(button);
    event.pressed = static_cast<bool>(action);
    event.mods = mods;
    ExecuteEvent(a_id, event);
}
void WindowManager::ScrollCallback(WindowID a_id, double xoffset, double yoffset)
{
    ScrollEvent event;
    event.window = a_id;
    event.offset = dvec2(xoffset, yoffset);
    ExecuteEvent(a_id, event);
}
void WindowManager::DropCallback(WindowID a_id, int count, const char** paths)
{
    WindowDropEvent event;
    event.window = a_id;
    event.paths = vector<string>(count);
    for(int i = 0; i < count; ++i) {
        event.paths[i] = string(paths[i]);
    }
    ExecuteEvent(a_id, event);
}

}