#pragma once
#include <glad/glad.h>
#include "Texture.h"
#include <vector>

namespace gpupro
{
    // A frame buffer is a collection of render-targets.
    class Framebuffer
    {
    public:
        Framebuffer();
        ~Framebuffer();
        Framebuffer(const Framebuffer&) = delete;
        Framebuffer& operator=(const Framebuffer&) = delete;
        Framebuffer(Framebuffer&& o) noexcept;
        Framebuffer& operator=(Framebuffer&& o) noexcept;

        /// attach a texture as depth target
        /// \param target texture with an internal depth format
        /// \param mipLevel mipmap level to be attached (default is 0)
        void attachDepthTexture(Texture& target, GLuint mipLevel = 0);

        /// attach a single layer of a texture as depth target
        /// \param target texture with an internal depth format
        /// \param mipLevel mipmap level to be attached (default is 0)
        /// \param layerOrFace texture layer or face to be attached
        void attachDepthTexture(Texture& target, GLuint mipLevel, GLuint layerOrFace);

        /// attach a texture as color target
        /// \param target texture with an internal depth format
        /// \param mipLevel mipmap level to be attached (default is 0)
        void attachColorTexture(GLuint index, Texture& target, GLuint mipLevel = 0);

        /// attach a single layer of a texture as color target
        /// \param target texture with an internal depth format
        /// \param mipLevel mipmap level to be attached (default is 0)
        /// \param layerOrFace texture layer or face to be attached (default is 0)
        void attachColorTexture(GLuint index, Texture& target, GLuint mipLevel, GLuint layerOrFace);

        /// Check frame buffer completeness once after all attachments are bound.
        /// Also, calls glDrawBuffers() to enable the attachments.
        void validate();

        /// Bind the framebuffer (all draw calls will draw on the bound framebuffer)
        void bind();

        /// Bind the default framebuffer (back buffer). This buffer is visible in the window
        static void bindDefaultFramebuffer();

        GLuint getID();
    private:
        void swap(Framebuffer& o) noexcept
        {
            std::swap(m_id, o.m_id);
            std::swap(m_drawBuffers, o.m_drawBuffers);
            std::swap(m_isValidated, o.m_isValidated);
        }

        GLuint m_id = 0;
        std::vector<GLenum> m_drawBuffers;
        bool m_isValidated = false;
    };
}
