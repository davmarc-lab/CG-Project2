#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight() {
    this->type = LightType::DIRECTIONAL;
}

void DirectionalLight::sendDataToShader(Shader shader) {
    shader.use();

    shader.setInt("lightType", this->type);
    shader.setVec3("light.direction", this->direction);

    shader.setVec3("light.ambient", this->vectors.ambient);
    shader.setVec3("light.diffuse", this->vectors.diffuse);
    shader.setVec3("light.specular", this->vectors.specular);
}
