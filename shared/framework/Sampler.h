#pragma once
#include <glad/glad.h>
#include <vector>
#include <stdexcept>

namespace gpupro
{
    enum class Wrap
    {
        REPEAT = GL_REPEAT,
        CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
        // CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER, // not implemented
        MIRRORED_REPEAT = GL_MIRRORED_REPEAT
    };

    enum class MinifyFilter
    {
        // Returns the value of the texture element that is nearest (in Manhattan distance) to the specified texture coordinates
        NEAREST = GL_NEAREST,
        // Returns the weighted average of the four texture elements that are closest to the specified texture coordinates
        LINEAR = GL_LINEAR,
        // Chooses the mipmap that most closely matches the size of the pixel being textured and uses the GL_NEAREST criterion
        NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST,
        // Chooses the mipmap that most closely matches the size of the pixel being textured and uses the GL_LINEAR criterion inside the mipmap
        LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,
        // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_NEAREST criterion inside the mipmap (linearly blends those values afterwards)
        NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,
        // Chooses the two mipmaps that most closely match the size of the pixel being textured and uses the GL_LINEAR criterion (linearly blends between two mipmaps)
        LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR
    };

    enum class MagnifyFilter
    {
        // Returns the value of the texture element that is nearest (in Manhattan distance) to the specified texture coordinates
        NEAREST = GL_NEAREST,
        // Returns the weighted average of the four texture elements that are closest to the specified texture coordinates
        LINEAR = GL_LINEAR,
    };

    class Sampler
    {
    public:
        /// creates a texture sampler
        /// \param wrapping texture wrapping for texture coordinates outside of [0, 1]
        /// \param magnify filter used when close to the texture
        /// \param minify filter used when the texture is far away (multiple texture pixels project to a single screen pixel)
        /// \param useAnisotropyFilter enable use of anisotropic filter (improved quality when looking at grazing angles)
        Sampler(Wrap wrapping, MagnifyFilter magnify, MinifyFilter minify, bool useAnisotropyFilter);

        ~Sampler();

        Sampler(const Sampler&) = delete;
        Sampler& operator=(const Sampler&) = delete;
        Sampler(Sampler&& o) noexcept;
        Sampler& operator=(Sampler&& o) noexcept;

        /// bind sampler
        /// \param bindingIndex shader binding slot ()
        void bind(GLuint bindingIndex) const;

        GLuint getID() const { return m_id; }
    private:
        void swap(Sampler& o) noexcept
        {
            std::swap(m_id, o.m_id);
        }

        GLuint m_id = 0;
    };
}
