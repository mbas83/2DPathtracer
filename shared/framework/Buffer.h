#pragma once
#include <glad/glad.h>
#include <vector>
#include <stdexcept>
#include "dassert.h"

namespace gpupro
{
    // The type of a buffer determines its MAIN purpose.
    // The buffer will be bound to that slot for several operations.
    // However, you can always bind the same buffer for DIFFERENT
    // purposes.
    enum class BufferType
    {
        ARRAY = GL_ARRAY_BUFFER,
        // vertex buffer
        ELEMENT_ARRAY = GL_ELEMENT_ARRAY_BUFFER,
        // index buffer
        SHADER_STORAGE = GL_SHADER_STORAGE_BUFFER,
        UNIFORM = GL_UNIFORM_BUFFER,
        ATOMIC_COUNTER = GL_ATOMIC_COUNTER_BUFFER,
        TRANSFORM_FEEDBACK = GL_TRANSFORM_FEEDBACK_BUFFER,
        DISPATCH_INDIRECT = GL_DISPATCH_INDIRECT_BUFFER,
        DRAW_INDIRECT = GL_DRAW_INDIRECT_BUFFER,
        QUERY = GL_QUERY_BUFFER
    };

    // A buffer is a pure memory block on GPU side.
    template <class TElement>
    class Buffer
    {
    public:

        /// creates a buffer 
        /// \param type specify main purpose of this buffer (usage: Buffer::Type::ARRAY)
        /// \param numElements number of elements in this buffer (byte size is sizeof(TElement) * numElements)
        /// \param data initialization data (may be nullptr)
        Buffer(BufferType type, GLuint numElements, const void* data = nullptr);

        // creates a buffer initialized with an std::vector
        /// \param type specify main purpose of this buffer (usage: BufferType::ARRAY)
        /// \param data initial data
        Buffer(BufferType type, const std::vector<TElement>& data) :
            Buffer(type, GLuint(data.size()), data.data())
        {
        }

        // creates a buffer initialized with a single member
        /// \param type specify main purpose of this buffer (usage: BufferType::ARRAY)
        /// \param data initial data
        Buffer(BufferType type, const TElement& data);

        // default constructor for empty buffer (m_id = 0)
        Buffer() = default;
        ~Buffer();
        Buffer(const Buffer&) = delete;
        Buffer& operator=(const Buffer&) = delete;
        Buffer(Buffer&& o) noexcept;
        Buffer& operator=(Buffer&& o) noexcept;

        /// Bind as vertex buffer
        /// \param bindingIndex binding index on the cpu side
        void bindAsVertexBuffer(GLuint bindingIndex) const;

        /// Bind to GL_ELEMENT_ARRAY_BUFFER (index buffer)
        void bindAsIndexBuffer() const;

        /// bind as uniform buffer
        /// \param bindingIndex binding index of the uniform buffer. Marked in shader as "layout(binding = X) uniform myBuffer"
        void bindAsUniformBuffer(GLuint bindingIndex) const;

        /// bind as shader storage buffer
        /// \param bindingIndex binding index of the uniform buffer. Marked in shader as "layout(binding = X) uniform myBuffer"
        void bindAsShaderStorageBuffer(GLuint bindingIndex);

        /// refreshes the contents of the entire buffer
        /// \param data data that replaces the buffer contents (must be the same size as the buffer)
        void subDataUpdate(const std::vector<TElement>& data);

        /// refreshes the contents of the entire buffer
        /// \param data data that replaces the buffer contents (must be the same size as the buffer)
        void subDataUpdate(const TElement& data);

        GLuint getNumElements() const { return m_size / sizeof(TElement); }
        GLuint getID() { return m_id; }

    private:
        /// refreshes the contents of the entire buffer
        void subDataUpdate(const void* data);

        void swap(Buffer<TElement>& o) noexcept
        {
            std::swap(m_id, o.m_id);
            std::swap(m_type, o.m_type);
            std::swap(m_size, o.m_size);
            std::swap(m_flags, o.m_flags);
        }

        GLuint m_id = 0;
        BufferType m_type;
        GLsizei m_size;
        GLenum m_flags;
    };

    // Implementations:

    template <class TElement>
    Buffer<TElement>::Buffer(BufferType type, GLuint numElements, const void* data):
        m_type(type),
        m_size(sizeof(TElement) * numElements),
        m_flags(0)
    {
        dassert(numElements > 0); // there should be data in the buffer...

        // uniform buffers may be updated from the cpu afterwards
        // this does usually not make sense for other types of buffers
        if (type == BufferType::UNIFORM) m_flags |= GL_DYNAMIC_STORAGE_BIT;

        // Generated one buffer
        glGenBuffers(1, &m_id);
        // Allocate space
        glBindBuffer(static_cast<GLenum>(m_type), m_id);
        glBufferStorage(static_cast<GLenum>(m_type), m_size, data, m_flags);
    }

    template <class TElement>
    Buffer<TElement>::Buffer(BufferType type, const TElement& data):
        Buffer(type, GLuint(1), &data)
    {
    }

    template <class TElement>
    Buffer<TElement>::~Buffer()
    {
        if (m_id)
        {
            glDeleteBuffers(1, &m_id);
            m_id = 0;
        }
    }

    template <class TElement>
    Buffer<TElement>::Buffer(Buffer&& o) noexcept
    {
        swap(o);
    }

    template <class TElement>
    Buffer<TElement>& Buffer<TElement>::operator=(Buffer&& o) noexcept
    {
        swap(o);
        return *this;
    }

    template <class TElement>
    void Buffer<TElement>::bindAsVertexBuffer(GLuint bindingIndex) const
    {
        dassert(m_id);
        glBindVertexBuffer(bindingIndex, m_id, 0, sizeof(TElement));
    }

    template <class TElement>
    void Buffer<TElement>::bindAsIndexBuffer() const
    {
        dassert(m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    template <class TElement>
    void Buffer<TElement>::bindAsUniformBuffer(GLuint bindingIndex) const
    {
        dassert(m_id);
        // bind whole buffer (use glBindBufferRange for subset)
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingIndex, m_id);
    }

    template <class TElement>
    void Buffer<TElement>::bindAsShaderStorageBuffer(GLuint bindingIndex)
    {
        dassert(m_id);
        // bind whole buffer (use glBindBufferRange for subset)
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bindingIndex, m_id);
    }

    template <class TElement>
    void Buffer<TElement>::subDataUpdate(const std::vector<TElement>& data)
    {
        if (sizeof(TElement) * data.size() != m_size)
            throw std::runtime_error("Buffer::subDataUpdate data size is not the same as buffer size");
        subDataUpdate(data.data());
    }

    template <class TElement>
    void Buffer<TElement>::subDataUpdate(const TElement& data)
    {
        if (sizeof(TElement) != m_size)
            throw std::runtime_error("Buffer::subDataUpdate data size is not the same as buffer size");
        subDataUpdate(&data);
    }

    template <class TElement>
    void Buffer<TElement>::subDataUpdate(const void* data)
    {
        if (!(m_flags & GL_DYNAMIC_STORAGE_BIT))
            throw std::runtime_error("Buffer::subDataUpdate requires a UNIFORM buffer");

        dassert(m_id);
        // Bind to arbitrary buffer point to call the storage command
        glBindBuffer(static_cast<GLenum>(m_type), m_id);
        glBufferSubData(static_cast<GLenum>(m_type), 0, m_size, data);
    }
} // namespace gpupro
