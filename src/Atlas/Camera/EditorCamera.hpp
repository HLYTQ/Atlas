#pragma once

#include "Atlas/Events/Event.hpp"
#include "Atlas/Events/MouseEvent.hpp"
#include "Camera.hpp"

namespace Atlas {

class EditorCamera : public Camera {
public:
    EditorCamera() = default;
    EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

    void OnUpdate(bool is_update);
    void OnEvent(Event& e);
    void SetViewportSize(float width, float height);

    glm::mat4 GetViewProjection() const noexcept {
        return m_Projection * m_ViewMatrix;
    }

    const glm::vec3& GetPosition() const noexcept {
        return m_Position;
    }

    float GetDistance() const {
        return m_Distance;
    }
    void SetDistance(float distance) {
        m_Distance = distance;
    }

    void SetViewPortSize(float width, float height) {
        m_ViewportWidth  = width;
        m_ViewportHeight = height;
        UpdateProjection();
    }

    const float GetViewPortWidth() const noexcept {
        return m_ViewportWidth;
    }

    const float GetViewPortHeight() const noexcept {
        return m_ViewportHeight;
    }

    const glm::mat4& GetViewMatrix() const {
        return m_ViewMatrix;
    }

    const float GetAspectRatio() const noexcept {
        return m_AspectRatio;
    }

    glm::vec3 CalculatePosition() const;

    glm::quat GetOrientation() const;
    glm::vec3 GetUpDirection() const;
    glm::vec3 GetRightDirection() const;
    glm::vec3 GetForwardDirection() const;

private:
    void UpdateProjection();
    void UpdateView();

    std::pair<float, float> PanSpeed() const;

    bool OnMouseScroll(MouseScrolledEvent& e);
    void MousePan(const glm::vec2& delta);
    void MouseRotate(const glm::vec2& delta);
    void MouseZoom(float delta);

    const float RotationSpeed() const noexcept {
        return 0.8f;
    }
    float ZoomSpeed() const;

private:
    float m_FOV            = 45.0f;
    float m_AspectRatio    = 1.778f;
    float m_NearClip       = 0.1f;
    float m_FarClip        = 1000.0f;
    float m_Distance       = 10.0f;
    float m_Pitch          = 0.0f;
    float m_Yaw            = 0.0f;
    float m_ViewportWidth  = 1280;
    float m_ViewportHeight = 720;

    glm::mat4 m_ViewMatrix   = glm::mat4(1.0f);
    glm::vec3 m_Position     = glm::vec3(0.0f);
    glm::vec3 m_FocalPoint   = glm::vec3(0.0f);
    glm::vec2 m_InitMousePos = glm::vec2(0.0f);
};
} // namespace Atlas
