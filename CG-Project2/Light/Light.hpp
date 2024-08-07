#pragma once

#include "../LibLightStruct.hpp"

#include "../Entity/Entity.hpp"
#include "../Shader/Shader.hpp"

extern int max_lights;

/*
 * Interface used to define Light classes behavior.
 */
class Light {
  public:
    vec3 color = vec3(1.0f);
    float intensity = 1.0f;
    LightVectors vectors;
    // Defines the kind of light (Directional, Point or Spot).
    LightType type;

    // Create the light caster if necessary (not for Directional lights).
    Entity *caster = nullptr;
    // Caster Shader.
    Shader casterShader;
    bool show_caster = true;

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

    LightType getType() { return this->type; }

    inline vec3 getPosition() { return this->caster == nullptr ? vec3(0) : this->caster->getPosition(); }

    inline void setPosition(vec3 pos) {
        if (this->caster != nullptr)
            this->caster->setPosition(pos);
    }

    inline Entity *getCaster() { return this->caster; }

    inline Shader *getShader() { return &this->casterShader; }

    // Retrieves true if the caster is allocated in memory.
    bool isCasterInstaced() { return this->caster != nullptr || this->type == LightType::DIRECTIONAL; }

    // Tells to the scene if it must draw the light caster or not.
    bool showCaster() { return this->show_caster; }

    // Simple toString to send data to the shader.
    inline string addIndexToString(string before, int index, string after) { return before + "[" + to_string(index) + "]." + after; }

    /* ---Override Methods--- */
    virtual void initCaster() = 0;

    // Virtual method to send all the necessary data of each light to the shader.
    virtual void sendDataToShader(Shader shader, int index) = 0;

    virtual void drawCaster() = 0;
};
