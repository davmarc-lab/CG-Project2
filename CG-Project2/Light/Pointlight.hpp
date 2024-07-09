#pragma once

#include "Light.hpp"

class PointLight : public Light {
private:
    vec3 position = vec3(0);
    PointLightInfo info;

public:
    PointLight() {}

    PointLight(vec3 position, float constant, float linear, float quadratic);

    inline vec3 getPosition() { return this->position; }

    inline void setPosition(vec3 pos) { this->position = pos; }
    
    inline float getConstant() { return this->info.constant; }
    
    inline void setConstant(const float val) { this->info.constant = val; }

    inline float getLinear() { return this->info.linear; }

    inline void setLinear(const float val) { this->info.linear = val; }

    inline float getQuadratic() { return this->info.quadratic; }

    inline void setQuadratic(const float val) { this->info.quadratic = val; }

    virtual void sendDataToShader(Shader shader) override;

    ~PointLight() = default;
};
