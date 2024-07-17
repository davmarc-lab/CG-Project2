#pragma once

#include "Light.hpp"

#include "../Menu/IGDebug.hpp"

class DirectionalLight : public Light {
private:
    vec3 direction = vec3(1);

public:
    DirectionalLight();

    inline vec3 getDirection() { return this->direction; }

    inline void setDirection(vec3 dir) { this->direction = dir; }

    inline virtual void initCaster() override {
        IGDebug::instance()->addLog(LogType::MISSING_CASTER, "No Caster Needed for Directional Light");
    }

    virtual void sendDataToShader(Shader shader, int index) override;

    virtual void drawCaster() override {}

    ~DirectionalLight() = default;
};
