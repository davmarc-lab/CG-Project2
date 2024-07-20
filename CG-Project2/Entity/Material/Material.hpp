#pragma once

#include "../../LibLightStruct.hpp"

#include <glm/vec3.hpp>
#include <vector>

class Material {
  private:
    LightVectors vectors;

    float shininess;

    const char* name;

  public:
    // default material = RED_PLASTIC
    Material() : Material(glm::vec3(0), glm::vec3(0.5, 0, 0), glm::vec3(0.7, 0.6, 0.6), 32, "None") {}

    Material(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, float shininess, const char* name);

    inline friend bool operator==(Material m1, Material m2) {
        return m1.shininess == m2.shininess && m1.vectors.ambient == m2.vectors.ambient && m1.vectors.diffuse == m2.vectors.diffuse &&
               m1.vectors.specular == m2.vectors.specular;
    }

    inline glm::vec3 getAmbient() { return this->vectors.ambient; }

    inline glm::vec3 getDiffuse() { return this->vectors.diffuse; }

    inline glm::vec3 getSpecular() { return this->vectors.specular; }

    inline float getShininess() { return this->shininess; }

    inline const char* getName() { return this->name; }

    ~Material() = default;
};

namespace material {
const Material EMERALD(glm::vec3(0.0215f, 0.1745f, 0.0215f), glm::vec3(0.07568f, 0.61424f, 0.07568f), glm::vec3(0.633f, 0.727811f, 0.633f), 76.8f, "Emerald");

const Material BRASS(glm::vec3(0.329412f, 0.223529f, 0.027451f), glm::vec3(0.780392f, 0.568627f, 0.113725f),
                     glm::vec3(0.992157f, 0.941176f, 0.807843f), 27.8974f, "Brass");

const Material SLATE(glm::vec3(0.02, 0.02, 0.02), glm::vec3(0.1, 0.1, 0.1), glm::vec3(0.4, 0.4, 0.4), 1.78125, "Slate");

const Material RED_PLASTIC(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.0f, 0.0f), glm::vec3(0.7f, 0.6f, 0.6f), 32.0f, "Red Plastic");

const Material YELLOW_PLASTIC(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.60f, 0.60f, 0.50f), 32.0f, "Yellow Plastic");

const Material NONE(glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, "None");
inline std::vector<Material> materials = {NONE, EMERALD, BRASS, SLATE, RED_PLASTIC, YELLOW_PLASTIC};
} // namespace material

