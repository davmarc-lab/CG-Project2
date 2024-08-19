#pragma once
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <glm/trigonometric.hpp>
#include <glm/vec3.hpp>

using namespace glm;

// This namespace defines some camera default parameters used for initialization and reset button.
namespace cmr {
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 1.0f;
const float SENSITIVITY = 0.02f;
const float ZOOM = 45.0f;
const vec3 POSITION = vec3(0, 0, 3);
const float TB_SPEED = 20.0f;
} // namespace cmr

// Store all the camera vectors.
struct CameraVectors {
    vec3 cameraPos = cmr::POSITION;
    vec3 cameraFront = vec3(0, 0, -1);
    vec3 cameraUp = vec3(0, 1, 0);
    vec3 cameraRight = vec3(1, 0, 0);
    vec3 direction = vec3(0);
    vec3 target = vec3(0);
};

// Store all the camera properties.
struct CameraInfo {
    float speed = cmr::SPEED;
    float sensitivity = cmr::SENSITIVITY;
    float zoom = cmr::ZOOM;
    float tb_speed = cmr::TB_SPEED;
};

// Used to store the camera rotation data.
struct CameraRotation {
    float pitch = cmr::PITCH; // rotation around x axis
    float yaw = cmr::YAW;     // rotation around y axis
};

/*
 * This class is used to create a camera pointed in a direction.
 */
class Camera {
  private:
    // Defines the world y axis.
    vec3 worldUp = vec3(0, 1, 0);
    CameraVectors vectors;
    CameraInfo info;
    CameraRotation rotation;

    // Update the camera vector after every movement.
    void updateCameraVectors() {
        this->vectors.cameraFront =
            normalize(vec3(cos(radians(this->rotation.yaw)) * cos(radians(this->rotation.pitch)), -sin(radians(this->rotation.pitch)),
                           sin(radians(this->rotation.yaw)) * cos(radians(this->rotation.pitch))));
        this->vectors.cameraRight = normalize(cross(this->vectors.cameraFront, this->worldUp));
        this->vectors.cameraUp = normalize(cross(this->vectors.cameraRight, this->vectors.cameraFront));
    }

  public:
    Camera() { this->updateCameraVectors(); }

    // Retrieves the View matrix.
    inline mat4 getViewMatrix() {
        return glm::lookAt(this->vectors.cameraPos, this->vectors.cameraPos + this->vectors.cameraFront, this->vectors.cameraUp);
    }

    inline mat4 getThirdViewMatrix(vec3 point) {
        return glm::lookAt(this->vectors.cameraPos, point, this->vectors.cameraUp);
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

    inline float getCameraZoom() { return this->info.zoom; }

    inline void setCameraZoom(const float zoom) { this->info.zoom = zoom; }

    inline float getTrackballSpeed() { return this->info.tb_speed; }

    inline void setTrackballSpeed(const float speed) { this->info.tb_speed = speed; }

    inline vec3 getCameraDirection() { return this->vectors.direction; }

    inline void setCameraDirection(vec3 dir) { this->vectors.direction = dir; }

    inline vec3 getCameraTarget() { return this->vectors.target; }

    inline void setCameraTarget(vec3 target) { this->vectors.target = target; }

    void setFromRotation(vec3 rot);

    // This method changes the camera position and updates the camera vectors.
    void moveCamera(vec3 position);

    // This method can be used to process the mouse input to move the camera in the space, it updates the vectors as well.
    void processMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);

    void revertCamera();

    ~Camera() = default;
};
