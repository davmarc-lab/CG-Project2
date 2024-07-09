#pragma once

#include <glm/vec3.hpp>

enum LightType
{
    DIRECTIONAL,
    POINTLIGHT,
    SPOTLIGHT
};

struct LightVectors {
    glm::vec3 ambient = glm::vec3(0.2f);
    glm::vec3 diffuse = glm::vec3(0.5f);
    glm::vec3 specular = glm::vec3(1.0f);
};

struct PointLightInfo {
    float constant = 1.0f;
    float linear = 0.22f;
    float quadratic = 0.20f;
};

