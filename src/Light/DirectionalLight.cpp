#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight(vec3 ambient, vec3 diffuse, vec3 specular, vec3 direction)
{
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
    this->direction = direction;
}

void DirectionalLight::drawLight(Shader shader)
{
    // Set shader type for directional lights
    shader.use();
    shader.setInt("lightType", LightType::DIRECTIONAL);

    shader.setVec3("light.direction", this->direction);

    shader.setVec3("light.ambient", this->ambient);
    shader.setVec3("light.diffuse", this->diffuse);
    shader.setVec3("light.specular", this->specular);
}

