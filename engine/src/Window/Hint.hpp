#ifndef MORPH_WINDOW_HINT
#define MORPH_WINDOW_HINT

#include <Core/Core.hpp>

namespace Morph
{

class WindowManager;
class WindowHints;

class WindowHint
{
    friend class WindowHints;
protected:
    virtual void Set() const = 0;
};

enum class WindowBoolSetting : int
{
    RESIZABLE = 0x00020003,
    VISIBLE = 0x00020004,
    DECORATED = 0x00020005,
    FOCUSED = 0x00020001,
    ICONIFIED = 0x00020002,
    FLOATING = 0x00020007,
    MAXIMIZED = 0x00020008,
    CENTER_CURSOR = 0x00020009,
    TRANSPARENT_FRAMEBUFFER = 0x0002000A,
    FOCUS_ON_SHOW = 0x0002000C,
    SCALE_TO_MONITOR = 0x0002200C,
    STEREO = 0x0002100C,
    SRGB_CAPABLE = 0x0002100E,
    DOUBLEBUFFER = 0x00021010,
    OPENGL_FORWARD_COMPAT = 0x00022006,
    OPENGL_DEBUG_CONTEXT = 0x00022007
};

enum class WindowIntSetting : int
{
    RED_BITS = 0x00021001,
    GREEN_BITS = 0x00021002,
    BLUE_BITS = 0x00021003,
    ALPHA_BITS = 0x00021004,
    DEPTH_BITS = 0x00021005,
    STENCIL_BITS = 0x00021006,
    ACCUM_RED_BITS = 0x00021007,
    ACCUM_GREEN_BITS = 0x00021008,
    ACCUM_BLUE_BITS = 0x00021009,
    ACCUM_ALPHA_BITS = 0x0002100A,
    AUX_BUFFERS = 0x0002100B,
    STEREO = 0x0002100C,
    SAMPLES = 0x0002100D
};

enum class WindowClientAPI : int
{
    NO_API = 0,
    OPENGL = 0x00030001,
    OPENGL_ES = 0x00030002
};

class WindowBoolHint : public WindowHint
{
private:
    WindowBoolSetting m_setting;
    bool m_enabled;
public:
    WindowBoolHint(WindowBoolSetting setting, bool enabled)
        : m_setting(setting), m_enabled(enabled) {}
protected:
    void Set() const override;
};

class WindowIntHint : public WindowHint
{
private:
    WindowIntSetting m_setting;
    int m_value;
public:
    WindowIntHint(WindowIntSetting setting);
    WindowIntHint(WindowIntSetting setting, int value)
        : m_setting(setting), m_value(value) {}
protected:
    void Set() const override;
};

class WindowContextVersionHint : public WindowHint
{
private:
    int m_major;
    int m_minor;
public:
    WindowContextVersionHint(int major, int minor) : m_major(major), m_minor(minor) {}
protected:
    void Set() const override;
};

class WindowClientAPIHint : public WindowHint
{
private:
    WindowClientAPI m_clientAPI;
public:
    WindowClientAPIHint(WindowClientAPI clientAPI) : m_clientAPI(clientAPI) {}
protected:
    void Set() const override;
};

using WindowHintVar = variant<
    WindowBoolHint,
    WindowIntHint,
    WindowContextVersionHint,
    WindowClientAPIHint
>;

class WindowHints
{
    friend class WindowManager;
private:
    vector<WindowHintVar> m_hints;
public:
    WindowHints(const init_list<WindowHintVar>& hints) : m_hints(hints) {}
    WindowHints() : m_hints() {}
    void Add(const WindowHintVar& hint);
private:
    void Set() const;
};

}

#endif // MORPH_WINDOW_HINT