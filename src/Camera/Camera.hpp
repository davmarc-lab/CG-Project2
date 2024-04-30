#pragma once

#include "../Lib.hpp"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

class Camera
{
    private:
        vec3 cameraPos = vec3(0);
        vec3 cameraFront = vec3(0, 0, -1);
        vec3 cameraUp = vec3(0, 1, 0);
        vec3 cameraRight = vec3(1, 0, 0);
        vec3 worldUp = vec3(0, 1, 0);
        vec3 rotation;
        float cameraSpeed = SPEED;
        float mouseSensitivity = SENSITIVITY;
        float cameraZoom = ZOOM;

        float yaw = YAW;
        float pitch = PITCH;

        void updateCameraVectors()
        {
            vec3 front;
            front.x = cos(radians(this->yaw)) * cos(radians(this->pitch));
            front.y = -sin(radians(this->pitch));
            front.z = sin(radians(this->yaw)) * cos(radians(this->pitch));

            this->cameraFront = normalize(front);
            this->cameraRight = normalize(cross(this->cameraFront, this->worldUp));
            this->cameraUp = normalize(cross(this->cameraRight, this->cameraFront));
        }

    public:
        Camera() 
        {
            this->updateCameraVectors();
        }

        mat4 getViewMatrix() { return glm::lookAt(this->cameraPos,
                this->cameraPos + this->cameraFront,
                this->cameraUp); }

        void setCameraFront(vec3 vec) { this->cameraFront = vec; }

        vec3 getCameraFront() { return this->cameraFront; }

        vec3 getCameraUp() { return this->cameraUp; }

        void setCameraVelocity(const float velocity) { this->cameraSpeed = velocity; }

        float getCameraVelocity() { return this->cameraSpeed; }

        vec3 getCameraRight() { return this->cameraRight; }

        vec3 getCameraPosition() { return this->cameraPos; }

        float getMouseSensitivity() { return this->mouseSensitivity; }

        void setMouseSensitivity(float val) { this->mouseSensitivity = val; }

        void moveCamera(vec3 position);

        void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

        void rotateCamera(vec3 rotationAxis, float rotationValue);
};
