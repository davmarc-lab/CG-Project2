#pragma once

#include "Light.hpp"

#include "../Entity/Sphere.hpp"

/*
 * This class implements a Spot light (something like a torch).
 */
class SpotLight : public Light {
  private:
    vec3 position = vec3(0);
    vec3 direction = vec3(1);
    LightInfo info;
    // Light extra parameters.
    float cutOff = 12.5f;
    float outer_cutOff = 17.5f;

  public:
    SpotLight() { this->type = LightType::SPOTLIGHT; }

    SpotLight(vec3 position, vec3 direction, float constant, float linear, float quadratic);

    inline vec3 getDirection() { return this->direction; }

    inline void setDirection(vec3 dir) { this->direction = dir; }

    inline float getConstant() { return this->info.constant; }

    inline void setConstant(const float val) { this->info.constant = val; }

    inline float getLinear() { return this->info.linear; }

    inline void setLinear(const float val) { this->info.linear = val; }

    inline float getQuadratic() { return this->info.quadratic; }

    inline void setQuadratic(const float val) { this->info.quadratic = val; }

    inline float getCutOff() { return this->cutOff; }

    inline void setCutOff(float val) { this->cutOff = val; }

    inline float getOuterCutOff() { return this->outer_cutOff; }

    inline void setOuterCutOff(float val) { this->outer_cutOff = val; }

    inline virtual void initCaster() override {
        this->casterShader = Shader("./resources/shaders/casterVertexShader.glsl", "./resources/shaders/casterFragmentShader.glsl");

        // create light caster
        this->caster = new Sphere();
        this->caster->createVertexArray();
        this->caster->setPosition(this->position);
        this->caster->setScale(vec3(0.1));
    }

    virtual void sendDataToShader(Shader shader, int index) override;

    virtual void drawCaster() override;

    ~SpotLight() = default;
};
