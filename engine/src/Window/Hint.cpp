#include "Hint.hpp"

#include <Graphics/OpenGL.hpp>

namespace Morph
{

void WindowBoolHint::Set() const
{
    glfwWindowHint(static_cast<int>(m_setting), m_enabled);
}

WindowIntHint::WindowIntHint(WindowIntSetting setting)
    : m_setting(setting), m_value(GLFW_DONT_CARE) {}

void WindowIntHint::Set() const
{
    glfwWindowHint(static_cast<int>(m_setting), m_value);
}

void WindowContextVersionHint::Set() const
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, m_major);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, m_minor);
}
void WindowClientAPIHint::Set() const
{
    glfwWindowHint(GLFW_CLIENT_API, static_cast<int>(m_clientAPI));
}

void WindowHints::Add(const WindowHintVar& hint)
{
    m_hints.push_back(hint);
}

void WindowHints::Set() const
{
    for(const WindowHintVar& hintVar : m_hints) {
        match(hintVar,
            [](const WindowHint& hint) {
                hint.Set();
            }
        );
    }
}

}