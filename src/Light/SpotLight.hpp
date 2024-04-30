#pragma once

#include "Light.hpp"

class SpotLight : public Light
{
    private:
        vec3 position;
        vec3 direction;
        float cutOff;
        float outerCutOff;
        float constant;
        float linear;
        float quadratic;

        bool smooth = false;

    public:
        SpotLight(vec3 position, vec3 direction, float cutOff, float outerCutOff);

        vec3 getPosition() { return this->position; }

        void setPosition(vec3 pos) { this->position = pos; }

        vec3 getDirection() { return this->direction; }

        void setDirection(vec3 dir) { this->direction = dir; }

        float getCutOffValue() { return this->cutOff; }

        void setCutOffValue(float value) { this->outerCutOff = value; }

        float getOuterCutOffValue() { return this->cutOff; }

        void setOuterCutOffValue(float value) { this->outerCutOff = value; }

        float getConstantValue() { return this->constant; }

        void setConstantValue(float value) { this->constant = value; }

        float getLinearValue() { return this->linear; }

        void setLinearValue(float value) { this->linear = value; }

        float getQuadraticValue() { return this->quadratic; }

        void setQuadraticValue(float value) { this->quadratic = value; }

        void setSmooth(bool value) { this->smooth = value; }

        bool isSmooth() { return this->smooth; }

        virtual void drawLight(Shader shader) override;
};
