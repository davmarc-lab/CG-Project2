#pragma once

// All structs used for light classes.

#include <string>
#include <glm/vec3.hpp>

// Defines the possible light cater
enum LightType
{
    DIRECTIONAL,
    POINTLIGHT,
    SPOTLIGHT
};

// Defines all light models implemented
enum LightComp {
    PHONG,
    BLINN,
    INT_PHONG,
    INT_BLINN,
    DEFAULT
};

// Basic Light vectors
struct LightVectors {
    glm::vec3 ambient = glm::vec3(0.1f);
    glm::vec3 diffuse = glm::vec3(0.8f);
    glm::vec3 specular = glm::vec3(1.f);
};

// Additional Light properties
struct LightInfo {
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
};

// Simple toString method using the LightType enum values
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
