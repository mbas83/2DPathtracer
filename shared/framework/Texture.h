#pragma once
#include <stdexcept>
#include <optional>
#include "format.h"

namespace gpupro
{
    class Sampler;

    enum class TextureLayout
    {
        TEX_1D = GL_TEXTURE_1D,
        TEX_2D = GL_TEXTURE_2D,
        TEX_3D = GL_TEXTURE_3D,
        CUBE_MAP = GL_TEXTURE_CUBE_MAP,
        TEX_2D_ARRAY = GL_TEXTURE_2D_ARRAY,
        CUBE_MAP_ARRAY = GL_TEXTURE_CUBE_MAP_ARRAY,
    };

    // A texture. Can also be used as render-target and image (random
    // read write access).
    class Texture
    {
    public:
        static constexpr GLuint MAX_MIP_LEVELS = GLuint(-1);
        static constexpr GLint ALL_LAYERS = GLint(-1);

        /// creates storage for a texture
        /// \param layout texture layout (for example TextureLayout::TEX_2D or TextureLayout::TEX_3D)
        /// \param format pixel format (for example InternalFormat::RGBA8 for 8bit pixels with Red Green Blue Alpha Channel)
        /// \param mipLevels number of mipmap levels, needs to be at least 1. Set to Texture::MAX_MIP_LEVELS to get the highest possible number of mipmap levels
        /// \param width of the texture
        /// \param height of the texture (should be 1 for 1D textures)
        /// \param depthOrArraySize depth for 3D textures and array size for 2D texture arrays and cube map arrays (should be 1 otherwise)
        Texture(TextureLayout layout, InternalFormat format, GLuint mipLevels, GLsizei width, GLsizei height,
                GLsizei depthOrArraySize = 1);

        /// loads a texture with the following formats: png, jpg, dds and ktx. 
        /// png and jpg files are loaded as RGBA8 files
        /// dds and ktx keep their original file format
        /// \param filename file on disc
        /// \param generateMipmaps indicates if a full mip map chain should be generated (this will be ignored for ktx and dds if the number of internal mipmap levels bigger than one)
        /// \param ldrIsSrgb for ldr images (png, jpg): if true, assumes that the image format is an SRGB format, otherwise an RGB will be used
        /// \param outNumChannels (optional) if not nullptr, this parameter will be filled with the information about how many channels were in the original file (1,2,3 or 4)
        static Texture loadFromFile(const std::string& filename, bool generateMipmaps = true,
                                    bool ldrIsSrgb = true, int* outNumChannels = nullptr);

        Texture() = default; // empty contructor (m_id = 0)
        Texture(const Texture&) = delete;
        Texture& operator=(const Texture&) = delete;
        Texture(Texture&& o) noexcept;
        Texture& operator=(Texture&& o) noexcept;
        ~Texture();

        /// bind as sampled texture
        /// \param bindingIndex shader binding index between 0 and 31 (do NOT use GL_TEXTURE0 etc.!) (layout(binding = X) uniform sampler2D mytexture;)
        void bindAsTexture(GLuint bindingIndex) const;

        /// bind as read + write image
        /// \param bindingIndex shader binding index (layout(binding = X) uniform image2D myimage;)
        void bindAsImage(GLuint bindingIndex, GLuint mipLevel = 0, GLint layer = ALL_LAYERS);

        /// uploads texture data
        /// \param format of the data   
        /// \param type of the data
        /// \param data data to be uploaded
        /// \param mipLevel destination mip level
        /// \param layer destination layer (for texture arrays)
        /// \param face destination face (for cubemaps in [0, 5])
        void setData(SetDataFormat format, SetDataType type, const void* data, GLuint mipLevel = 0, GLuint layer = 0,
                     GLuint face = 0);

        GLsizei getWidth() const;
        GLsizei getHeight() const;
        GLsizei getDepthOrArraySize() const;
        /// returns 6 for cubemaps, 1 otherwise
        GLuint getFaces() const;
        GLuint getNumMipLevels() const;
        InternalFormat getFormat() const;

        GLuint getID();

        void saveToFile(const std::string& filename);

        /// bindless texture extension (https://www.khronos.org/opengl/wiki/Bindless_Texture)
        /// This function creates a bindless handle and makes it resident
        /// use "layout(bindless_sampler) uniform;" in the shader to enable bindless textures for all samplers
        /// \return (resident) bindless texture handle
        GLuint64 getTextureHandle() const;

        GLuint64 getTextureSamplerHandle(const Sampler& sampler) const;
    private:

        static GLuint computeMaxMipLevels(GLsizei maxResolution);

        void swap(Texture& o) noexcept
        {
            std::swap(m_id, o.m_id);
            std::swap(m_size[0], o.m_size[0]);
            std::swap(m_size[1], o.m_size[1]);
            std::swap(m_size[2], o.m_size[2]);
            std::swap(m_layout, o.m_layout);
            std::swap(m_format, o.m_format);
            std::swap(m_mipLevels, o.m_mipLevels);
            std::swap(m_faces, o.m_faces);
        }

        GLuint m_id = 0;
        GLsizei m_size[3];
        TextureLayout m_layout;
        InternalFormat m_format;
        GLuint m_mipLevels;
        GLuint m_faces = 1; // for cubemaps, otherwise 1
    };
}
