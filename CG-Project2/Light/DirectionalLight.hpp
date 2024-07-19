#pragma once

#include "Light.hpp"

#include "../Menu/Logger/LogManager.hpp"

class DirectionalLight : public Light {
private:
    vec3 direction = vec3(1, 0, 0);

public:
    DirectionalLight();

    inline vec3 getDirection() { return this->direction; }

    inline void setDirection(vec3 dir) { this->direction = dir; }

    inline virtual void initCaster() override {
        LogManager::instance()->addLog(logs::MISSING_CASTER, glfwGetTime(), "No Caster Needed for Directional Light");
    }

    virtual void sendDataToShader(Shader shader, int index) override;

    virtual void drawCaster() override {}

    ~DirectionalLight() = default;
};
