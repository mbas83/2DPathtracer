#pragma once
#include <glad/glad.h>
#include <vector>
#include <stdexcept>

namespace gpupro
{
    enum class VertexType
    {
        BYTE = GL_BYTE,
        UNSIGNED_BYTE = GL_UNSIGNED_BYTE,
        SHORT = GL_SHORT,
        UNSIGNED_SHORT = GL_UNSIGNED_SHORT,
        INT = GL_INT,
        UNSIGNED_INT = GL_UNSIGNED_INT,

        FIXED = GL_FIXED,

        FLOAT = GL_FLOAT,
        HALF_FLOAT = GL_HALF_FLOAT,
        DOUBLE = GL_DOUBLE,
    };

    class VertexArray
    {
    public:
        VertexArray();

        ~VertexArray();
        VertexArray(const VertexArray&) = delete;
        VertexArray& operator=(const VertexArray&) = delete;
        VertexArray(VertexArray&& o) noexcept;
        VertexArray& operator=(VertexArray&& o) noexcept;

        /// adds an attribute binding to the vertex array
        /// \param bindingIndex cpu side binding index (from Buffer::bindAsVertexBuffer(X))
        /// \param attributeIndex shader side location (layout(location = X) out vec3 position;)
        /// \param type underlying vertex type (VertexType::FLOAT, VertexType::INT, etc.)
        /// \param numComponents number of values per vertex (i.e. 3 for vec3, 2 for vec2, 1 for float, int...)
        /// \param offset byte offset to where the first vertex starts
        VertexArray& addBinding(GLuint bindingIndex, GLuint attributeIndex, VertexType type, GLuint numComponents,
                                GLuint offset = 0);

        /// sets this vertex array as active vertex array
        void bind() const;
    private:
        static bool isIntegerType(VertexType t);

        void swap(VertexArray& o) noexcept
        {
            std::swap(m_id, o.m_id);
        }

        GLuint m_id = 0;
    };
}
