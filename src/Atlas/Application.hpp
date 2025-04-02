#pragma once

#include "Atlas/Events/ApplicationEvent.hpp"
#include "Atlas/Events/Event.hpp"
#include "Atlas/ImGui/ImGuiLayer.hpp"
#include "Atlas/LayerStack.hpp"
#include "Core.hpp"
#include "OpenGL/OpenGLWindow.hpp"

namespace Atlas {

class Application {
public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event& e);
    constexpr bool OnWindowClose(WindowCloseEvent& e) noexcept;

    void PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }
    void PushOverLayer(Layer* layer) {
        m_LayerStack.PushOverLay(layer);
    }

    static inline Application& Get() noexcept {
        return (*s_Ptr);
    }
    inline Window& GetWindow() noexcept {
        return (*m_Window);
    }

private:
    Scope<Window> m_Window;
    Scope<ImGuiLayer> m_ImGuiLayer;
    bool m_Running = true;
    LayerStack m_LayerStack;
    static Application* s_Ptr;
};

} // namespace Atlas
