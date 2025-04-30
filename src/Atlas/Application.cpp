#include "Application.hpp"

#include "Input.hpp"
#include "atlaspch.hpp"
#include "Atlas/Renderer/Renderer.hpp"
#include "Atlas/Renderer/RendererCommand.hpp"

namespace Atlas {

Application* Application::s_Ptr = nullptr;

Application::Application() {
    s_Ptr    = this;
    m_Window = Window::Create();
    m_Window->SetEventCallback(BIND_CALLBACK_FUNC(Application::OnEvent));
    m_ImGuiLayer = new ImGuiLayer();
    PushOverLayer(m_ImGuiLayer);
}

Application::~Application() { }

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_CALLBACK_FUNC(Application::OnWindowClose));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_CALLBACK_FUNC(Application::OnWindowResize));
    // AT_CORE_INFO(e.ToString());
    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
        (*--it)->OnEvent(e);
        if (e.Handled) {
            break;
        }
    }
}

constexpr bool Application::OnWindowClose(WindowCloseEvent& e) noexcept {
    m_Running = false;
    return true;
}

bool Application::OnWindowResize(WindowResizeEvent& e) noexcept {
    RendererCommand::SetViewPort(GetWindow().GetWidth(), GetWindow().GetHeight());
    return true;
}

void Application::Run() {
    while (m_Running) {

        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate();
        }

        m_ImGuiLayer->Begin();
        {
            for (Layer* layer : m_LayerStack) {
                layer->OnImGuiRender();
            }
        }
        m_ImGuiLayer->End();

        // auto pos = Input::GetMousePosition();
        // AT_CORE_TRACE("{0}, {1}", pos.x, pos.y);

        m_Window->OnUpdate();
    }
}

} // namespace Atlas
