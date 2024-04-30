#include "PointLight.hpp"

PointLight::PointLight(vec3 position, float constant, float linear, float quadratic)
{
    this->position = position;
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;
}

void PointLight::drawLight(Shader shader)
{
    // Set shader type for directional lights
    shader.use();

    shader.setInt("lightType", LightType::POINTLIGHT);

    shader.setVec3("light.position", this->position);
    shader.setFloat("light.constant", this->constant);
    shader.setFloat("light.linear", this->linear);
    shader.setFloat("light.quadratic", this->quadratic);

    shader.setVec3("light.ambient", this->ambient);
    shader.setVec3("light.diffuse", this->diffuse);
    shader.setVec3("light.specular", this->specular);
}
