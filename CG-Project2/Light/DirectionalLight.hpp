#pragma once

#include "Light.hpp"

class DirectionalLight : public Light {
private:
    vec3 direction = vec3(1);

public:
    DirectionalLight();

    inline vec3 getDirection() { return this->direction; }

    inline void setDirection(vec3 dir) { this->direction = dir; }

    virtual void sendDataToShader(Shader shader, int index) override;

    ~DirectionalLight() = default;
};
