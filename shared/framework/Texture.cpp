#include "Texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include <iostream>

#include "Sampler.h"
#include <stb_image.h>
#include "dassert.h"
#include <gli/gli.hpp>
#include <gli/make_texture.hpp>

gpupro::Texture::Texture(TextureLayout layout, InternalFormat format, GLuint mipLevels, GLsizei width, GLsizei height,
                         GLsizei depthOrArraySize):
    m_size{width, height, depthOrArraySize},
    m_layout(layout),
    m_format(format),
    m_mipLevels(mipLevels),
    m_faces((m_layout == TextureLayout::CUBE_MAP || m_layout == TextureLayout::CUBE_MAP_ARRAY) ? 6 : 1)
{
    dassert(width > 0);
    dassert(height > 0);
    dassert(depthOrArraySize > 0);

    glGenTextures(1, &m_id);
    glBindTexture(static_cast<GLenum>(m_layout), m_id);

    if (m_mipLevels == MAX_MIP_LEVELS) // compute max number of mip levels
        m_mipLevels = computeMaxMipLevels(
            std::max(width, std::max(height, m_layout == TextureLayout::TEX_3D ? depthOrArraySize : 1)));
    dassert(mipLevels > 0);

    switch (m_layout)
    {
    case TextureLayout::TEX_1D:
        if (height != 1 || depthOrArraySize != 1)
            throw std::runtime_error("Texture height and depthOrArraySize should be 1 for 1D Textures");
        glTexStorage1D(static_cast<GLenum>(m_layout), m_mipLevels, static_cast<GLenum>(m_format), m_size[0]);
        break;
    case TextureLayout::TEX_2D:
    case TextureLayout::CUBE_MAP:
        if (depthOrArraySize != 1)
            throw std::runtime_error("Texture depthOrArraySize should be 1 for 2D/1D Textures and Cube Maps");
        glTexStorage2D(static_cast<GLenum>(m_layout), m_mipLevels, static_cast<GLenum>(m_format), m_size[0], m_size[1]);
        break;
    case TextureLayout::TEX_3D:
    case TextureLayout::TEX_2D_ARRAY:
    case TextureLayout::CUBE_MAP_ARRAY:
        glTexStorage3D(static_cast<GLenum>(m_layout), m_mipLevels, static_cast<GLenum>(m_format), m_size[0], m_size[1],
                       m_size[2] * m_faces);
        break;
    }
}

gpupro::Texture::Texture(Texture&& o) noexcept
{
    swap(o);
}

gpupro::Texture& gpupro::Texture::operator=(Texture&& o) noexcept
{
    swap(o);
    return *this;
}

gpupro::Texture::~Texture()
{
    if (m_id)
    {
        glDeleteTextures(1, &m_id);
        m_id = 0;
    }
}

void gpupro::Texture::bindAsTexture(GLuint bindingIndex) const
{
    dassert(m_id);
    dassert(bindingIndex >= 0);
    dassert(bindingIndex < 32);
    glActiveTexture(GL_TEXTURE0 + bindingIndex);
    glBindTexture(static_cast<GLenum>(m_layout), m_id);
}

void gpupro::Texture::bindAsImage(GLuint bindingIndex, GLuint mipLevel, GLint layer)
{
    dassert(m_id);
    dassert(mipLevel < GLuint(m_mipLevels));
    dassert(layer < getDepthOrArraySize());

    // always use read + write access for simplicity
    if (layer == ALL_LAYERS)
        glBindImageTexture(bindingIndex, m_id, mipLevel, GL_TRUE, 0, GL_READ_WRITE, static_cast<GLenum>(m_format));
    else
        glBindImageTexture(bindingIndex, m_id, mipLevel, GL_FALSE, layer, GL_READ_WRITE, static_cast<GLenum>(m_format));
}

GLsizei gpupro::Texture::getWidth() const
{
    return m_size[0];
}

GLsizei gpupro::Texture::getHeight() const
{
    return m_size[1];
}

GLsizei gpupro::Texture::getDepthOrArraySize() const
{
    return m_size[2];
}

GLuint gpupro::Texture::getFaces() const
{
    return m_faces;
}

GLuint gpupro::Texture::getNumMipLevels() const
{
    return m_mipLevels;
}

gpupro::InternalFormat gpupro::Texture::getFormat() const
{
    return m_format;
}

GLuint gpupro::Texture::getID()
{
    return m_id;
}

GLuint64 gpupro::Texture::getTextureHandle() const
{
    dassert(m_id);
    auto handle = glGetTextureHandleARB(m_id);
    glMakeTextureHandleResidentARB(handle);
    return handle;
}

GLuint64 gpupro::Texture::getTextureSamplerHandle(const Sampler& sampler) const
{
    dassert(m_id);
    auto handle = glGetTextureSamplerHandleARB(m_id, sampler.getID());
    glMakeTextureHandleResidentARB(handle);
    return handle;
}

void gpupro::Texture::setData(SetDataFormat format, SetDataType type, const void* data, GLuint mipLevel, GLuint layer,
                              GLuint face)
{
    dassert(m_id);
    dassert(face < m_faces);
    dassert(mipLevel < m_mipLevels);

    // calc correct mipmap size (divide by two for each mip level)
    GLsizei mipSize[3];
    mipSize[0] = std::max(m_size[0] >> mipLevel, 1);
    mipSize[1] = std::max(m_size[1] >> mipLevel, 1);
    mipSize[2] = std::max(m_size[2] >> mipLevel, 1);

    glBindTexture(static_cast<GLenum>(m_layout), m_id);
    switch (m_layout)
    {
    case TextureLayout::TEX_1D:
        dassert(layer == 0);
        glTexSubImage1D(static_cast<GLenum>(m_layout), mipLevel, 0, mipSize[0], static_cast<GLenum>(format),
                        static_cast<GLenum>(type), data);
        break;
    case TextureLayout::TEX_2D:
        dassert(layer == 0);
        glTexSubImage2D(static_cast<GLenum>(m_layout), mipLevel, 0, 0, mipSize[0], mipSize[1],
                        static_cast<GLenum>(format), static_cast<GLenum>(type), data);
        break;
    case TextureLayout::TEX_3D:
        dassert(layer == 0);
        glTexSubImage3D(static_cast<GLenum>(m_layout), mipLevel, 0, 0, 0, mipSize[0], mipSize[1], mipSize[2],
                        static_cast<GLenum>(format), static_cast<GLenum>(type), data);
        break;
    case TextureLayout::CUBE_MAP:
        dassert(layer == 0);
        glTexSubImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face, mipLevel, 0, 0, mipSize[0], mipSize[1],
                        static_cast<GLenum>(format), static_cast<GLenum>(type), data);
        break;
    case TextureLayout::TEX_2D_ARRAY:
        dassert(layer < GLuint(m_size[2]));
        glTexSubImage3D(static_cast<GLenum>(m_layout), mipLevel, 0, 0, layer, mipSize[0], mipSize[1], 1,
                        static_cast<GLenum>(format), static_cast<GLenum>(type), data);
        break;
    case TextureLayout::CUBE_MAP_ARRAY:
        dassert(layer < GLuint(m_size[2] / 6));
        glTexSubImage3D(static_cast<GLenum>(m_layout), mipLevel, 0, 0, layer * 6 + face, mipSize[0], mipSize[1], 1,
                        static_cast<GLenum>(format), static_cast<GLenum>(type), data);
        break;
    }
}

GLuint gpupro::Texture::computeMaxMipLevels(GLsizei maxResolution)
{
    GLuint maxMip = 1;
    while ((maxResolution /= 2) > 0) ++maxMip;

    return maxMip;
}

/////////// LOAD AND SAVE FROM FILE METHODS ////////////

gpupro::Texture gpupro::Texture::loadFromFile(const std::string& filename, bool generateMipmaps, bool ldrIsSrgb,
                                              int* outNumChannels)
{
    auto extension = filename.substr(filename.find_last_of('.') + 1);

    if (extension == "dds" || extension == "ktx") // ktx and dds textures
    {
        auto gliTex = gli::load(filename);
        if (gliTex.empty())
            throw std::runtime_error("could not load image: " + filename);
        gliTex = gli::flip(gliTex); // textures in opengl need to be loaded from bottom to top...

        // create gli opengl context and extract format information
        static gli::gl GL(gli::gl::PROFILE_GL33);
        const auto format = GL.translate(gliTex.format(), gliTex.swizzles());
        const TextureLayout layout = (TextureLayout)GL.translate(gliTex.target());
        const auto extend = gliTex.extent();
        const auto setDataFormat = (SetDataFormat)format.External;
        const auto setDataType = (SetDataType)format.Type;

        // create texture
        auto mipLevels = gliTex.levels();
        if (mipLevels == 1 && generateMipmaps) mipLevels = MAX_MIP_LEVELS;
        Texture tex(layout, InternalFormat(format.Internal), GLuint(mipLevels), extend.x, extend.y,
                    GLsizei(extend.z * gliTex.layers()));
        // set swizzle parameters that are present in some ktx files
        glTexParameteri(static_cast<GLenum>(tex.m_layout), GL_TEXTURE_SWIZZLE_R, format.Swizzles[0]);
        glTexParameteri(static_cast<GLenum>(tex.m_layout), GL_TEXTURE_SWIZZLE_G, format.Swizzles[1]);
        glTexParameteri(static_cast<GLenum>(tex.m_layout), GL_TEXTURE_SWIZZLE_B, format.Swizzles[2]);
        glTexParameteri(static_cast<GLenum>(tex.m_layout), GL_TEXTURE_SWIZZLE_A, format.Swizzles[3]);

        // set data
        for (GLuint layer = 0; layer < gliTex.layers(); ++layer) // layer == array layer
            for (GLuint face = 0; face < gliTex.faces(); ++face) // face = cubemap face
                for (GLuint level = 0; level < gliTex.levels(); ++level) // level == mipmap
                {
                    tex.setData(setDataFormat, setDataType, gliTex.data(layer, face, level), level, layer, face);
                }

        if (mipLevels == MAX_MIP_LEVELS)
            glGenerateMipmap(static_cast<GLenum>(tex.m_layout));
        if (outNumChannels) *outNumChannels = gli::is_channel(gliTex.swizzles()[0]) +
            gli::is_channel(gliTex.swizzles()[1]) + gli::is_channel(gliTex.swizzles()[2]) +
            gli::is_channel(gliTex.swizzles()[3]); // count number of valid channels
        return tex;
    }
    else // load png jpg bmp 
    {
        stbi_set_flip_vertically_on_load(1); // opengl expect images from bottom top top but the are generally stored top to bottom
        int nChannels;
        int width;
        int height;
        // for simplicity always expect 4 channels
        const auto data = stbi_load(filename.c_str(), &width, &height, &nChannels, 4);
        if (!data)
            throw std::runtime_error("could not load image: " + filename);

        auto format = InternalFormat::RGBA8;
        if (ldrIsSrgb) format = InternalFormat::SRGB8_ALPHA8; // use srgb color format
        Texture tex(TextureLayout::TEX_2D, format, generateMipmaps ? MAX_MIP_LEVELS : 1, width, height);
        tex.setData(SetDataFormat::RGBA, SetDataType::UINT8, data);

        stbi_image_free(data);

        if (generateMipmaps)
            glGenerateMipmap(GLenum(TextureLayout::TEX_2D));
        if (outNumChannels) *outNumChannels = nChannels;

        return tex;
    }
}

void gpupro::Texture::saveToFile(const std::string& filename)
{
    auto extension = filename.substr(filename.find_last_of('.') + 1);

    if (extension == "dds" || extension == "ktx")
    {
        // obtain gli format information
        static gli::gl GL(gli::gl::PROFILE_GL33);
        gli::gl::external_format external;
        gli::gl::type_format type_format;
        auto gliFormat = GL.findEx(gli::gl::internal_format(m_format), &external, &type_format);
        if (gliFormat == gli::format::FORMAT_UNDEFINED)
            throw std::runtime_error("Texture::saveToFile() could not convert internal image format to gli format");

        // create texture storage based on layout
        gli::texture tex;
        switch (m_layout)
        {
        case TextureLayout::TEX_1D:
            tex = gli::make_texture1d(gliFormat, gli::extent1d(m_size[0]), m_mipLevels);
            break;
        case TextureLayout::TEX_2D:
            tex = gli::make_texture2d(gliFormat, gli::extent2d(m_size[0], m_size[1]), m_mipLevels);
            break;
        case TextureLayout::TEX_3D:
            tex = gli::make_texture3d(gliFormat, gli::extent3d(m_size[0], m_size[1], m_size[2]), m_mipLevels);
            break;
        case TextureLayout::CUBE_MAP:
            tex = gli::make_texture_cube(gliFormat, gli::extent2d(m_size[0], m_size[1]), m_mipLevels);
            break;
        case TextureLayout::TEX_2D_ARRAY:
            tex = gli::make_texture2d_array(gliFormat, gli::extent2d(m_size[0], m_size[1]), m_size[2], m_mipLevels);
            break;
        case TextureLayout::CUBE_MAP_ARRAY:
            tex = gli::make_texture_cube_array(gliFormat, gli::extent2d(m_size[0], m_size[1]), m_size[2], m_mipLevels);
            break;
        default: throw std::runtime_error("Texture::saveToFile() unsupported texture layout for " + extension);
        }

        // download image data from GPU
        glBindTexture(static_cast<GLenum>(m_layout), m_id);
        if (tex.layers() == 1 && tex.faces() == 1)
        {
            // single layer texture
            for (GLuint level = 0; level < tex.levels(); ++level) // level == mipmap
            {
                glGetTexImage(static_cast<GLenum>(m_layout), level, static_cast<GLenum>(external),
                              static_cast<GLenum>(type_format), tex.data(0, 0, level));
            }
        }
        else
            // multi layer textures need a special extension function to extract single layers correctly (GL_ARB_get_texture_sub_image)
        {
            // layout = 2D_ARRAY, CUBE/_ARRAY
            if (!glGetTextureSubImage)
                throw std::runtime_error(
                    "Texture::saveToFile() cannot save textures with multiple layers because GL_ARB_get_texture_sub_image is not supported on this device");

            for (GLuint layer = 0; layer < tex.layers(); ++layer) // layer == array layer
                for (GLuint face = 0; face < tex.faces(); ++face) // face = cubemap face
                    for (GLuint level = 0; level < tex.levels(); ++level) // level == mipmap
                    {
                        auto s = tex.extent(level);
                        auto gliSize = GLsizei(tex.size(level)); // this should be size for single layer/face
                        auto layerStart = GLint(layer * tex.faces() + face);
                        glGetTextureSubImage(m_id, level, 0, 0, layerStart, s.x, s.y, 1, static_cast<GLenum>(external),
                                             static_cast<GLenum>(type_format), gliSize, tex.data(layer, face, level));
                    }
        }

        // data from opengl is bottom to top and not top to bottom...
        tex = gli::flip(tex);

        // save to disc
        if (extension == "ktx")
        {
            if (!gli::save_ktx(tex, filename))
                throw std::runtime_error("could not write " + filename);
        }
        else
        {
            if (!gli::save_dds(tex, filename))
                throw std::runtime_error("could not write " + filename);
        }
        std::cerr << "saved " << filename << '\n';
    }
    else throw std::runtime_error("Texture::saveToFile() extension not supported: ." + extension);
}
