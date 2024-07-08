#pragma once
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

using namespace glm;

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

struct CameraVectors {
    vec3 cameraPos = vec3(0);
    vec3 cameraFront = vec3(0, 0, -1);
    vec3 cameraUp = vec3(0, 1, 0);
    vec3 cameraRight = vec3(1, 0, 0);
};

struct CameraInfo {
    float speed = SPEED;
    float sensitivity = SENSITIVITY;
    float zoom = ZOOM;
};

struct CameraRotation {
    float pitch = PITCH;    // rotation around x axis
    float yaw = YAW;      // rotation around y axis
};

class Camera {
private:
    vec3 worldUp = vec3(0, 1, 0);
    CameraVectors vectors;
    CameraInfo info;
    CameraRotation rotation;

    void updateCameraVectors() {
        vec3 front;
        front.x = cos(radians(this->rotation.yaw)) * cos(radians(this->rotation.pitch));
        front.y = -sin(radians(this->rotation.pitch));
        front.z = sin(radians(this->rotation.yaw)) * cos(radians(this->rotation.pitch));

        this->vectors.cameraFront = normalize(front);
        this->vectors.cameraRight = normalize(cross(this->vectors.cameraFront, this->worldUp));
        this->vectors.cameraUp = normalize(cross(this->vectors.cameraRight, this->vectors.cameraFront));
    }

public:
    Camera() { this->updateCameraVectors(); }

    inline mat4 getViewMatrix() {
        return glm::lookAt(this->vectors.cameraPos, this->vectors.cameraPos + this->vectors.cameraFront, this->vectors.cameraUp);
    }

    inline void setCameraFront(vec3 vec) { this->vectors.cameraFront = vec; }

    inline vec3 getCameraFront() { return this->vectors.cameraFront; }

    inline vec3 getCameraUp() { return this->vectors.cameraUp; }

    inline void setCameraVelocity(const float velocity) { this->info.speed = velocity; }

    inline float getCameraVelocity() { return this->info.speed; }

    inline vec3 getCameraRight() { return this->vectors.cameraRight; }

    inline vec3 getCameraPosition() { return this->vectors.cameraPos; }

    inline float getMouseSensitivity() { return this->info.sensitivity; }

    inline void setMouseSensitivity(float val) { this->info.sensitivity = val; }

    void moveCamera(vec3 position);

    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    void rotateCamera(vec3 rotationAxis, float rotationValue);

    ~Camera() = default;
};
