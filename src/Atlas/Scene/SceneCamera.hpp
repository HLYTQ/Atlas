#pragma once

#include "Atlas/Camera/Camera.hpp"

namespace Atlas {

class SceneCamera : public Camera {
public:
    SceneCamera();
    virtual ~SceneCamera() override = default;

    void SetPerspective(float verticalFOV, float nearClip, float farClip);

    void SetViewportSize(uint32_t width, uint32_t height);

    float GetPerspectiveVerticalFOV() const {
        return m_PerspectiveFOV;
    }
    void SetPerspectiveVerticalFOV(float verticalFOV) {
        m_PerspectiveFOV = verticalFOV;
        RecalculateProjection();
    }
    float GetPerspectiveNearClip() const {
        return m_PerspectiveNear;
    }
    void SetPerspectiveNearClip(float clip) {
        m_PerspectiveNear = clip;
        RecalculateProjection();
    }
    float GetPerspectiveFarClip() const {
        return m_PerspectiveFar;
    }
    void SetPerspectiveFarClip(float clip) {
        m_PerspectiveFar = clip;
        RecalculateProjection();
    }

private:
    void RecalculateProjection();

public:
    float m_PerspectiveFOV  = glm::radians(45.0f);
    float m_PerspectiveNear = 0.01f, m_PerspectiveFar = 1000.0f;

    float m_AspectRatio = 0.0f;
};

} // namespace Atlas
