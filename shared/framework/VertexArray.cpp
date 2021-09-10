#include "VertexArray.h"
#include "constants.h"
#include "dassert.h"

gpupro::VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_id);
}

gpupro::VertexArray::~VertexArray()
{
    if (m_id)
    {
        glDeleteVertexArrays(1, &m_id);
        m_id = 0;
    }
}

gpupro::VertexArray::VertexArray(VertexArray&& o) noexcept
{
    swap(o);
}

gpupro::VertexArray& gpupro::VertexArray::operator=(VertexArray&& o) noexcept
{
    swap(o);
    return *this;
}

int GetOpenglConstant(GLenum name)
{
    int res = 0;
    glGetIntegerv(name, &res);
    return res;
}

gpupro::VertexArray& gpupro::VertexArray::addBinding(GLuint bindingIndex, GLuint attributeIndex, VertexType type,
                                                     GLuint numComponents, GLuint offset)
{
    glBindVertexArray(m_id); // activate binding
    dassert(attributeIndex < getOpenglConstant<int>(GL_MAX_VERTEX_ATTRIBS));
    glEnableVertexAttribArray(attributeIndex);
    // glVertexAttribDivisor(attributeIndex, 1); // not used here
    // the following methods specify the type and size of one vertex
    // unfortunately, different methods need to be used depending if
    // we works with a double, floating point types or integer types
    if (type == VertexType::DOUBLE)
        glVertexAttribLFormat(attributeIndex, numComponents, GLenum(type), offset);
    else if (isIntegerType(type))
        glVertexAttribIFormat(attributeIndex, numComponents, GLenum(type), offset);
    else
        glVertexAttribFormat(attributeIndex, numComponents, GLenum(type), GL_FALSE, offset);
    // note that the last "glVertexAttribFormat" can also use integer inputs and convert them to floats
    // however I never needed to use this feature and it usually just causes confusion in the exercises
    dassert(bindingIndex < GLuint(getOpenglConstant<int>(GL_MAX_VERTEX_ATTRIB_BINDINGS)));
    glVertexAttribBinding(attributeIndex, bindingIndex);

    glBindVertexArray(0); // deactivate binding
    return *this;
}

void gpupro::VertexArray::bind() const
{
    glBindVertexArray(m_id);
}

bool gpupro::VertexArray::isIntegerType(VertexType t)
{
    switch (t)
    {
    case VertexType::BYTE:
    case VertexType::UNSIGNED_BYTE:
    case VertexType::SHORT:
    case VertexType::UNSIGNED_SHORT:
    case VertexType::INT:
    case VertexType::UNSIGNED_INT:
        return true;
    default:
        return false;
    }
}
