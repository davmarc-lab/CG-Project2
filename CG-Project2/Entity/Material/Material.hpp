#pragma once

#include "../../LibLightStruct.hpp"

#include <glm/vec3.hpp>

class Material {
private:
    LightVectors vectors;

    float shininess;

public:
    // default material = RED_PLASTIC
    Material() : vectors({ glm::vec3(0), glm::vec3(0.5, 0, 0), glm::vec3(0.7, 0.6, 0.6)}), shininess(32) {}

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess);

    inline glm::vec3 getAmbient() { return this->vectors.ambient; }

    inline glm::vec3 getDiffuse() { return this->vectors.diffuse; }

    inline glm::vec3 getSpecular() { return this->vectors.specular; }

    inline float getShininess() { return this->shininess; }

    ~Material() = default;
};

namespace material {
    const Material EMERALD(glm::vec3(0.0215f, 0.1745f, 0.0215f),
            glm::vec3(0.07568f, 0.61424f, 0.07568f),
            glm::vec3(0.633f, 0.727811f, 0.633f),
            76.8f);

    const Material BRASS(glm::vec3(0.329412f, 0.223529f, 0.027451f),
            glm::vec3(0.780392f, 0.568627f, 0.113725f),
            glm::vec3(0.992157f, 0.941176f, 0.807843f),
            27.8974f);

    const Material SLATE(glm::vec3(0.02, 0.02, 0.02),
            glm::vec3(0.1, 0.1, 0.1),
            glm::vec3(0.4, 0.4, 0.4),
            1.78125);

    const Material RED_PLASTIC(glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.5f,0.0f,0.0f),
            glm::vec3(0.7f,0.6f,0.6f),
            32.0f);

    const Material YELLOW_PLASTIC(glm::vec3(0.0f,0.0f,0.0f),
            glm::vec3(0.5f,0.5f,0.0f),
            glm::vec3(0.60f,0.60f,0.50f),
            32.0f);

    const Material NONE(glm::vec3(0.0f),
            glm::vec3(0.0f),
            glm::vec3(0.0f),
            0.0f);
}
