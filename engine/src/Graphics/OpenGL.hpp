
#ifndef MORPH_OPENGL_HPP
#define MORPH_OPENGL_HPP

#include <Core/Core.hpp>
#include <Core/Debug.hpp>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#ifndef MORPH_DEBUG
#define GL(action) gl##action
#else // ifdef MORPH_DEBUG
#define GL(action) gl##action;\
    if(Morph::OpenGL::Error(__FILE__, __LINE__)) MORPH_DEBUG_BREAK;
#endif // MORPH_DEBUG

namespace Morph {

enum class OpenGLError
{
    NONE,
    INVALID_ENUM,
    INVALID_VALUE,
    INVALID_OPERATION,
    STACK_OVERFLOW,
    STACK_UNDERFLOW,
    OUT_OF_MEMORY,
    INVALID_FRAMEBUFFER_OPERATION,
    CONTEXT_LOST
};

class OpenGL
{
private:
    static EnumMap<OpenGLError> s_openGLErrorValues;
public:
    static OpenGLError GetError();
    static void ClearError();
    static bool Error(const char* file, int line);
};

}

#endif // MORPH_OPENGL_HPP