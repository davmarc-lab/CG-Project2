#pragma once

#include "Light.hpp"

#include "../Menu/Logger/LogManager.hpp"

/*
 * Creates a Directional light (usually is the sun).
 */
class DirectionalLight : public Light {
  private:
    // Pointing direction of the light.
    vec3 direction = vec3(1, 0, 0);

  public:
    DirectionalLight();

    inline vec3 getDirection() { return this->direction; }

    inline void setDirection(vec3 dir) { this->direction = dir; }

    inline virtual void initCaster() override { LogManager::instance()->addLog(logs::MISSING_CASTER, "No Caster Needed for Directional Light"); }

    virtual void sendDataToShader(Shader shader, int index) override;

    virtual void drawCaster() override {}

    ~DirectionalLight() = default;
};
