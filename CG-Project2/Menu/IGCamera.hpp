#pragma once

#include "IGMenu.hpp"

#include "../Lib.hpp"
#include "../LibCustomGui.hpp"

class IGCamera : public IGMenu {
public:
    IGCamera() {}

    inline void render() {
        ImGui::Begin("Camera");

        // Camera FOV
        auto zoom = camera.getCameraZoom();
        if (ImGui::DragFloat("Zoom##1", &zoom, 0.5f)) {
            if (zoom >= 20.f) {
                camera.setCameraZoom(zoom);
                updatePerspective(zoom);
            }
        }
        if (ImGui::Button("Reset##1")) {
            camera.setCameraZoom(cmr::ZOOM);
            updatePerspective(zoom);
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

        ImGui::End();
    }

    ~IGCamera() = default;
};
