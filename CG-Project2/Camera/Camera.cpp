#include "Camera.hpp"

void Camera::moveCamera(vec3 position)
{
    this->vectors.cameraPos = position;
    this->updateCameraVectors();
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= this->info.sensitivity;
    yoffset *= this->info.sensitivity;

    this->rotation.yaw += xoffset;
    this->rotation.pitch += yoffset;

    if (constrainPitch)
    {
        if (this->rotation.pitch > 89)
            this->rotation.pitch = 89;
        if (this->rotation.pitch < -89)
            this->rotation.pitch = -89;
    }

    this->updateCameraVectors();
}
