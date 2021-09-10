#include "Framebuffer.h"

#include "dassert.h"

gpupro::Framebuffer::Framebuffer()
{
    glGenFramebuffers(1, &m_id);
}

gpupro::Framebuffer::~Framebuffer()
{
    if (m_id != 0)
    {
        glDeleteFramebuffers(1, &m_id);
        m_id = 0;
    }
}

gpupro::Framebuffer::Framebuffer(Framebuffer&& o) noexcept
{
    swap(o);
}

gpupro::Framebuffer& gpupro::Framebuffer::operator=(Framebuffer&& o) noexcept
{
    swap(o);
    return *this;
}

void gpupro::Framebuffer::attachDepthTexture(Texture& target, GLuint mipLevel)
{
    if (!isDepthFormat(target.getFormat()))
        throw std::runtime_error(
            "Framebuffer::attachDepthTarget cannot attach the texture as depth target. It has not a depth format");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
    glFramebufferTexture(
        GL_DRAW_FRAMEBUFFER, isStencilFormat(target.getFormat()) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT,
        target.getID(), mipLevel);

}

void gpupro::Framebuffer::attachDepthTexture(Texture& target, GLuint mipLevel, GLuint layerOrFace)
{
    if (!isDepthFormat(target.getFormat()))
        throw std::runtime_error(
            "Framebuffer::attachDepthTarget cannot attach the texture as depth target. It has not a depth format");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
    glFramebufferTextureLayer(
        GL_DRAW_FRAMEBUFFER, isStencilFormat(target.getFormat()) ? GL_DEPTH_STENCIL_ATTACHMENT : GL_DEPTH_ATTACHMENT,
        target.getID(), mipLevel, layerOrFace);
}

void gpupro::Framebuffer::attachColorTexture(GLuint index, Texture& target, GLuint mipLevel)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
    glFramebufferTexture(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target.getID(), mipLevel);
    
    // To enable all the color targets we need a call to glDrawBuffers.
    // Since this must be done for all targets at the same time, collect the
    // current buffer.
    m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
}

void gpupro::Framebuffer::attachColorTexture(GLuint index, Texture& target, GLuint mipLevel, GLuint layerOrFace)
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
    glFramebufferTextureLayer(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, target.getID(), mipLevel, layerOrFace);

    // To enable all the color targets we need a call to glDrawBuffers.
    // Since this must be done for all targets at the same time, collect the
    // current buffer.
    m_drawBuffers.push_back(GL_COLOR_ATTACHMENT0 + index);
}

void gpupro::Framebuffer::validate()
{
    if (m_isValidated)
        throw std::runtime_error("Frambuffer::validate framebuffer was already validated");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
    GLenum status = glCheckFramebufferStatus(GL_DRAW_FRAMEBUFFER);
    switch (status)
    {
    case GL_FRAMEBUFFER_COMPLETE: break; // framebuffer complete!
    case GL_FRAMEBUFFER_UNDEFINED:
        throw std::runtime_error("Framebuffer::validate status returned GL_FRAMEBUFFER_UNDEFINED");
    case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
        throw std::runtime_error("Framebuffer::validate at least one of the attachments is incomplete");
    case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
        throw std::runtime_error("Framebuffer::validate nothing is attached to the framebuffer");
    case GL_FRAMEBUFFER_UNSUPPORTED:
        throw std::runtime_error(
            "Framebuffer::validate the combination of texture formats violates an implementation-dependent set of restrictions");
    default:
        throw std::runtime_error("Framebuffer::validate invalid framebuffer");
    }

    // Enable all the attached targets from the list.
    glDrawBuffers(static_cast<GLsizei>(m_drawBuffers.size()), m_drawBuffers.data());
    m_isValidated = true;

    // unbind framebuffer after everything is set up (otherwise the next draw call will write into this buffer)
    bindDefaultFramebuffer();
}

void gpupro::Framebuffer::bind()
{
    if (!m_isValidated)
        throw std::runtime_error(
            "Framebuffer::bind framebuffer needs to be validated before first use (call Framebuffer::validate())");

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
}

void gpupro::Framebuffer::bindDefaultFramebuffer()
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
}

GLuint gpupro::Framebuffer::getID()
{
    return m_id;
}
