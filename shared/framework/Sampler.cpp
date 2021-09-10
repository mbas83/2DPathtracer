#include "Sampler.h"

gpupro::Sampler::Sampler(Wrap wrapping, MagnifyFilter magnify, MinifyFilter minify, bool useAnisotropyFilter)
{
    glGenSamplers(1, &m_id);
    // wrapping on borders
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_R, static_cast<GLint>(wrapping));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_S, static_cast<GLint>(wrapping));
    glSamplerParameteri(m_id, GL_TEXTURE_WRAP_T, static_cast<GLint>(wrapping));
    // minify and magnify modes
    glSamplerParameteri(m_id, GL_TEXTURE_MAG_FILTER, static_cast<GLint>(magnify));
    glSamplerParameteri(m_id, GL_TEXTURE_MIN_FILTER, static_cast<GLint>(minify));
    if (useAnisotropyFilter)
    {
        // determine maximum possible anisotropic filter setting
        float maxAniso = 1.0f;
        glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAniso);
        glSamplerParameterf(m_id, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAniso);
    }
}

gpupro::Sampler::~Sampler()
{
    if (m_id)
    {
        glDeleteSamplers(1, &m_id);
        m_id = 0;
    }
}

gpupro::Sampler::Sampler(Sampler&& o) noexcept
{
    swap(o);
}

gpupro::Sampler& gpupro::Sampler::operator=(Sampler&& o) noexcept
{
    swap(o);
    return *this;
}

void gpupro::Sampler::bind(GLuint bindingIndex) const
{
    glBindSampler(bindingIndex, m_id);
}
