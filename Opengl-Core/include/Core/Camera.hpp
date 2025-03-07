#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>

namespace ogl {
	inline const float YAW = -90.f;
	inline const float PITCH = 0.f;
	inline const float SPEED = 0.2f;
	inline const float SENSITIVITY = 0.02f;
	inline const float ZOOM = 45.f;
	inline const glm::vec3 POSITION = glm::vec3(0, 0, 3);
	// float tbSpeed = 20.f;

	struct CameraVectors {
		glm::vec3 cameraPos = POSITION;
		glm::vec3 cameraFront = glm::vec3(0, 0, -1);
		glm::vec3 cameraUp = glm::vec3(0, 1, 0);
		glm::vec3 cameraRight = glm::vec3(1, 0, 0);
		glm::vec3 cameraDirection = glm::vec3(0);
		glm::vec3 cameraTarget = glm::vec3(0);
	};

	struct CameraInfo {
		float speed = SPEED;
		float sensitivity = SENSITIVITY;
		float zoom = ZOOM;
		// float tbSpeed = speed;
	};

	struct CameraRotation {
		float yaw = YAW;
		float pitch = PITCH;
	};

	class Camera {
	public:
		glm::mat4 getViewProjMatrix();

		inline glm::mat4 getProjMatrix() { return this->m_proj; }
		inline glm::mat4 getViewMatrix() { return this->m_view; }

		// Updates the Proj * View results BUT it assumes that the scene is 2D.
		inline void updateOrthoProjection(const float &left, const float &right, const float &bot, const float &top) {
			this->m_proj = glm::ortho(left, right, bot, top);
			this->m_view = glm::mat4(1);
			this->m_viewProj = this->m_proj * this->m_view;
		}

		inline void updatePerspProjection(const float &fov, const float &width, const float &height, const float &near, const float &far) {
			this->m_proj = glm::perspective(fov, width / height, near, far);
			this->updateCameraVectors();
		}

		void moveCamera(glm::vec3 position);

		void processMouseMovement(const float &xoffset, const float &yoffset, const bool &constrainPitch = true);

		inline void setCameraFront(glm::vec3 vec) { this->m_vectors.cameraFront = vec; }

		inline glm::vec3 getCameraFront() { return this->m_vectors.cameraFront; }

		inline glm::vec3 getCameraUp() { return this->m_vectors.cameraUp; }

		inline void setCameraVelocity(const float velocity) { this->m_info.speed = velocity; }

		inline float getCameraVelocity() { return this->m_info.speed; }

		inline glm::vec3 getCameraRight() { return this->m_vectors.cameraRight; }

		inline glm::vec3 getCameraPosition() { return this->m_vectors.cameraPos; }

		inline float getMouseSensitivity() { return this->m_info.sensitivity; }

		inline void setMouseSensitivity(float val) { this->m_info.sensitivity = val; }

		inline float getCameraZoom() { return this->m_info.zoom; }

		inline void setCameraZoom(const float zoom) { this->m_info.zoom = zoom; }

		// inline float getTrackballSpeed() { return this->info.tb_speed; }

		// inline void setTrackballSpeed(const float speed) { this->info.tb_speed = speed; }

		inline glm::vec3 getCameraDirection() { return this->m_vectors.cameraDirection; }

		inline void setCameraDirection(glm::vec3 dir) { this->m_vectors.cameraDirection = dir; }

		inline glm::vec3 getCameraTarget() { return this->m_vectors.cameraTarget; }

		inline void setCameraTarget(glm::vec3 target) { this->m_vectors.cameraTarget = target; }

		Camera() { this->updateCameraVectors(); }

		~Camera() = default;

	private:
		glm::vec3 m_worldUp = glm::vec3(0, 1, 0);
		CameraVectors m_vectors{};
		CameraInfo m_info{};
		CameraRotation m_rotation{};

		glm::mat4 m_view = glm::mat4(0);
		glm::mat4 m_proj = glm::mat4(0);
		glm::mat4 m_viewProj = glm::mat4(0);

		void updateCameraVectors();
	};

	inline Camera camera{};
} // namespace ogl
