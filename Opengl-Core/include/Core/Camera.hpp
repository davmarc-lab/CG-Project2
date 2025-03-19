#pragma once

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/glm.hpp>
#include <iostream>

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
		float ratio = 1;
		// float tbSpeed = speed;
	};

	struct CameraRotation {
		float yaw = YAW;
		float pitch = PITCH;
	};

	class Camera {
	public:
		glm::mat4 getViewProjMatrix() const;

		inline glm::mat4 getProjMatrix() const { return this->m_proj; }
		inline glm::mat4 getViewMatrix() const { return this->m_view; }

		// Updates the Proj * View results BUT it assumes that the scene is 2D.
		inline void updateOrthoProjection(const float &left, const float &right, const float &bot, const float &top) {
			this->m_proj = glm::ortho(left, right, bot, top);
			this->m_view = glm::mat4(1);
			this->m_viewProj = this->m_proj * this->m_view;
		}

		inline void updatePerspProjection(const float &fov, const float &width, const float &height, const float &near, const float &far) {
			this->m_proj = glm::perspective(glm::radians(fov), width / height, near, far);
			this->m_info.ratio = width / height;
			this->updateCameraVectors();
		}

		inline void updatePerspProjection(const float &fov, const float &ratio, const float &near, const float &far) {
			this->m_proj = glm::perspective(glm::radians(fov), ratio, near, far);
			this->m_info.ratio = ratio;
			this->updateCameraVectors();
		}

		void moveCamera(const glm::vec3 &axis);

		void processMouseMovement(const float &xoffset, const float &yoffset, const bool &constrainPitch = true);

		inline void setCameraFront(glm::vec3 vec) { this->m_vectors.cameraFront = vec; }

		inline glm::vec3 getCameraFront() const { return this->m_vectors.cameraFront; }

		inline glm::vec3 getCameraUp() const { return this->m_vectors.cameraUp; }

		inline void setCameraVelocity(const float velocity) { this->m_info.speed = velocity; }

		inline float getCameraVelocity() const { return this->m_info.speed; }

		inline glm::vec3 getCameraRight() const { return this->m_vectors.cameraRight; }

		inline glm::vec3 getCameraPosition() const { return this->m_vectors.cameraPos; }

		void setCameraPosition(const glm::vec3 &position);

		inline float getMouseSensitivity() const { return this->m_info.sensitivity; }

		inline void setMouseSensitivity(float val) { this->m_info.sensitivity = val; }

		inline float getCameraZoom() const { return this->m_info.zoom; }

		inline void setCameraZoom(const float zoom) {
			this->m_info.zoom = zoom;
			this->updatePerspProjection(this->m_info.zoom, this->m_info.ratio, 0.1f, 100.f);
		}

		inline CameraRotation getCameraRotation() const { return this->m_rotation; }

		inline void setCameraYaw(const float &yaw) {
			this->m_rotation.yaw = yaw;
			this->updateCameraVectors();
		}

		inline void setCameraPitch(const float &pitch) {
			this->m_rotation.pitch = pitch;
			this->updateCameraVectors();
		}

		// inline float getTrackballSpeed() { return this->info.tb_speed; }

		// inline void setTrackballSpeed(const float speed) { this->info.tb_speed = speed; }

		inline glm::vec3 getCameraDirection() const { return this->m_vectors.cameraDirection; }

		inline void setCameraDirection(glm::vec3 dir) { this->m_vectors.cameraDirection = dir; }

		inline glm::vec3 getCameraTarget() const { return this->m_vectors.cameraTarget; }

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
} // namespace ogl
