#include "Camera.hpp"

void Camera::moveCamera(vec3 position)
{
    this->cameraPos = position;
    this->updateCameraVectors();
}

void Camera::processMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
    xoffset *= this->mouseSensitivity;
    yoffset *= this->mouseSensitivity;

    this->yaw += xoffset;
    this->pitch += yoffset;

    if (constrainPitch)
    {
        if (this->pitch > 89)
            this->pitch = 89;
        if (this->pitch < -89)
            this->pitch = -89;
    }

    this->updateCameraVectors();
}
