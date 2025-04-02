#include "Atlas.hpp"
#include "Atlas/Layer.hpp"
#include "Atlas/ImGui/ImGuiLayer.hpp"

class ExampleLayer : public Atlas::Layer {
public:
    ExampleLayer() : Layer("ExampleLayer") {}

    void OnUpdate() override {
        AT_INFO("ExampleLayer Update!");
    }
    void OnEvent(Atlas::Event& e) override {
        AT_TRACE(e.ToString());
    }

};

int main(void) {
    Atlas::Application app;
    Atlas::Log::init();
    AT_CORE_WARN("Initlized Log");
    AT_INFO("Hello");

    auto app = new Atlas::Application();
    app->PushLayer(new Atlas::ImGuiLayer());
    app->Run();
    delete app;
}
