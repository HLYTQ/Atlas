#include "Example.hpp"

#include <GLFW/glfw3.h>
#include <ImGuiFileDialog.h>
#include <glad/glad.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Atlas/Scene/Components.hpp"
#include "atlaspch.hpp"
#include <glm/gtc/type_ptr.hpp>

static std::string GetRootPath(const std::string& path) noexcept {
    return std::string(AT_CURRENT_PATH) + path;
}

static void HelpMarker(const char* desc) {
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip()) {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}

ExampleLayer::ExampleLayer() : Layer("ExampleLayer") {}

ExampleLayer::~ExampleLayer() {
    Atlas::Renderer3D::ShutDown();
}

void ExampleLayer::OnAttach() {
    // m_camera = Atlas::EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f);
    Atlas::Renderer3D::Init();
    m_FrameBuffer = Atlas::FrameBuffer::Create(1280, 720);
    m_ActiveScene = Atlas::CreateRef<Atlas::Scene>();
    // m_LightShader = Atlas::Shader::Create();

    s_ImGuiState.tex_diffuse = Atlas::Texture2D::Create(GetRootPath("assets/textures/Checkerboard.png"));

    auto modelEntity = m_ActiveScene->CreateEntity();
    modelEntity.AddComponent<Atlas::SpriteRendererComponent>(glm::vec4{0.1f, 0.1f, 0.1f, 1});
    modelEntity.AddComponent<Atlas::TransformComponent>();

    m_Mesh = Atlas::CreateRef<Atlas::Mesh>(GetRootPath("assets/Models/Cube.fbx"));
    modelEntity.AddComponent<Atlas::MeshComponent>(m_Mesh);

    glm::mat4 model = glm::mat4(1.0);
    // glm::vec3 center = meshes.m_Mesh->GetCenter();
    // glm::vec3 size = meshes.m_Mesh->GetSize();
    // float maxSize = glm::max(size.x, glm::max(size.y, size.z));
    // model = glm::scale(model, glm::vec3(6.0f / maxSize));
    // model = glm::translate(model, -center);
    modelEntity.GetComponent<Atlas::TransformComponent>() = model;
    m_ModelEntity                                         = modelEntity;

    m_Skybox.texture = Atlas::TextureCube::Create(GetRootPath("assets/textures/skybox/bluesky/"));
    m_Skybox.shader  = Atlas::Shader::Create(GetRootPath("assets/shaders/skybox.glsl"));

    // m_Skybox.textureHDR = Atlas::TextureHDR::Create(GetRootPath("assets/hdr/newport_loft.hdr"));
    // m_Skybox.shaderHDR  = Atlas::Shader::Create(GetRootPath("assets/shaders/test_pbr.glsl"));
}

void ExampleLayer::OnUpdate() {
    m_FrameBuffer->Bind();

    auto& clearColor = m_ModelEntity.GetComponent<Atlas::SpriteRendererComponent>().Color;
    Atlas::RendererCommand::SetClearColor(clearColor);
    Atlas::RendererCommand::Clear();

    if (s_ImGuiState.use_skyboxCamera) {
        m_camera.OnUpdate(m_ViewPortFoused);
        m_cameraSkybox.OnUpdate(m_ViewPortFoused);
    } else {
        m_camera.OnUpdate(m_ViewPortFoused);
    }

    Atlas::Renderer3D::SetMesh(m_ModelEntity.GetComponent<Atlas::MeshComponent>());
    Atlas::Renderer3D::BeginScene(m_ActiveScene.get(), m_camera);

    if (s_ImGuiState.use_skybox) {
        // glm::mat4 skyboxView = glm::mat4_cast(glm::conjugate(m_cameraSkybox.GetOrientation()));
        m_Skybox.view       = glm::mat4(glm::mat3(m_cameraSkybox.GetViewMatrix()));
        m_Skybox.projection = m_cameraSkybox.GetProjection();
        m_Skybox.shader->Bind();
        m_Skybox.shader->SetMat4("ViewProjection", m_Skybox.projection * m_Skybox.view);
        Atlas::Renderer3D::RenderCube(m_Skybox.shader, m_Skybox.texture);
    }

    m_ActiveScene->OnUpdate();

    // m_Skybox.shaderHDR->Bind();
    // m_Skybox.view = glm::mat4(glm::mat3(m_camera.GetViewMatrix()));
    // m_Skybox.shader->SetMat4("ViewProjection", m_camera.GetProjection() * m_Skybox.view);
    // Atlas::Renderer3D::RenderCube(m_Skybox.shaderHDR, m_Skybox.textureHDR);


    Atlas::Renderer3D::EndScene();

    m_FrameBuffer->BindIntermediate();
    m_FrameBuffer->Unbind();
}

void ExampleLayer::OnImGuiRender() {
    // Note: Switch this to true to enable dockspace
    static bool dockspaceOpen                 = true;
    static bool opt_fullscreen_persistant     = true;
    bool opt_fullscreen                       = opt_fullscreen_persistant;
    static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

    // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
    // because it would be confusing to have two docking targets within each others.
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
    if (opt_fullscreen) {
        ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
                      | ImGuiWindowFlags_NoMove;
        window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    }

    // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the
    // pass-thru hole, so we ask Begin() to not render a background.
    if (dockspace_flags) { // TODO: ImGuiDockNodeFlags_PassthruCentralNode
        window_flags |= ImGuiWindowFlags_NoBackground;
    }

    // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
    // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
    // all active windows docked into it will lose their parent and become undocked.
    // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
    // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
    ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
    ImGui::PopStyleVar();

    if (opt_fullscreen) {
        ImGui::PopStyleVar(2);
    }

    // DockSpace
    ImGuiIO& io           = ImGui::GetIO();
    ImGuiStyle& style     = ImGui::GetStyle();
    float minWinSizeX     = style.WindowMinSize.x;
    style.WindowMinSize.x = 370.0f;
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
        ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
        ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
    }

    style.WindowMinSize.x = minWinSizeX;

    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("File")) {
            if (ImGui::MenuItem("Open Model", "C-o")) {
                IGFD::FileDialogConfig config;
                config.path = "D:\\vs2022\\Programmer\\Atlas";
                ImGuiFileDialog::Instance()->OpenDialog("Open Model", "Choose Model", ".fbx,.obj,.*", config);
            }

            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View")) {
            if (ImGui::MenuItem("Editor Window", "C-e")) {
                s_ImGuiState.window_editor = true;
            }
            if (ImGui::MenuItem("Model Window", "C-m")) {
                s_ImGuiState.window_model = true;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2{0, 0});
    ImGui::Begin("ViewPort");
    {
        m_ViewPortFoused  = ImGui::IsWindowFocused();
        m_ViewPortHovered = ImGui::IsWindowHovered();

        Atlas::Application::Get().GetImGuiLayer()->BlockEvents(!m_ViewPortFoused || !m_ViewPortHovered);

        uint32_t frameID = m_FrameBuffer->GetColorAttachment();

        ImVec2 viewPortSize = ImGui::GetContentRegionAvail();
        if (m_ViewPortSize.x != viewPortSize.x || m_ViewPortSize.y != viewPortSize.y) {
            m_ViewPortSize = viewPortSize;
            m_FrameBuffer->Resize((uint32_t) m_ViewPortSize.x, (uint32_t) m_ViewPortSize.y);
            m_camera.SetViewPortSize(m_ViewPortSize.x, m_ViewPortSize.y);
            m_cameraSkybox.SetViewPortSize(m_ViewPortSize.x, m_ViewPortSize.y);
        }

        ImGui::Image(static_cast<ImTextureID>(frameID), viewPortSize, {0, 1}, {1, 0});
    }
    ImGui::End(); // end ViewPort
    ImGui::PopStyleVar();

    if (s_ImGuiState.window_editor) {
        ImGui::Begin("Editor", &s_ImGuiState.window_editor);

        if (ImGui::CollapsingHeader("Basic")) {

            auto& Color = m_ModelEntity.GetComponent<Atlas::SpriteRendererComponent>().Color;

            ImGui::ColorEdit4("Clear Color", (float*) &Color);
            // Material flag Control
            {
                auto& mesh      = m_ModelEntity.GetComponent<Atlas::MeshComponent>().m_Mesh;
                auto& materials = mesh->GetMaterials();
                // Depth Test
                ImGui::Checkbox("Depth Test", &s_ImGuiState.checkbox_depthtest);
                for (auto& material : materials) {
                    material->SetFlag(Atlas::MaterialFlag::DepthTest, s_ImGuiState.checkbox_depthtest);
                }
                ImGui::SameLine();
                HelpMarker("Choose if enable Depth Test.");

                ImGui::SameLine();

                // Draw Lines
                ImGui::Checkbox("Draw Lines", &s_ImGuiState.checkbox_Lines);
                for (auto& material : materials) {
                    material->SetFlag(Atlas::MaterialFlag::Lines, s_ImGuiState.checkbox_Lines);
                }

                ImGui::SameLine();
                HelpMarker("Choose to use GL_LINES.");
            }
        }

        // Light Update
        {
            auto& shader = m_ModelEntity.GetComponent<Atlas::MeshComponent>().m_Mesh->GetMeshShader();
            if (!m_LightState.isInit) {
                shader->SetFloat3("u_Light.Color", m_LightState.color);
                m_LightState.isInit = true;
            }
            auto distance       = m_camera.GetDistance();
            glm::vec4 pos       = glm::vec4(distance * glm::sin(m_LightState.h_trans), distance * m_LightState.v_trans,
                      distance * glm::cos(m_LightState.h_trans), 1.0);
            glm::mat4 transView = glm::mat4_cast(glm::conjugate(m_camera.GetOrientation()));
            shader->SetFloat3("u_Light.Position", glm::vec3(pos * m_camera.GetViewMatrix() * transView));
            shader->SetFloat3("u_Light.viewPos", m_camera.GetPosition());
        }

        if (ImGui::CollapsingHeader("Light")) {
            ImGui::ColorEdit3("Light Color", &m_LightState.color[0]);
            if (m_LightState.lastColor != m_LightState.color) {
                auto& shader = m_ModelEntity.GetComponent<Atlas::MeshComponent>().m_Mesh->GetMeshShader();
                shader->SetFloat3("u_Light.Color", m_LightState.color);
                m_LightState.lastColor = m_LightState.color;
            }
            if (ImGui::TreeNode("Light Postion")) {
                ImGui::SliderFloat("X", &m_LightState.h_trans, 0.0f, 6.283f);
                ImGui::SliderFloat("Y", &m_LightState.v_trans, -0.0f, 1.0f);

                auto& shader = m_ModelEntity.GetComponent<Atlas::MeshComponent>().m_Mesh->GetMeshShader();

                auto distance = m_camera.GetDistance();
                glm::vec4 pos = glm::vec4(distance * glm::sin(m_LightState.h_trans), distance * m_LightState.v_trans,
                    distance * glm::cos(m_LightState.h_trans), 1.0);
                shader->SetFloat3("u_Light.Position", glm::vec3(pos * m_camera.GetViewMatrix()));

                ImGui::TreePop();
                ImGui::Spacing();
            }
        }

        if (ImGui::CollapsingHeader("Skybox")) {
            ImGui::Checkbox("enable", &s_ImGuiState.use_skybox);
            ImGui::SameLine();
            ImGui::Checkbox("Toggle Skybox Camera", &s_ImGuiState.use_skyboxCamera);
            ImGui::Spacing();
            std::array<const char*, 4> items = {"bluesky", "Clouds", "stars", "home"};
            static int currentItem           = 0;
            const char* previewValue         = items[currentItem];
            if (ImGui::BeginCombo("Scene", previewValue)) {
                for (int i = 0; i < items.size(); ++i) {
                    bool isSelected = (currentItem == i);
                    if (ImGui::Selectable(items[i], isSelected)) {
                        if (currentItem == i) {
                            continue;
                        } else {
                            m_Skybox.texture = Atlas::TextureCube::Create(
                                GetRootPath("assets/textures/skybox/" + std::string(items[i]) + '/'));
                        }
                        currentItem = i;
                    }

                    if (isSelected) {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }


        ImGui::End(); // end Editor

        if (s_ImGuiState.window_model) {

            ImGui::Begin("Model", &s_ImGuiState.window_model);
            if (ImGui::CollapsingHeader("Materials")) {
                ImGui::SeparatorText("Albedo Texture");
                if (ImGui::ImageButton("diffuse", s_ImGuiState.tex_diffuse->GetRendererID(), ImVec2(100, 100))) {
                    IGFD::FileDialogConfig config;
                    config.path = "D:\\vs2022\\Programmer\\Atlas\\assets\\textures";
                    ImGuiFileDialog::Instance()->OpenDialog("Choose Textures", "Choose Textures", ".*", config);
                }
                ImGui::SameLine();
                ImGui::Checkbox("Use", &s_ImGuiState.use_diffuse);
                if (s_ImGuiState.use_diffuse) {
                    auto& materials = m_ModelEntity.GetComponent<Atlas::MeshComponent>().m_Mesh->GetMaterials();
                    for (auto& material : materials) {
                        material->GetTexture2D("u_AlbedoTexture") = s_ImGuiState.tex_diffuse;
                    }
                }
                // ImGui::Spacing();
                //
                // ImGui::SeparatorText("Metalness Texture");
                // if (ImGui::ImageButton("Metalness", s_ImGuiState.tex_diffuse->GetRendererID(), ImVec2(100, 100))) {
                //     IGFD::FileDialogConfig config;
                //     config.path = "D:\\vs2022\\Programmer\\Atlas\\assets\\textures";
                //     ImGuiFileDialog::Instance()->OpenDialog("Choose Textures", "Choose Textures", ".*", config);
                // }
                // ImGui::Spacing();
                //
                // ImGui::SeparatorText("Roughness Texture");
                // if (ImGui::ImageButton("Roughness", s_ImGuiState.tex_diffuse->GetRendererID(), ImVec2(100, 100))) {
                //     IGFD::FileDialogConfig config;
                //     config.path = "D:\\vs2022\\Programmer\\Atlas\\assets\\textures";
                //     ImGuiFileDialog::Instance()->OpenDialog("Choose Textures", "Choose Textures", ".*", config);
                // }

            }
            ImGui::End(); // end Model
        }
    }
    ImGui::End();


    if (ImGuiFileDialog::Instance()->Display("Open Model")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath     = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
            m_Mesh                                                    = Atlas::CreateRef<Atlas::Mesh>(filePathName);
            m_ModelEntity.GetComponent<Atlas::MeshComponent>().m_Mesh = m_Mesh;

            auto& meshes    = m_ModelEntity.GetComponent<Atlas::MeshComponent>();
            glm::mat4 model = glm::mat4(1.0);
            m_ModelEntity.GetComponent<Atlas::TransformComponent>() = model;
            m_LightState.isInit                                     = false;
        }

        ImGuiFileDialog::Instance()->Close();
    }
    if (ImGuiFileDialog::Instance()->Display("Choose Textures")) {
        if (ImGuiFileDialog::Instance()->IsOk()) { // action if OK
            std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
            std::string filePath     = ImGuiFileDialog::Instance()->GetCurrentPath();
            // action
            s_ImGuiState.tex_diffuse = Atlas::Texture2D::Create(filePathName);
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void ExampleLayer::OnEvent(Atlas::Event& e) {
    m_camera.OnEvent(e);
    m_cameraSkybox.OnEvent(e);
}
