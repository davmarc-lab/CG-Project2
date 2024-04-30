#pragma once

#include "Light.hpp"

class PointLight : public Light
{
    private:
        vec3 position;

        float constant;
        float linear;
        float quadratic;

    public:

        PointLight(vec3 position, float constant, float linear, float quadratic);

        vec3 getPosition() { return this->position; }

        void setPosition(vec3 pos) { this->position = pos; }

        float getConstantValue() { return this->constant; }

        void setConstantValue(float value) { this->constant = value; }
        
        float getLinearValue() { return this->linear; }

        void setLinearValue(float value) { this->linear = value; }

        float getQuadraticValue() { return this->quadratic; }
        
        void setQuadraticValue(float value) { this->quadratic = value; }

        virtual void drawLight(Shader shader) override;
};
