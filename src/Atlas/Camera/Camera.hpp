#pragma once

#include <glm/glm.hpp>

namespace Atlas {

class Camera {
public:
	Camera() = default;
	Camera(const glm::mat4& projection) : m_Projection(projection) {}

	virtual ~Camera() = default;

	const glm::mat4& GetProjection() const noexcept {
		return m_Projection;
	}
protected:
	glm::mat4 m_Projection;
};

} // namespace Atlas
