#pragma once

#include "Light.hpp"

#include "../Entity/Sphere.hpp"

class PointLight : public Light {
  private:
    vec3 position = vec3(0);
    LightInfo info;

  public:
    PointLight() { this->type = LightType::POINTLIGHT; }

    PointLight(vec3 position, float constant, float linear, float quadratic);

    inline float getConstant() { return this->info.constant; }

    inline void setConstant(const float val) { this->info.constant = val; }

    inline float getLinear() { return this->info.linear; }

    inline void setLinear(const float val) { this->info.linear = val; }

    inline float getQuadratic() { return this->info.quadratic; }

    inline void setQuadratic(const float val) { this->info.quadratic = val; }

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

    ~PointLight() = default;
};
