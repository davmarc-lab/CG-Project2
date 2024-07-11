#include "SpotLight.hpp"

SpotLight::SpotLight(vec3 position, vec3 direction, float constant, float linear, float quadratic) {
    this->position = position;
    this->direction = direction;
    this->info.constant = constant;
    this->info.linear = linear;
    this->info.quadratic = quadratic;
}

void SpotLight::sendDataToShader(Shader shader) {
    shader.use();

    shader.setInt("lightType", this->type);
    shader.setVec3("light.color", this->color);

    shader.setVec3("light.position", this->position);
    shader.setVec3("light.direction", this->direction);

    shader.setFloat("light.constant", this->info.constant);
    shader.setFloat("light.linear", this->info.linear);
    shader.setFloat("light.quadratic", this->info.quadratic);

    shader.setVec3("light.ambient", this->vectors.ambient);
    shader.setVec3("light.diffuse", this->vectors.diffuse);
    shader.setVec3("light.specular", this->vectors.specular);

    shader.setFloat("light.cutOff", this->cutOff);
    shader.setFloat("light.outer_cutOff", this->outer_cutOff);

    shader.setMat4("projection", projection);
}
