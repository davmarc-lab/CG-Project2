#pragma once

#include "../Lib.hpp"
#include "../LibLightStruct.hpp"

#include "../Shader/Shader.hpp"

class Light {
public:
    vec3 color = vec3(1.0f);
    float intensity = 1.0f;
    LightVectors vectors;
    LightType type;

    vec3 getAmbient() { return this->vectors.ambient; }

    void setAmbient(vec3 ambient) { this->vectors.ambient = ambient; }

    vec3 getDiffuse() { return this->vectors.diffuse; }

    void setDiffuse(vec3 diffuse) { this->vectors.diffuse = diffuse; }

    vec3 getSpecular() { return this->vectors.specular; }

    void setSpecular(vec3 specular) { this->vectors.specular = specular; }

    vec3 getColor() { return this->color; }

    void setColor(vec3 color) { this->color = color; }

    float getIntensity() { return this->intensity; }

    void setIntensity(float intensity) { this->intensity = intensity; } 

    /* ---Overrride Methods--- */
    virtual void sendDataToShader(Shader shader) = 0;
};
