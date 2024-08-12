#include "Camera.hpp"

void Camera::moveCamera(vec3 position) {
    this->vectors.cameraPos = position;
    this->updateCameraVectors();
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= this->info.sensitivity;
    yoffset *= this->info.sensitivity;

    this->rotation.yaw += xoffset;
    this->rotation.pitch += yoffset;

    if (constrainPitch) {
        if (this->rotation.pitch > 89)
            this->rotation.pitch = 89;
        if (this->rotation.pitch < -89)
            this->rotation.pitch = -89;
    }

    this->updateCameraVectors();
}

void Camera::revertCamera() {
    this->rotation.pitch = -this->rotation.pitch;
    this->rotation.yaw = -this->rotation.yaw;
    this->updateCameraVectors();
}

void Camera::setFromRotation(vec3 rot) {
    this->rotation.pitch = -rot.x;
    this->rotation.yaw = -rot.y + 90;
    this->updateCameraVectors();
}
