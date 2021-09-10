#include "Window.h"
#include <vector>
#include <iostream>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <memory>

namespace gpupro
{
    // required for the mouse and keyboard callbacks
    static Window* s_window = nullptr;

    static void errorCallbackGLFW(int error, const char* description)
    {
        std::cerr << "ERR: GLFW error, code " << error << " desc: \"" << description << "\"\n";
    }


    static void glDebugOutput(GLenum _source, GLenum _type, GLuint _id, GLenum _severity, GLsizei /*_length*/,
                              const GLchar* _message, const void* /*_userParam*/)
    {
        std::string debSource, debType, debSev;

        if (_source == GL_DEBUG_SOURCE_API)
            debSource = "OpenGL";
        else if (_source == GL_DEBUG_SOURCE_WINDOW_SYSTEM)
            debSource = "Windows";
        else if (_source == GL_DEBUG_SOURCE_SHADER_COMPILER)
            debSource = "Shader Compiler";
        else if (_source == GL_DEBUG_SOURCE_THIRD_PARTY)
            debSource = "Third Party";
        else if (_source == GL_DEBUG_SOURCE_APPLICATION)
            debSource = "Application";
        else if (_source == GL_DEBUG_SOURCE_OTHER)
            debSource = "Other";

        if (_type == GL_DEBUG_TYPE_ERROR)
            debType = "error";
        else if (_type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR)
            debType = "deprecated behavior";
        else if (_type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
            debType = "undefined behavior";
        else if (_type == GL_DEBUG_TYPE_PORTABILITY)
            debType = "portability";
        else if (_type == GL_DEBUG_TYPE_PERFORMANCE)
            debType = "performance";
        else if (_type == GL_DEBUG_TYPE_OTHER)
            debType = "message";

        if (_severity == GL_DEBUG_SEVERITY_HIGH)
            debSev = "high";
        else if (_severity == GL_DEBUG_SEVERITY_MEDIUM)
            debSev = "medium";
        else if (_severity == GL_DEBUG_SEVERITY_LOW)
            debSev = "low";
        else if (_severity == GL_DEBUG_SEVERITY_NOTIFICATION)
            debSev = "note";

        std::string logMessage = debSource + ": " + debType + "(" + debSev + ") " + std::to_string(_id) + ": " +
            _message;
        if (_type == GL_DEBUG_TYPE_ERROR || _type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR)
            throw std::runtime_error(logMessage.c_str());
        else if (_type == GL_DEBUG_TYPE_PERFORMANCE)
            std::cerr << "INF: " << logMessage.c_str() << '\n';
        else
            std::cerr << "WAR: " << logMessage.c_str() << '\n';
    }

    Window::Window(const Desc& desc)
        :
        m_width(desc.width),
        m_height(desc.height)
    {
        if (s_window)
            throw std::runtime_error("Only one instance of Window should be open to work properly");

        s_window = this;

        glfwSetErrorCallback(errorCallbackGLFW);
        if (!glfwInit()) throw std::runtime_error("Cannot initialize GLFW!\n");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
        glfwWindowHint(GLFW_SAMPLES, desc.multisamples);
        glfwWindowHint(GLFW_SRGB_CAPABLE, GLFW_TRUE); // framebuffer should be srgb capable
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_RESIZABLE, desc.resizeable ? GL_TRUE : GL_FALSE);
        glfwWindowHint(GLFW_DEPTH_BITS, desc.depthBits);
        glfwWindowHint(GLFW_STENCIL_BITS, desc.stencilBits);

        m_handle = glfwCreateWindow(desc.width, desc.height, desc.title.c_str(), nullptr, nullptr);
        if (!m_handle) throw std::runtime_error("Window creation failed!");
        glfwMakeContextCurrent(m_handle);
        glfwSetWindowSizeLimits(m_handle, 100, 100, GLFW_DONT_CARE, GLFW_DONT_CARE);

        if (!gladLoadGL())
            throw std::runtime_error("Cannot initialize Glad/load gl-function pointers!\n");

        // opengl error handling
        const GLubyte* vendor = glGetString(GL_VENDOR);
        const GLubyte* renderer = glGetString(GL_RENDERER);
        const GLubyte* version = glGetString(GL_VERSION);

        //std::cout << "Vendor: " << vendor << "\nRenderer: " << renderer << "\nVersion: " << version << '\n';
        //std::cerr << "GL-context version: " << GLVersion.major << '.' << GLVersion.minor << '\n';

        // enable all debug warning notifications for our purposes (we want as much information as we can get)
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
        // this is not important (only stuff like: buffer created in VRAM etc.)
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_LOW, 0, nullptr, GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_MEDIUM, 0, nullptr,GL_TRUE);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_HIGH, 0, nullptr, GL_TRUE);
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(reinterpret_cast<GLDEBUGPROC>(&glDebugOutput), nullptr);

        // Enable seamless cube map sampling which is always a good idea since it is core (3.2). 
        glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
        // Set packing alignment to 1 to allow loading textures with a size that is not a multiple of 4
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glPixelStorei(GL_PACK_ALIGNMENT, 1);

        glfwSetWindowSizeCallback(m_handle, [](GLFWwindow* window, int width, int height)
        {
            s_window->resize(width, height);
        });
        glfwSetCursorPosCallback(m_handle, [](GLFWwindow* window, double x, double y)
        {
            auto prevX = s_window->getMouseXNorm();
            auto prevY = s_window->getMouseYNorm();
            // clamp because mouse coordinates can be outisde of the window when a button is down
            s_window->m_mouseX = static_cast<size_t>(glm::clamp(int(x), 0, int(s_window->m_width - 1)));
            // reverse y because of different coordinate system
            s_window->m_mouseY = static_cast<size_t>(glm::clamp(int(s_window->m_height) - int(y) - 1, 0,
                                                                int(s_window->m_height - 1)));

            auto newX = s_window->getMouseXNorm();
            auto newY = s_window->getMouseYNorm();

            if (s_window->m_onMouseMove && (prevX != newX || prevY != newY))
                s_window->m_onMouseMove(newX, newY, newX - prevX, newY - prevY);
        });
        glfwSetKeyCallback(m_handle, [](GLFWwindow* window, int key, int scancode, int action, int mods)
        {
            if (action == GLFW_RELEASE)
            {
                if (s_window->m_onKeyUp)
                    s_window->m_onKeyUp(Key(key));
            }
            else if (action == GLFW_PRESS || action == GLFW_REPEAT)
            {
                if (s_window->m_onKeyDown)
                    s_window->m_onKeyDown(Key(key));
            }
        });
        glfwSetMouseButtonCallback(m_handle, [](GLFWwindow* window, int button, int action, int mods)
        {
            if (action == GLFW_PRESS)
            {
                if (button == GLFW_MOUSE_BUTTON_LEFT) s_window->m_mouseLeftDown = true;
                if (s_window->m_onMouseDown)
                    s_window->m_onMouseDown(Button(button), s_window->getMouseXNorm(), s_window->getMouseYNorm());
            }
            else if (action == GLFW_RELEASE)
            {
                if (button == GLFW_MOUSE_BUTTON_LEFT) s_window->m_mouseLeftDown = false;
                if (s_window->m_onMouseUp)
                    s_window->m_onMouseUp(Button(button), s_window->getMouseXNorm(), s_window->getMouseYNorm());
            }
        });

        resize(desc.width, desc.height);
    }

    Window::~Window()
    {
        if (m_handle)
            glfwDestroyWindow(m_handle);
        glfwTerminate();
        s_window = nullptr;
    }

    void Window::handleEvents()
    {
        glfwPollEvents();
        m_open = !glfwWindowShouldClose(m_handle);
    }

    void Window::swapBuffer() const
    {
        glfwSwapBuffers(m_handle);
    }

    void Window::setTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_handle, title.c_str());
    }

    void Window::resize(size_t width, size_t height)
    {
        if (width == 0 || height == 0)
            return; // ignore (minimize)

        m_width = width;
        m_height = height;

        glViewport(0, 0, static_cast<GLsizei>(width), static_cast<GLsizei>(height));

        if (m_onSizeChange)
            m_onSizeChange(int(m_width), int(m_height));
    }
}
