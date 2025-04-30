#pragma once
#include "Atlas.hpp"
#include "Atlas/Camera/EditorCamera.hpp"
// #include "Atlas/Camera/Camera3D.hpp"

#include <imgui.h>

class ExampleLayer : public Atlas::Layer {
public:
    ExampleLayer();
    ~ExampleLayer();


    void OnAttach() override;

    void OnUpdate() override;
    
    void OnImGuiRender() override;

    void OnEvent(Atlas::Event& e) override;

private:

    Atlas::EditorCamera m_camera;
    Atlas::EditorCamera m_cameraSkybox;

    Atlas::Ref<Atlas::FrameBuffer> m_FrameBuffer;

    Atlas::Ref<Atlas::Scene> m_ActiveScene;
    Atlas::Entity m_ModelEntity;

    Atlas::Ref<Atlas::Shader> m_LightShader;
    Atlas::Ref<Atlas::Mesh> m_Mesh;

    struct SkyboxUnit {
        Atlas::Ref<Atlas::TextureCube> texture;
        Atlas::Ref<Atlas::Shader> shader;
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);

        Atlas::Ref<Atlas::TextureHDR> textureHDR;
        Atlas::Ref<Atlas::Shader> shaderHDR;
    } m_Skybox;

    struct Light {
        bool isInit = false;
        
        glm::vec3 color = glm::vec3(1.0f);
        glm::vec3 lastColor = glm::vec3(1.0f);

        float h_trans = 1.0f;
        float v_trans = 0.0f;

    } m_LightState;

    struct ImguiState {
        // Windows
        bool window_editor = true;
        bool window_model = true;

        // Check box
        bool checkbox_Lines = false;
        bool checkbox_depthtest = true;

        // textures
        bool use_diffuse = false;
        Atlas::Ref<Atlas::Texture2D> tex_diffuse;

        // Skyhbox Toggle
        bool use_skybox = false;
        bool use_skyboxCamera = false;

    } s_ImGuiState;

    ImVec2 m_ViewPortSize;
    bool m_ViewPortFoused;
    bool m_ViewPortHovered;

};
