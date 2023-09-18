
#ifndef MORPH_WINDOW_MANAGER_HPP
#define MORPH_WINDOW_MANAGER_HPP

#include <Core/Core.hpp>
#include <Core/EventSystem.hpp>

#include <Window/Hint.hpp>
#include <Window/Monitor.hpp>
#include <Window/Window.hpp>

namespace Morph {

enum class WindowManagerErrorType : int
{
    NO_ERROR,
    NOT_INITIALIZED,
    NO_CURRENT_CONTEXT,
    INVALID_ENUM,
    INVALID_VALUE,
    OUT_OF_MEMORY,
    API_UNAVAILABLE,
    VERSION_UNAVAILABLE,
    PLATFORM_ERROR,
    FORMAT_UNAVAILABLE,
    NO_WINDOW_CONTEXT
};

struct WindowManagerError
{
    WindowManagerErrorType type;
    string message;
};

class WindowManager :
	public IEventSystem<WindowManagerError>,
	public IEventSystem<MonitorEvent>,
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
private:
    static mutex s_instanceMutex;
    static EnumMap<WindowManagerErrorType> s_errorTypeValues;
    static EnumMap<MonitorState> s_monitorStateValues;
    static WindowManager* s_instance;
    unord_map<WindowID, Window> m_windows;
    unord_map<MonitorID, Monitor> m_monitors;
    UnordMapIteratorWrap<WindowID, Window> m_windowsIterators;
    UnordMapIteratorWrap<MonitorID, Monitor> m_monitorsIterators;
    EventSystem m_eventSystem;
    WindowHints m_defaultHints;
public:
    static WindowManager* Get();
    ~WindowManager();
private:
    WindowManager();
public:
    WindowManagerError GetError() const;

    opt<WindowID> CreateWindow(ivec2 size, const string& title);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const Monitor& monitor);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const Window& share);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const Monitor& monitor, const Window& share);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const WindowHints& hints);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const Monitor& monitor, const WindowHints& hints);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const Window& share, const WindowHints& hints);
    opt<WindowID> CreateWindow(ivec2 size, const string& title, const Monitor& monitor, const Window& share, const WindowHints& hints);
    bool DestroyWindow(WindowID id);
    opt<ref<Window>> GetWindow(WindowID id);
    opt<cref<Window>> GetWindow(WindowID id) const;
    inline UnordMapIteratorWrap<WindowID, Window>& GetWindows() { return m_windowsIterators; }
    inline const UnordMapIteratorWrap<WindowID, Window>& GetWindows() const { return m_windowsIterators; }
    inline usize GetWindowsCount() { return m_windows.size(); }

    MonitorID GetPrimaryMonitorID();
    opt<ref<Monitor>> GetPrimaryMonitor();
    opt<ref<Monitor>> GetMonitor(MonitorID id);
    opt<cref<Monitor>> GetMonitor(MonitorID id) const;
    inline UnordMapIteratorWrap<MonitorID, Monitor>& GetMonitors() { return m_monitorsIterators; }
    inline const UnordMapIteratorWrap<MonitorID, Monitor>& GetMonitors() const { return m_monitorsIterators; }
    inline usize GetMonitorsCount() { return m_monitors.size(); }

    void PollEvents();
    void WaitEvents();
    void WaitEventsWithTimeout(double timeoutSeconds);
    void WakeUpFromWaitEvents();

    string GetKeyName(Key key, int scancode);

    MORPH_EVENT_ACTIONS(WindowManagerError,m_eventSystem)
	MORPH_EVENT_ACTIONS(MonitorEvent,m_eventSystem)
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
    void AddWindow(WindowID id);
    void SetupMonitors();

    template<typename Event>
	inline static void ExecuteEvent(const Event& event) {
		s_instance->m_eventSystem.Execute(event);
	}
    template<typename Event>
	inline static void ExecuteEvent(WindowID id, const Event& event) {
		s_instance->GetWindow(id).value().get().m_eventSystem.Execute(event);
		s_instance->m_eventSystem.Execute(event);
	}

    static void ErrorCallback(int code, const char* message);
    static void MonitorCallback(MonitorID id, int event);
    static void CloseCallback(WindowID id);
	static void SizeCallback(WindowID id, int width, int height);
	static void FramebufferSizeCallback(WindowID id, int width, int height);
	static void ContentScaleCallback(WindowID id, float xscale, float yscale);
    static void MinimizeCallback(WindowID id, int minimized);
    static void MaximizeCallback(WindowID id, int maximized);
    static void FocusCallback(WindowID id, int focused);
    static void KeyCallback(WindowID id, int key, int scancode, int action, int mods);
    static void CharacterCallback(WindowID id, uint codepoint);
    static void CursorPositionCallback(WindowID id, double xpos, double ypos);
    static void CursorEnterCallback(WindowID id, int entered);
    static void MouseButtonCallback(WindowID id, int button, int action, int mods);
    static void ScrollCallback(WindowID id, double xoffset, double yoffset);
    static void DropCallback(WindowID id, int count, const char** paths);
};

}

#endif // MORPH_WINDOW_MANAGER_HPP