#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace gpupro
{
    enum class InternalFormat
    {
        // One channel
        R8 = GL_R8,
        R8S = GL_R8_SNORM,
        R8I = GL_R8I,
        R8UI = GL_R8UI,
        R16 = GL_R16,
        R16S = GL_R16_SNORM,
        R16I = GL_R16I,
        R16UI = GL_R16UI,
        R16F = GL_R16F,
        R32I = GL_R32I,
        R32UI = GL_R32UI,
        R32F = GL_R32F,

        // Two channels
        RG8 = GL_RG8,
        RG8S = GL_RG8_SNORM,
        RG8I = GL_RG8I,
        RG8UI = GL_RG8UI,
        RG16 = GL_RG16,
        RG16S = GL_RG16_SNORM,
        RG16I = GL_RG16I,
        RG16UI = GL_RG16UI,
        RG16F = GL_RG16F,
        RG32I = GL_RG32I,
        RG32UI = GL_RG32UI,
        RG32F = GL_RG32F,

        // Three channels
        R3_G3_B2 = GL_R3_G3_B2,
        RGB4 = GL_RGB4,
        RGB5 = GL_RGB5,
        RGB8 = GL_RGB8,
        RGB8S = GL_RGB8_SNORM,
        RGB8I = GL_RGB8I,
        RGB8UI = GL_RGB8UI,
        SRGB8 = GL_SRGB8,
        RGB10 = GL_RGB10,
        RGB12 = GL_RGB12,
        RGB16 = GL_RGB16,
        RGB16S = GL_RGB16_SNORM,
        RGB16I = GL_RGB16I,
        RGB16UI = GL_RGB16UI,
        RGB16F = GL_RGB16F,
        RGB32I = GL_RGB32I,
        RGB32UI = GL_RGB32UI,
        RGB32F = GL_RGB32F,
        R11F_G11F_B10F = GL_R11F_G11F_B10F,
        RGB9_E5 = GL_RGB9_E5,

        // Four channels
        RGBA2 = GL_RGBA2,
        RGBA4 = GL_RGBA4,
        RGB5_A1 = GL_RGB5_A1,
        RGBA8 = GL_RGBA8,
        RGBA8S = GL_RGBA8_SNORM,
        RGBA8I = GL_RGBA8I,
        RGBA8UI = GL_RGBA8UI,
        SRGB8_ALPHA8 = GL_SRGB8_ALPHA8,
        RGB10_A2 = GL_RGB10_A2,
        RGB10_A2UI = GL_RGB10_A2UI,
        RGBA12 = GL_RGBA12,
        RGBA16 = GL_RGBA16,
        RGBA16S = GL_RGBA16_SNORM,
        RGBA16I = GL_RGBA16I,
        RGBA16UI = GL_RGBA16UI,
        RGBA16F = GL_RGBA16F,
        RGBA32I = GL_RGBA32I,
        RGBA32UI = GL_RGBA32UI,
        RGBA32F = GL_RGBA32F,

        // Depth stencil formats
        DEPTH_COMPONENT32F = GL_DEPTH_COMPONENT32F,
        DEPTH_COMPONENT24 = GL_DEPTH_COMPONENT24,
        DEPTH_COMPONENT16 = GL_DEPTH_COMPONENT16,
        DEPTH32F_STENCIL8 = GL_DEPTH32F_STENCIL8,
        DEPTH24_STENCIL8 = GL_DEPTH24_STENCIL8,
        STENCIL_INDEX8 = GL_STENCIL_INDEX8,
    };

    bool isDepthFormat(InternalFormat _format);
    bool isStencilFormat(InternalFormat _format);
    bool isSignedFormat(InternalFormat _format);

    // Possible data formats for setData functions.
    enum class SetDataFormat
    {
        R = GL_RED,
        RG = GL_RG,
        RGB = GL_RGB,
        BGR = GL_BGR,
        RGBA = GL_RGBA,
        BGRA = GL_BGRA,
        DEPTH_COMPONENT = GL_DEPTH_COMPONENT,
        STENCIL_INDEX = GL_STENCIL_INDEX,
        R_INTEGER = GL_RED_INTEGER,
        RG_INTEGER = GL_RG_INTEGER,
        RGB_INTEGER = GL_RGB_INTEGER,
        RGBA_INTEGER = GL_RGBA_INTEGER,
        BGR_INTEGER = GL_BGR_INTEGER,
        BGRA_INTEGER = GL_BGRA_INTEGER
    };

    // Possible (common) data formats for setData functions.
    enum class SetDataType
    {
        UINT8 = GL_UNSIGNED_BYTE,
        INT8 = GL_BYTE,
        UINT16 = GL_UNSIGNED_SHORT,
        INT16 = GL_SHORT,
        UINT32 = GL_UNSIGNED_INT,
        INT32 = GL_INT,
        FLOAT = GL_FLOAT
    };

    // helper to convert from data types (float, int) to gl enums (GL_FLOAT, GL_INT)
    namespace gltype
    {
        // if Type is equal to ErrorType it could not be translated
        constexpr GLenum ErrorType = 0;
        template <typename T>
        constexpr GLenum Type = ErrorType;
        template <>
        constexpr GLenum Type<GLfloat> = GL_FLOAT;
        template <>
        constexpr GLenum Type<GLint> = GL_INT;
        template <>
        constexpr GLenum Type<GLuint> = GL_UNSIGNED_INT;
        template <>
        constexpr GLenum Type<GLshort> = GL_SHORT;
        template <>
        constexpr GLenum Type<GLushort> = GL_UNSIGNED_SHORT;
        template <>
        constexpr GLenum Type<GLbyte> = GL_BYTE;
        template <>
        constexpr GLenum Type<GLubyte> = GL_UNSIGNED_BYTE;
        template <typename T>
        constexpr GLenum Type<glm::tvec1<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tvec2<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tvec3<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tvec4<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat2x2<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat2x3<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat2x4<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat3x2<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat3x3<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat3x4<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat4x2<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat4x3<T>> = Type<T>;
        template <typename T>
        constexpr GLenum Type<glm::tmat4x4<T>> = Type<T>;

        template <typename T>
        constexpr GLsizei NumComponents = 0;
        template <>
        constexpr GLsizei NumComponents<GLfloat> = 1;
        template <>
        constexpr GLsizei NumComponents<GLint> = 1;
        template <>
        constexpr GLsizei NumComponents<GLuint> = 1;
        template <>
        constexpr GLsizei NumComponents<GLshort> = 1;
        template <>
        constexpr GLsizei NumComponents<GLushort> = 1;
        template <>
        constexpr GLsizei NumComponents<GLbyte> = 1;
        template <>
        constexpr GLsizei NumComponents<GLubyte> = 1;
        template <typename T>
        constexpr GLsizei NumComponents<glm::tvec1<T>> = 1;
        template <typename T>
        constexpr GLsizei NumComponents<glm::tvec2<T>> = 2;
        template <typename T>
        constexpr GLsizei NumComponents<glm::tvec3<T>> = 3;
        template <typename T>
        constexpr GLsizei NumComponents<glm::tvec4<T>> = 4;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat2x2<T>> = 2 * 2;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat2x3<T>> = 2 * 3;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat2x4<T>> = 2 * 4;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat3x2<T>> = 3 * 2;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat3x3<T>> = 3 * 3;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat3x4<T>> = 3 * 4;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat4x2<T>> = 4 * 2;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat4x3<T>> = 4 * 3;
        template <typename T>
        constexpr GLenum NumComponents<glm::tmat4x4<T>> = 4 * 4;
    }
} // namespace gpupro
