#include "OpenGL.hpp"

using namespace magic_enum::ostream_operators;

namespace Morph {

EnumMap<OpenGLError> OpenGL::s_openGLErrorValues = {
    GL_NO_ERROR,
    GL_INVALID_ENUM,
    GL_INVALID_VALUE,
    GL_INVALID_OPERATION,
    GL_STACK_OVERFLOW,
    GL_STACK_UNDERFLOW,
    GL_OUT_OF_MEMORY,
    GL_INVALID_FRAMEBUFFER_OPERATION,
    GL_CONTEXT_LOST
};

OpenGLError OpenGL::GetError()
{
    return s_openGLErrorValues.ToEnum(glGetError());
}

void OpenGL::ClearError()
{
    while(OpenGL::GetError() != OpenGLError::NONE);
}

bool OpenGL::Error(const char* file, int line)
{
    OpenGLError err;
    bool res = false;
    if((err = GetError()) != OpenGLError::NONE) {
        res = true;
        critical("OpenGL error {}, line {}, file {}", err, line, file);
    }
    return res;
}

}