#include "Application.hpp"
#include "Input.hpp"
#include "Log.hpp"

namespace Atlas {

#define BIND_CALLBACK_FUNC(x) std::bind(&Application::x, this, std::placeholders::_1)

Application* Application::s_Ptr = nullptr;

Application::Application() {
    s_Ptr    = this;
    m_Window = Scope<Window>(Window::Create());
    m_Window->SetEventCallback(BIND_CALLBACK_FUNC(OnEvent));
    m_ImGuiLayer = Scope<ImGuiLayer>();
}

Application::~Application() {}

void Application::OnEvent(Event& e) {
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_CALLBACK_FUNC(OnWindowClose));
    AT_CORE_INFO(e.ToString());

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

void Application::Run() {
    while (m_Running) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        for (Layer* layer : m_LayerStack) {
            layer->OnUpdate();
        }
        m_ImGuiLayer->Begin();
        for (Layer* layer : m_LayerStack) {
            layer->OnImGuiRender();
        }
        m_ImGuiLayer->End();

        auto [x, y] = Input::GetMousePos();
        AT_CORE_TRACE("{0}, {1}", x, y);

        m_Window->OnUpdate();
    }
}

} // namespace Atlas
