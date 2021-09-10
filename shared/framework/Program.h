#pragma once
#include <algorithm>
#include <vector>
#include "Shader.h"

namespace gpupro
{
    class Program
    {
    public:
        Program()
            :
            m_id(glCreateProgram())
        {
        }

        ~Program()
        {
            if (m_id)
            {
                glDeleteProgram(m_id);
                m_id = 0;
            }
        }

        Program(const Program&) = delete;
        Program& operator=(const Program&) = delete;

        Program(Program&& o) noexcept
        {
            swap(o);
        }

        Program& operator=(Program&& o) noexcept
        {
            swap(o);
            return *this;
        }

        Program& attachVertexShader(const std::string& filename)
        {
            Shader shader(GL_VERTEX_SHADER);
            shader.loadFromFile(filename);
            return attach(std::move(shader));
        }

        Program& attachFragmentShader(const std::string& filename)
        {
            Shader shader(GL_FRAGMENT_SHADER);
            shader.loadFromFile(filename);
            return attach(std::move(shader));
        }

        Program& attachGeometryShader(const std::string& filename)
        {
            Shader shader(GL_GEOMETRY_SHADER);
            shader.loadFromFile(filename);
            return attach(std::move(shader));
        }

        Program& attachComputeShader(const std::string& filename)
        {
            Shader shader(GL_COMPUTE_SHADER);
            shader.loadFromFile(filename);
            return attach(std::move(shader));
        }

        Program& attachTesselationShader(const std::string& controlShaderFilename,
                                         const std::string& evaluationShaderFilename)
        {
            Shader control(GL_TESS_CONTROL_SHADER);
            control.loadFromFile(controlShaderFilename);
            attach(std::move(control));
            Shader eval(GL_TESS_EVALUATION_SHADER);
            eval.loadFromFile(evaluationShaderFilename);
            return attach(std::move(eval));
        }

        /// \brief links all shaders
        /// \return reference to self
        Program& link()
        {
            // program should only be linked once!
            dassert(!m_isLinked);

            // Link all attached shaders (if possible)
            glLinkProgram(m_id);

            for (const auto& s : m_attachments)
                glDetachShader(m_id, s.getId());
            m_attachments.clear();

            GLint isLinked = GL_FALSE;
            glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);
            if (isLinked == GL_FALSE)
            {
                // Read out the error message.
                GLint length = 0;
                glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
                std::string errorLog;
                errorLog.resize(length);
                glGetProgramInfoLog(m_id, length, &length, &errorLog[0]);
                errorLog = std::string("Linker error: ") + errorLog;
                throw std::runtime_error(errorLog.c_str());
            }

            m_isLinked = true;
            return *this;
        }

        /// \brief binds the program on the gpu
        void bind() const
        {
            dassert(m_isLinked); // call .link() first before using bind
            glUseProgram(m_id);
        }


        /// attachs a shader to the program
        /// \param shader loaded shader
        /// \return reference to self
        Program& attach(Shader shader)
        {
            // don't add shader after the program was compiled!
            dassert(!m_isLinked);
            glAttachShader(m_id, shader.getId());

            m_attachments.emplace_back(std::move(shader));
            return *this;
        }

    private:
        void swap(Program& o) noexcept
        {
            std::swap(m_id, o.m_id);
            std::swap(m_attachments, o.m_attachments);
            std::swap(m_isLinked, o.m_isLinked);
        }

        GLuint m_id = 0;
        std::vector<Shader> m_attachments;
        bool m_isLinked = false;
    };
}
