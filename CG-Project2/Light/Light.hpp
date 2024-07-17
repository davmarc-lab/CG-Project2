#pragma once

#include "../Lib.hpp"
#include "../LibLightStruct.hpp"

#include "../Entity/Entity.hpp"
#include "../Shader/Shader.hpp"

extern int max_lights;

class Light {
public:
    vec3 color = vec3(1.0f);
    float intensity = 1.0f;
    LightVectors vectors;
    LightType type;

    Entity* caster;
    Shader casterShader;
    bool show_caster = true;

    mat4 custom_view = mat4(1.0f);


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

    inline void setPosition(vec3 pos) { if (this->caster != nullptr) this->caster->setPosition(pos); }

    inline Entity* getCaster() { return this->caster; }

    inline Shader* getShader() { return &this->casterShader; }

    bool showCaster() { return this->show_caster; }

    inline mat4 getCustomView() { return this->custom_view; }

    inline void setCustomView(mat4 view) { this->custom_view = view; }

    inline string addIndexToString(string before, int index, string after) {
        return before + "[" + to_string(index) + "]." + after;
    }

    /* ---Overrride Methods--- */
    virtual void initCaster() = 0;

    virtual void sendDataToShader(Shader shader, int index) = 0;

    virtual void drawCaster() = 0;
};
