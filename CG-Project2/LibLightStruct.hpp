#pragma once

#include <string>
#include <glm/vec3.hpp>

enum LightType
{
    DIRECTIONAL,
    POINTLIGHT,
    SPOTLIGHT
};

enum LightComp {
    PHONG,
    BLINN,
    INT_PHONG,
    INT_BLINN,
    NONE
};

struct LightVectors {
    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.8f);
    glm::vec3 specular = glm::vec3(1.f);
};

struct LightInfo {
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

inline std::string getLightTypeName(LightType type) {
    switch (type) {
        case LightType::DIRECTIONAL:
            return "Directional";
        case LightType::POINTLIGHT:
            return "Point";
        case LightType::SPOTLIGHT:
            return "Spot";
    }

    return "No Name";
}
