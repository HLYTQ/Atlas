#pragma once

#include <cstdint>

#include "Core.hpp"
#include "Atlas/Events/Event.hpp"
#include "OpenGL/OpenGLWindow.hpp"

namespace Atlas {

struct WindowProps {
    WindowProps(const std::string& title = "Atlas Engine", uint32_t width = 1280, uint32_t height = 720)
        : Title(title), Width(width), Height(height) {}
public:
    std::string Title;
    uint32_t Width;
    uint32_t Height;
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;

public:
    virtual ~Window() = default;

    static Scope<Window> Create(const WindowProps& props = WindowProps());
    
    virtual void OnUpdate() = 0;

    virtual uint32_t GetWidth() const  = 0;
    virtual uint32_t GetHeight() const = 0;

    // Window attributes
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void SetVSync(bool enabled)                            = 0;
    virtual bool IsVSync() const                                   = 0;

    virtual void* GetNativeWindow() const = 0;
};

} // namespace Atlas
