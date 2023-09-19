
#ifndef MORPH_INPUT_HPP
#define MORPH_INPUT_HPP

#include <Core/Core.hpp>

struct GLFWwindow;

namespace Morph {

using WindowID = GLFWwindow*;

enum class Key : int
{
    UNKNOWN,
    SPACE,
    APOSTROPHE, /* ' */    
    COMMA, /* , */
    MINUS, /* - */
    PERIOD, /* . */
    SLASH, /* / */
    ALPHA_0,
    ALPHA_1,
    ALPHA_2,
    ALPHA_3,
    ALPHA_4,
    ALPHA_5,
    ALPHA_6,
    ALPHA_7,
    ALPHA_8,
    ALPHA_9,
    SEMICOLON, /* ; */
    EQUAL, /* = */
    A,
    B,
    C,
    D,
    E,
    F,
    G,
    H,
    I,
    J,
    K,
    L,
    M,
    N,
    O,
    P,
    Q,
    R,
    S,
    T,
    U,
    V,
    W,
    X,
    Y,
    Z,
    LEFT_BRACKET, /* [ */
    BACKSLASH, /* \ */
    RIGHT_BRACKET, /* ] */
    GRAVE_ACCENT, /* ` */
    WORLD_1, /* non-US #1 */
    WORLD_2, /* non-US #2 */
    ESCAPE,
    ENTER,
    TAB,
    BACKSPACE,
    INSERT,
    DELETE,
    RIGHT,
    LEFT,
    DOWN,
    UP,
    PAGE_UP,
    PAGE_DOWN,
    HOME,
    END,
    CAPS_LOCK,
    SCROLL_LOCK,
    NUM_LOCK,
    PRINT_SCREEN,
    PAUSE,
    F1,
    F2,
    F3,
    F4,
    F5,
    F6,
    F7,
    F8,
    F9,
    F10,
    F11,
    F12,
    F13,
    F14,
    F15,
    F16,
    F17,
    F18,
    F19,
    F20,
    F21,
    F22,
    F23,
    F24,
    F25,
    KEYPAD_0,
    KEYPAD_1,
    KEYPAD_2,
    KEYPAD_3,
    KEYPAD_4,
    KEYPAD_5,
    KEYPAD_6,
    KEYPAD_7,
    KEYPAD_8,
    KEYPAD_9,
    KEYPAD_DECIMAL,
    KEYPAD_DIVIDE,
    KEYPAD_MULTIPLY,
    KEYPAD_SUBTRACT,
    KEYPAD_ADD,
    KEYPAD_ENTER,
    KEYPAD_EQUAL,
    LEFT_SHIFT,
    LEFT_CONTROL,
    LEFT_ALT,
    LEFT_SUPER,
    RIGHT_SHIFT,
    RIGHT_CONTROL,
    RIGHT_ALT,
    RIGHT_SUPER,
    MENU
};

enum class ModKey : int
{
    SHIFT = 0x0001,
    CONTROL = 0x0002,
    ALT = 0x0004,
    SUPER = 0x0008,
    CAPS_LOCK = 0x0010,
    NUM_LOCK = 0x0020
};

enum class KeyAction : int
{
    RELEASE,
    PRESS,
    REPEAT
};

enum class MouseButton : int
{
    LEFT_1,
    RIGHT_2,
    MIDDLE_3,
    OTHER_4,
    OTHER_5,
    OTHER_6,
    OTHER_7,
    OTHER_8
};

enum class CursorState : int
{
    NORMAL,
    HIDDEN,
    DISABLED
};

struct KeyEvent
{
    WindowID window;
    Key key;
    int scancode;
    KeyAction action;
    int mods;

    inline bool ModKeyPressed(ModKey modKey) const {
        return mods & static_cast<int>(modKey);
    }
};
struct CharEvent
{
    WindowID window;
    uint codepoint; // UTF8
};
struct CursorPosEvent
{
    WindowID window;
    dvec2 screenPos;
    dvec2 eyePos;
    dvec2 eyeDir;
};
struct CursorEnterEvent
{
    WindowID window;
    bool entered;
};
struct MouseButtonEvent
{
    WindowID window;
    MouseButton button;
    bool pressed;
    int mods;

    inline bool ModKeyPressed(ModKey modKey) {
        return mods & static_cast<int>(modKey);
    }
};
struct ScrollEvent
{
    WindowID window;
    dvec2 offset; // standard is on Y axis
};

}

#endif // MORPH_INPUT_HPP