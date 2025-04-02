#include "OpenGL/OpenGLWindow.hpp"

#include "Atlas/Events/ApplicationEvent.hpp"
#include "Atlas/Events/KeyEvent.hpp"
#include "Atlas/Events/MouseEvent.hpp"
#include "Atlas/Log.hpp"
#include "Core.hpp"

namespace Atlas {

static bool s_GLFWInitialized = false;

OpenGLWindow::OpenGLWindow(const WindowProps& props) {
    Init(props);
}

OpenGLWindow::~OpenGLWindow() {
    Shutdown();
}

Scope<Window> Window::Create(const WindowProps& props) {
    return CreateScope<OpenGLWindow>(props);
}

void OpenGLWindow::Init(const WindowProps& props) {
    m_Data.Title  = props.Title;
    m_Data.Width  = props.Width;
    m_Data.Height = props.Height;

    AT_CORE_TRACE("Creating window: {0} ({1} , {2})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized) {
        // NOTE: glfwTerminate on system shutdown
        if (!glfwInit()) {
            glfwSetErrorCallback([](int error, const char* description){
                AT_CORE_ERROR("Could not intialize GLFW!");
                assert(true);
            });
        }

        s_GLFWInitialized = true;
    }
    m_Window = glfwCreateWindow((int) props.Width, (int) props.Height, m_Data.Title.c_str(), nullptr, nullptr);
    glfwMakeContextCurrent(m_Window);

    int version_glad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    if (version_glad == 0) {
        AT_CORE_ERROR("ERROR: Failed to initialize OpenGL context.\n");
        assert(true);
    }

    glfwSetWindowUserPointer(m_Window, &m_Data);
    SetVSync(true);

    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
        data.Width       = width;
        data.Height      = height;

        WindowResizeEvent event(width, height);
        data.EventCallback(event);
    });
    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);
        WindowCloseEvent event;
        data.EventCallback(event);
    });
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS:
            {
                KeyPressedEvent event(key, 0);
                data.EventCallback(event);
                break;
            }
        case GLFW_RELEASE:
            {
                KeyReleasedEvent event(key);
                data.EventCallback(event);
                break;
            }
        case GLFW_REPEAT:
            {
                KeyPressedEvent event(key, 1);
                data.EventCallback(event);
                break;
            }
        }
    });
    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS:
            {
                MouseButtonPressedEvent event(button);
                data.EventCallback(event);
                break;
            }
        case GLFW_RELEASE:
            {
                MouseButtonReleasedEvent event(button);
                data.EventCallback(event);
                break;
            }
        }
    });
    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos) {
        WindowData& data = *(WindowData*) glfwGetWindowUserPointer(window);

        MouseMovedEvent event((float) xpos, (float) ypos);
        data.EventCallback(event);
    });
}

void OpenGLWindow::Shutdown() {
    glfwDestroyWindow(m_Window);
}

void OpenGLWindow::OnUpdate() {
    glfwPollEvents();
    glfwSwapBuffers(m_Window);
}

void OpenGLWindow::SetVSync(bool enabled) {
    if (enabled) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }

    m_Data.VSync = enabled;
}

bool OpenGLWindow::IsVSync() const {
    return m_Data.VSync;
}

} // namespace Atlas
