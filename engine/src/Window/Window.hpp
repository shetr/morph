
#ifndef MORPH_WINDOW_HPP
#define MORPH_WINDOW_HPP

#include <Core/Core.hpp>
#include <Core/EventSystem.hpp>

#include "Monitor.hpp"
#include "Input.hpp"

struct GLFWwindow;

namespace Morph {

class WindowManager;

using WindowID = GLFWwindow*;

struct WindowFrameSize
{
    int left;
    int top;
    int right;
    int bottom;
};

struct WindowCloseEvent
{
    WindowID window;
};

struct WindowSizeEvent
{
    WindowID window;
    uvec2 size;
};

struct WindowFramebufferSizeEvent
{
    WindowID window;
    uvec2 size;
};

struct WindowContentScaleEvent
{
    WindowID window;
    vec2 scale;
};

struct WindowSizeLimits
{
    opt<int> minWidth;
    opt<int> minHeight;
    opt<int> maxWidth;
    opt<int> maxHeight;
};

struct WindowAspectRatio
{
    int numer;
    int denom;
};

struct WindowMinimizeEvent
{
    WindowID window;
    bool minimized;
};
struct WindowMaximizeEvent
{
    WindowID window;
    bool maximized;
};
struct WindowRestoreEvent
{
    WindowID window;
};
struct WindowFocusEvent
{
    WindowID window;
    bool focused;
};
struct WindowDropEvent
{
    WindowID window;
    vector<string> paths;
};

class Window : 
	public IEventSystem<WindowCloseEvent>,
	public IEventSystem<WindowSizeEvent>,
	public IEventSystem<WindowFramebufferSizeEvent>,
	public IEventSystem<WindowContentScaleEvent>,
	public IEventSystem<WindowMinimizeEvent>,
	public IEventSystem<WindowMaximizeEvent>,
	public IEventSystem<WindowRestoreEvent>,
	public IEventSystem<WindowFocusEvent>,
	public IEventSystem<KeyEvent>,
	public IEventSystem<CharEvent>,
	public IEventSystem<CursorPosEvent>,
	public IEventSystem<CursorEnterEvent>,
	public IEventSystem<MouseButtonEvent>,
	public IEventSystem<ScrollEvent>,
	public IEventSystem<WindowDropEvent>
{
    friend class WindowManager;
private:
    WindowID m_id;
    EventSystem m_eventSystem;
    WindowSizeLimits m_sizeLimits;
    opt<WindowAspectRatio> m_fixedAspectRatio;
    dvec2 m_lastCursorPos = dvec2(0, 0);
    dvec2 m_lastCursorDir = dvec2(0, 0);
    bool m_lastCursorPosValid = false;
private:
    Window(WindowID id) : m_id(id) {}
public:
    inline WindowID id() const { return m_id; }
    
    uvec2 GetSize() const;
	void SetSize(uvec2 size);
	WindowFrameSize GetFrameSize() const;
    vec2 GetContentScale() const;
    bool ShouldClose() const;
    void SetShouldClose(bool shouldClose);
    WindowSizeLimits GetSizeLimits() const;
    void SetSizeLimits(WindowSizeLimits limits);
    opt<WindowAspectRatio> GetFixedAspectRatio() const;
    void SetFixedAspectRatio(WindowAspectRatio aspectRatio);
    ivec2 GetPos() const;
    void SetPos(ivec2 pos);
    void SetTitle(const string& title);
    bool IsResizableByUser() const;
    void SetResizableByUser(bool resizable);
    bool IsFloating() const;
    void SetFloating(bool floating);
    bool IsDecorated() const;
    void SetDecorated(bool decorated);
    bool IsMinimized() const;
    void Minimize();
    bool IsMaximized() const;
    void Maximize();
    void Restore();
    bool IsFocused() const;
    void Focus();
    bool IsHovered() const;
    bool IsVisible() const;
    void Show();
    void Hide();
    void RequestAttention();
    bool IsFullScreen() const;
    opt<MonitorID> GetFullScreenMonitor() const;
    void SetWindowed(ivec2 pos, ivec2 size);
    void SetFullScreen(const Monitor& monitor);
    bool IsKeyPressed(Key key) const;
    dvec2 GetCursorPosScreen() const;
    dvec2 GetCursorPosEye() const;
    dvec2 GetLastCursorDir() const;
    dvec2 ScreenPosToEyePos(dvec2 screenPos) const;
    bool IsMouseButtonPressed(MouseButton button) const;
    void SetCursor(CursorState state);
    void SetCursorPosScreen(dvec2 pos);

	MORPH_EVENT_ACTIONS(WindowCloseEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowSizeEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowFramebufferSizeEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowContentScaleEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowMinimizeEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowMaximizeEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowRestoreEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowFocusEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(KeyEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(CharEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(CursorPosEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(CursorEnterEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(MouseButtonEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(ScrollEvent,m_eventSystem)
	MORPH_EVENT_ACTIONS(WindowDropEvent,m_eventSystem)
private:
    static EnumMap<Key> s_keyValues;
    static EnumMap<CursorState> s_cursorStateValues;
};

}

#endif // MORPH_WINDOW_HPP