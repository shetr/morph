#include "Window.hpp"

#include <Graphics/OpenGL.hpp>

namespace Morph {


uvec2 Window::GetSize() const
{
    uvec2 res;
    glfwGetWindowSize(m_id, (int*)&res.x, (int*)&res.y);
    return res;
}
void Window::SetSize(uvec2 size)
{
    glfwSetWindowSize(m_id, size.x, size.y);
}
WindowFrameSize Window::GetFrameSize() const
{
    WindowFrameSize res;
    glfwGetWindowFrameSize(m_id, &res.left, &res.top, &res.right, &res.bottom);
    return res;
}
vec2 Window::GetContentScale() const
{
    vec2 scale;
    glfwGetWindowContentScale(m_id, &scale.x, &scale.y);
    return scale;
}
bool Window::ShouldClose() const
{
    return glfwWindowShouldClose(m_id);
}
void Window::SetShouldClose(bool shouldClose)
{
    glfwSetWindowShouldClose(m_id, shouldClose);
}
WindowSizeLimits Window::GetSizeLimits() const
{
    return m_sizeLimits;
}
void Window::SetSizeLimits(WindowSizeLimits limits)
{
    m_sizeLimits = limits;
    glfwSetWindowSizeLimits(m_id, 
        limits.minWidth.value_or(GLFW_DONT_CARE),
        limits.minHeight.value_or(GLFW_DONT_CARE),
        limits.maxWidth.value_or(GLFW_DONT_CARE),
        limits.maxHeight.value_or(GLFW_DONT_CARE)
    );
}
opt<WindowAspectRatio> Window::GetFixedAspectRatio() const
{
    return m_fixedAspectRatio;
}
void Window::SetFixedAspectRatio(WindowAspectRatio aspectRatio)
{
    m_fixedAspectRatio = aspectRatio;
    glfwSetWindowAspectRatio(m_id, aspectRatio.numer, aspectRatio.denom);
}
ivec2 Window::GetPos() const
{
    ivec2 pos;
    glfwGetWindowPos(m_id, &pos.x, &pos.y);
    return pos;
}
void Window::SetPos(ivec2 pos)
{
    glfwSetWindowPos(m_id, pos.x, pos.y);
}
void Window::SetTitle(const string& title)
{
    glfwSetWindowTitle(m_id, title.c_str());
}
bool Window::IsResizableByUser() const
{
    return glfwGetWindowAttrib(m_id, GLFW_RESIZABLE);
}
void Window::SetResizableByUser(bool resizable)
{
    glfwSetWindowAttrib(m_id, GLFW_RESIZABLE, resizable);
}
bool Window::IsFloating() const
{
    return glfwGetWindowAttrib(m_id, GLFW_FLOATING);
}
void Window::SetFloating(bool floating)
{
    glfwSetWindowAttrib(m_id, GLFW_FLOATING, floating);
}
bool Window::IsDecorated() const
{
    return glfwGetWindowAttrib(m_id, GLFW_DECORATED);
}
void Window::SetDecorated(bool decorated)
{
    glfwSetWindowAttrib(m_id, GLFW_DECORATED, decorated);
}
bool Window::IsMinimized() const
{
    return glfwGetWindowAttrib(m_id, GLFW_ICONIFIED);
}
void Window::Minimize()
{
    glfwIconifyWindow(m_id);
}
bool Window::IsMaximized() const
{
    return glfwGetWindowAttrib(m_id, GLFW_MAXIMIZED);
}
void Window::Maximize()
{
    glfwMaximizeWindow(m_id);
}
void Window::Restore()
{
    glfwRestoreWindow(m_id);
}
bool Window::IsFocused() const
{
    return glfwGetWindowAttrib(m_id, GLFW_FOCUSED);
}
void Window::Focus()
{
    glfwFocusWindow(m_id);
}
bool Window::IsHovered() const
{
    return glfwGetWindowAttrib(m_id, GLFW_HOVERED);
}
bool Window::IsVisible() const
{
    return glfwGetWindowAttrib(m_id, GLFW_VISIBLE);
}
void Window::Show()
{
    glfwShowWindow(m_id);
}
void Window::Hide()
{
    glfwHideWindow(m_id);
}
void Window::RequestAttention()
{
    glfwRequestWindowAttention(m_id);
}
bool Window::IsFullScreen() const
{
    return glfwGetWindowMonitor(m_id);
}
opt<MonitorID> Window::GetFullScreenMonitor() const
{
    MonitorID monitorID = glfwGetWindowMonitor(m_id);
    if(monitorID != nullptr) {
        return monitorID;
    }
    return {};
}
void Window::SetWindowed(ivec2 pos, ivec2 size)
{
    glfwSetWindowMonitor(m_id, nullptr, pos.x, pos.y, size.x, size.y, 0);
}
void Window::SetFullScreen(const Monitor& monitor)
{
    MonitorVideoMode videoMode = monitor.GetVideoMode();
    glfwSetWindowMonitor(m_id, monitor.id(), 0, 0, videoMode.resolution.x, videoMode.resolution.y, videoMode.refreshRate);
}
bool Window::IsKeyPressed(Key key) const
{
    return static_cast<bool>(glfwGetKey(m_id, s_keyValues.ToValue(key)));
}
dvec2 Window::GetCursorPosScreen() const
{
    dvec2 pos;
    glfwGetCursorPos(m_id, &pos.x, &pos.y);
    return pos;
}
dvec2 Window::GetCursorPosEye() const
{
    return ScreenPosToEyePos(GetCursorPosScreen());
}
dvec2 Window::GetLastCursorDir() const
{
    return m_lastCursorDir;
}
dvec2 Window::ScreenPosToEyePos(dvec2 screenPos) const
{
    dvec2 windowSize = GetSize();
    return dvec2(2, -2) * screenPos / windowSize + dvec2(-1, 1);
}
bool Window::IsMouseButtonPressed(MouseButton button) const
{
    return static_cast<bool>(glfwGetMouseButton(m_id, static_cast<int>(button)));
}
void Window::SetCursor(CursorState state)
{
    glfwSetInputMode(m_id, GLFW_CURSOR, s_cursorStateValues.ToValue(state));
}
void Window::SetCursorPosScreen(dvec2 pos)
{
    glfwSetCursorPos(m_id, pos.x, pos.y);
}

EnumMap<Key> Window::s_keyValues = {
    GLFW_KEY_UNKNOWN,
    GLFW_KEY_SPACE,
    GLFW_KEY_APOSTROPHE,
    GLFW_KEY_COMMA,
    GLFW_KEY_MINUS,
    GLFW_KEY_PERIOD,
    GLFW_KEY_SLASH,
    GLFW_KEY_0,
    GLFW_KEY_1,
    GLFW_KEY_2,
    GLFW_KEY_3,
    GLFW_KEY_4,
    GLFW_KEY_5,
    GLFW_KEY_6,
    GLFW_KEY_7,
    GLFW_KEY_8,
    GLFW_KEY_9,
    GLFW_KEY_SEMICOLON,
    GLFW_KEY_EQUAL,
    GLFW_KEY_A,
    GLFW_KEY_B,
    GLFW_KEY_C,
    GLFW_KEY_D,
    GLFW_KEY_E,
    GLFW_KEY_F,
    GLFW_KEY_G,
    GLFW_KEY_H,
    GLFW_KEY_I,
    GLFW_KEY_J,
    GLFW_KEY_K,
    GLFW_KEY_L,
    GLFW_KEY_M,
    GLFW_KEY_N,
    GLFW_KEY_O,
    GLFW_KEY_P,
    GLFW_KEY_Q,
    GLFW_KEY_R,
    GLFW_KEY_S,
    GLFW_KEY_T,
    GLFW_KEY_U,
    GLFW_KEY_V,
    GLFW_KEY_W,
    GLFW_KEY_X,
    GLFW_KEY_Y,
    GLFW_KEY_Z,
    GLFW_KEY_LEFT_BRACKET,
    GLFW_KEY_BACKSLASH,
    GLFW_KEY_RIGHT_BRACKET,
    GLFW_KEY_GRAVE_ACCENT,
    GLFW_KEY_WORLD_1,
    GLFW_KEY_WORLD_2,
    GLFW_KEY_ESCAPE,
    GLFW_KEY_ENTER,
    GLFW_KEY_TAB,
    GLFW_KEY_BACKSPACE,
    GLFW_KEY_INSERT,
    GLFW_KEY_DELETE,
    GLFW_KEY_RIGHT,
    GLFW_KEY_LEFT,
    GLFW_KEY_DOWN,
    GLFW_KEY_UP,
    GLFW_KEY_PAGE_UP,
    GLFW_KEY_PAGE_DOWN,
    GLFW_KEY_HOME,
    GLFW_KEY_END,
    GLFW_KEY_CAPS_LOCK,
    GLFW_KEY_SCROLL_LOCK,
    GLFW_KEY_NUM_LOCK,
    GLFW_KEY_PRINT_SCREEN,
    GLFW_KEY_PAUSE,
    GLFW_KEY_F1,
    GLFW_KEY_F2,
    GLFW_KEY_F3,
    GLFW_KEY_F4,
    GLFW_KEY_F5,
    GLFW_KEY_F6,
    GLFW_KEY_F7,
    GLFW_KEY_F8,
    GLFW_KEY_F9,
    GLFW_KEY_F10,
    GLFW_KEY_F11,
    GLFW_KEY_F12,
    GLFW_KEY_F13,
    GLFW_KEY_F14,
    GLFW_KEY_F15,
    GLFW_KEY_F16,
    GLFW_KEY_F17,
    GLFW_KEY_F18,
    GLFW_KEY_F19,
    GLFW_KEY_F20,
    GLFW_KEY_F21,
    GLFW_KEY_F22,
    GLFW_KEY_F23,
    GLFW_KEY_F24,
    GLFW_KEY_F25,
    GLFW_KEY_KP_0,
    GLFW_KEY_KP_1,
    GLFW_KEY_KP_2,
    GLFW_KEY_KP_3,
    GLFW_KEY_KP_4,
    GLFW_KEY_KP_5,
    GLFW_KEY_KP_6,
    GLFW_KEY_KP_7,
    GLFW_KEY_KP_8,
    GLFW_KEY_KP_9,
    GLFW_KEY_KP_DECIMAL,
    GLFW_KEY_KP_DIVIDE,
    GLFW_KEY_KP_MULTIPLY,
    GLFW_KEY_KP_SUBTRACT,
    GLFW_KEY_KP_ADD,
    GLFW_KEY_KP_ENTER,
    GLFW_KEY_KP_EQUAL,
    GLFW_KEY_LEFT_SHIFT,
    GLFW_KEY_LEFT_CONTROL,
    GLFW_KEY_LEFT_ALT,
    GLFW_KEY_LEFT_SUPER,
    GLFW_KEY_RIGHT_SHIFT,
    GLFW_KEY_RIGHT_CONTROL,
    GLFW_KEY_RIGHT_ALT,
    GLFW_KEY_RIGHT_SUPER,
    GLFW_KEY_MENU
};
EnumMap<CursorState> Window::s_cursorStateValues = {
    GLFW_CURSOR_NORMAL,
    GLFW_CURSOR_HIDDEN,
    GLFW_CURSOR_DISABLED
};

}