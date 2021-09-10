#pragma once
#include <glad/glad.h>

namespace gpupro
{
    template <typename T>
    inline T getOpenglConstant(GLenum name) = delete;

    template <>
    inline GLint getOpenglConstant<GLint>(GLenum name)
    {
        GLint res = 0;
        glGetIntegerv(name, &res);
        return res;
    }
}
