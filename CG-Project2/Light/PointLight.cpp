#include "PointLight.hpp"

PointLight::PointLight(vec3 position, float constant, float linear, float quadratic) {
    this->position = position;
    this->info.constant = constant;
    this->info.linear = linear;
    this->info.quadratic = quadratic;
}

void PointLight::sendDataToShader(Shader shader) {
    shader.use();

    shader.setInt("lightType", this->type);
    shader.setVec3("light.color", this->color);
    shader.setVec3("light.position", this->position);

    shader.setFloat("light.constant", this->info.constant);
    shader.setFloat("light.linear", this->info.linear);
    shader.setFloat("light.quadratic", this->info.quadratic);

    shader.setVec3("light.ambient", this->vectors.ambient);
    shader.setVec3("light.diffuse", this->vectors.diffuse);
    shader.setVec3("light.specular", this->vectors.specular);

    shader.setMat4("projection", projection);
}
