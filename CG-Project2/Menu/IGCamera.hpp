#pragma once

#include "IGMenu.hpp"

#include "../Lib.hpp"

class IGCamera : public IGMenu {
public:
    IGCamera() {}

    inline void render() {
        ImGui::Begin("Camera");

        // Camera FOV
        auto zoom = camera.getCameraZoom();
        if (ImGui::DragFloat("Fov##1", &zoom, 0.5f, 20.f, 120.f)) {
            camera.setCameraZoom(zoom);
            updatePerspective(zoom);
        }
        if (ImGui::Button("Reset##1")) {
            camera.setCameraZoom(cmr::ZOOM);
            updatePerspective(cmr::ZOOM);
        }

        // Camera position
        auto pos = camera.getCameraPosition();
        if (ImGui::DragFloat3("Position##1", &pos.x, 0.005f)) {
            camera.moveCamera(pos);
        }
        if (ImGui::Button("Reset##2")) {
            camera.moveCamera(cmr::POSITION);
        }
        // Camera Rotation ?

        // Camera velocity
        auto velocity = camera.getCameraVelocity();
        if (ImGui::DragFloat("Velocity##1", &velocity, 0.005, 0.0f)) {
            if (velocity > 0.f) {
                camera.setCameraVelocity(velocity);
            }
        }
        if (ImGui::Button("Reset##3")) {
            camera.setCameraVelocity(cmr::SPEED);
        }

        // Camera sensitivity
        auto sens = camera.getMouseSensitivity();
        if (ImGui::DragFloat("Sensitivity##1", &sens, 0.005, 0.0f)) {
            if (sens > 0.f) {
                camera.setMouseSensitivity(sens);
            }
        }
        if (ImGui::Button("Reset##4")) {
            camera.setMouseSensitivity(cmr::SENSITIVITY);
        }

        // Trackball speed
        auto speed = camera.getTrackballSpeed();
        if (ImGui::DragFloat("Trackball speed##1", &speed, 0.5, 0.0f)) {
            if (speed > 0.f) {
                camera.setTrackballSpeed(speed);
            }
        }
        if (ImGui::Button("Reset##5")) {
            camera.setTrackballSpeed(cmr::TB_SPEED);
        }

        ImGui::End();
    }

    ~IGCamera() = default;
};
