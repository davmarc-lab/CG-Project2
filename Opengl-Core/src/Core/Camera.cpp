#include "../../include/Core/Camera.hpp"
#include <glm/ext/matrix_transform.hpp>
#include "../../include/Core/Event.hpp"

namespace ogl {
	void Camera::updateCameraVectors() {
		this->m_vectors.cameraFront = normalize(glm::vec3(cos(glm::radians(this->m_rotation.yaw)) * cos(glm::radians(this->m_rotation.pitch)),
														  -sin(glm::radians(this->m_rotation.pitch)),
														  sin(glm::radians(this->m_rotation.yaw) * cos(glm::radians(this->m_rotation.pitch)))));
		this->m_vectors.cameraRight = normalize(cross(this->m_vectors.cameraFront, this->m_worldUp));
		this->m_vectors.cameraUp = normalize(cross(this->m_vectors.cameraRight, this->m_vectors.cameraFront));
		this->m_view = glm::lookAt(this->m_vectors.cameraPos, this->m_vectors.cameraPos + this->m_vectors.cameraFront, this->m_vectors.cameraUp);
		this->m_viewProj = this->m_proj * this->m_view;
		EventManager::instance()->post(event::shader::SHADER_PROJECTION_CHANGED);
	}

	glm::mat4 Camera::getViewProjMatrix() {
		return this->m_viewProj;
	}

	void Camera::moveCamera(glm::vec3 pos) {
		this->m_vectors.cameraPos += (pos * this->m_info.speed);
		this->updateCameraVectors();
	}

	void Camera::processMouseMovement(const float &xoffset, const float &yoffset, const bool &constrainPitch) {
		this->m_rotation.yaw += (xoffset * this->m_info.sensitivity);
		this->m_rotation.pitch += (yoffset * this->m_info.sensitivity);

		if (constrainPitch) {
			if (this->m_rotation.pitch > 89)
				this->m_rotation.pitch = 89;
			if (this->m_rotation.pitch < -89)
				this->m_rotation.pitch = -89;
		}

		this->updateCameraVectors();
	}
} // namespace ogl
