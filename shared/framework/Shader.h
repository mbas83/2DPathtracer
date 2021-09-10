#pragma once
#include <glad/glad.h>
#include <fstream>
#include <sstream>

namespace gpupro
{
    class Shader
    {
    public:
        Shader(GLenum type)
            :
            m_id(glCreateShader(type))
        {
        }

        Shader(const Shader&) = delete;
        Shader& operator=(const Shader&) = delete;

        Shader(Shader&& o) noexcept
        {
            swap(o);
        }

        Shader& operator=(Shader&& o) noexcept
        {
            swap(o);
            return *this;
        }

        ~Shader()
        {
            if (m_id)
            {
                glDeleteShader(m_id);
                m_id = 0;
            }
        }

        /// \brief loads the shader source from a file
        /// \param filename file that should be loaded
        /// \returns reference to self
        Shader& loadFromFile(const std::string& filename)
        {
            // create input file stream
            std::ifstream file;
            file.open(filename.c_str());

            if (!file.is_open())
                throw std::runtime_error("could not find " + filename);

            // string stream to convert file into string
            std::stringstream sstream;
            sstream << file.rdbuf();
            file.close();

            auto source = sstream.str();

            try
            {
                loadFromSource(source);
            }
            catch (const std::exception& e)
            {
                throw std::runtime_error("Error in " + (filename + "\n") + e.what());
            }

            return *this;
        }

        /// \brief loads the shader source from a string
        /// \param source string that should be used as shader source
        /// \returns reference to self
        Shader& loadFromSource(const std::string& source)
        {
            auto src = source.c_str();
            glShaderSource(m_id, 1, &src, nullptr);

            glCompileShader(m_id);

            GLint isCompiled = GL_FALSE;
            glGetShaderiv(m_id, GL_COMPILE_STATUS, &isCompiled);

            if (isCompiled == GL_FALSE)
            {
                // Read out the error message.
                GLint length = 0;
                glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
                std::string errorLog;
                errorLog.resize(length);
                glGetShaderInfoLog(m_id, length, &length, &errorLog[0]);

                errorLog = "Failed to compile shader\n" + errorLog;
                throw std::runtime_error(errorLog);
            }
            return *this;
        }

        /// \returns opengl id
        GLuint getId() const
        {
            return m_id;
        }

        /// \returns a vertex shader that creates a screen filling quad from four (empty) input vertices using the vertex ID
        static Shader createScreenFillingQuadVertexShader()
        {
            Shader vertex(GL_VERTEX_SHADER);
            vertex.loadFromSource(
                R"(
        #version 440 core
        void main() {
            vec4 vertex = vec4(0.0, 0.0, 0.0, 1.0);
            if(uint(gl_VertexID) == 0u) vertex = vec4(1.0, -1.0, 0.0, 1.0);
            if(uint(gl_VertexID) == 1u) vertex = vec4(-1.0, -1.0, 0.0, 1.0);
            if(uint(gl_VertexID) == 2u) vertex = vec4(1.0, 1.0, 0.0, 1.0);
            if(uint(gl_VertexID) == 3u) vertex = vec4(-1.0, 1.0, 0.0, 1.0);
            gl_Position = vertex;
        })");
            return vertex;
        }

        /// \returns a fragment shader that outputs the texture at slot 0 based on the gl_FragCoord
        static Shader createScreenFillingTextureFragmentShader()
        {
            Shader fragment(GL_FRAGMENT_SHADER);
            fragment.loadFromSource(
                R"(
            #version 440 core
            uniform sampler2D tex;
            out vec4 fragColor;
            void main() {
                fragColor = texelFetch(tex, ivec2(gl_FragCoord.xy), 0);
        })");
            return fragment;
        }

    private:
        void swap(Shader& o) noexcept
        {
            std::swap(m_id, o.m_id);
        }

        GLuint m_id = 0;
    };
}
