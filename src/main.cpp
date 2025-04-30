#include "Atlas.hpp"
#include "Example.hpp"

namespace Atlas {

    class Atlasnut : public Application {
    public:
        Atlasnut() : Application() {
            PushLayer(new ExampleLayer());
        }

        ~Atlasnut() override = default;
    };
}

int main() {
    Atlas::Log::init();
    AT_CORE_WARN("Initlized Log");
    AT_INFO("Hello");

    auto app = new Atlas::Atlasnut();
    app->Run();
    delete app;
}
