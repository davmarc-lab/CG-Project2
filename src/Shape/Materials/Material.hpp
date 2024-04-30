#pragma once

#include "../../Lib.hpp"

class Material
{
    private:
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        GLfloat shininess;

    public:
        Material() {}
        
        Material(vec3 ambient, vec3 diffuse, vec3 specular, GLfloat shininess);

        vec3 getAmbient() { return this->ambient; }
        vec3 getDiffuse() { return this->diffuse; }
        vec3 getSpecular() { return this->specular; }
        GLfloat getShininess() { return this->shininess; }
};

namespace material
{
    const Material EMERALD(vec3(0.0215f, 0.1745f, 0.0215f),
            vec3(0.07568f, 0.61424f, 0.07568f),
            vec3(0.633f, 0.727811f, 0.633f),
            76.8f);

    const Material BRASS(vec3(0.329412f, 0.223529f, 0.027451f),
            vec3(0.780392f, 0.568627f, 0.113725f),
            vec3(0.992157f, 0.941176f, 0.807843f),
            27.8974f);

    const Material SLATE(vec3(0.02, 0.02, 0.02),
            vec3(0.1, 0.1, 0.1),
            vec3(0.4, 0.4, 0.4),
            1.78125);

    const Material RED_PLASTIC(vec3(0.0f,0.0f,0.0f),
            vec3(0.5f,0.0f,0.0f),
            vec3(0.7f,0.6f,0.6f),
            32.0f);

    const Material YELLOW_PLASTIC(vec3(0.0f,0.0f,0.0f),
            vec3(0.5f,0.5f,0.0f),
            vec3(0.60f,0.60f,0.50f),
            32.0f);
}

