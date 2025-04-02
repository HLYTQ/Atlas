#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Atlas/Window.hpp"

namespace Atlas {

class OpenGLWindow : public Window {
public:
    OpenGLWindow(const WindowProps& props);
    virtual ~OpenGLWindow();

    void OnUpdate() override;

    inline unsigned int GetWidth() const override {
        return m_Data.Width;
    }
    inline unsigned int GetHeight() const override {
        return m_Data.Height;
    }

    inline void SetEventCallback(const EventCallbackFn& callback) override {
        m_Data.EventCallback = callback;
    }
    void SetVSync(bool enabled) override;
    bool IsVSync() const override;

    inline void* GetNativeWindow() const override {
        return m_Window;
    }

private:
    virtual void Init(const WindowProps& props);
    virtual void Shutdown();

private:
    GLFWwindow* m_Window;

    struct WindowData {
        std::string Title;
        unsigned int Width;
        unsigned int Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

    WindowData m_Data;
};

} // namespace Atlas
