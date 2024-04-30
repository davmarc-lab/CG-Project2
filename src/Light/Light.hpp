#pragma once

#include "../Lib.hpp"
#include "../Shader/Shader.hpp"

enum LightType
{
    DIRECTIONAL,
    POINTLIGHT,
    SPOTLIGHT
};

class Light
{
    public:
        vec3 color = vec3(1.0f);
        float intensity = 1.0f;
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;

        vec3 getAmbient() { return this->ambient; }

        void setAmbient(vec3 ambient) { this->ambient = ambient; }

        vec3 getDiffuse() { return this->diffuse; }

        void setDiffuse(vec3 diffuse) { this->diffuse = diffuse; }

        vec3 getSpecular() { return this->specular; }

        void setSpecular(vec3 specular) { this->specular = specular; }

        vec3 getColor() { return this->color; }

        void setColor(vec3 color) { this->color = color; }

        float getIntensity() { return this->intensity; }

        void setIntensity(float intensity) { this->intensity = intensity; } 

        /* ---Overrride Methods--- */
        virtual void drawLight(Shader shader) = 0;
};
