#pragma once

#include "atlaspch.hpp"
#include "Atlas/Events/ApplicationEvent.hpp"
#include "Atlas/Events/Event.hpp"
#include "Atlas/ImGui/ImGuiLayer.hpp"
#include "Atlas/LayerStack.hpp"
#include "Atlas/Renderer/Shader.hpp"
#include "Atlas/Renderer/VertexArray.hpp"
#include "OpenGL/OpenGLWindow.hpp"

namespace Atlas {

class Application {
public:
    Application();
    virtual ~Application();

    void Run();
    void OnEvent(Event& e);

    constexpr bool OnWindowClose(WindowCloseEvent& e) noexcept;
    bool OnWindowResize(WindowResizeEvent& e) noexcept;

    void PushLayer(Layer* layer) {
        m_LayerStack.PushLayer(layer);
    }
    void PushOverLayer(Layer* layer) {
        m_LayerStack.PushOverLay(layer);
    }

    static Application& Get() noexcept {
        return (*s_Ptr);
    }

    Window& GetWindow() noexcept {
        return (*m_Window);
    }

    ImGuiLayer* GetImGuiLayer() noexcept {
        return m_ImGuiLayer;
    }

private:
    Scope<Window> m_Window;
    ImGuiLayer* m_ImGuiLayer;

    bool m_Running = true;
    LayerStack m_LayerStack;

    static Application* s_Ptr;
};

} // namespace Atlas
