#include "DirectionalLight.hpp"

DirectionalLight::DirectionalLight() {
    this->casterShader = Shader("./resources/shaders/casterVertexShader.glsl", "./resources/shaders/casterFragmentShader.glsl");
    this->type = LightType::DIRECTIONAL;
}

void DirectionalLight::sendDataToShader(Shader shader, int index) {
    shader.use();

    shader.setInt("lights[" + to_string(index) + "].lightType", this->type);
    shader.setInt("max_num_lights", max_lights);

    shader.setVec3("lights[" + to_string(index) + "].color", this->color);

    shader.setVec3("lights[" + to_string(index) + "].direction", this->direction);

    shader.setVec3("lights[" + to_string(index) + "].ambient", this->vectors.ambient);
    shader.setVec3("lights[" + to_string(index) + "].diffuse", this->vectors.diffuse);
    shader.setVec3("lights[" + to_string(index) + "].specular", this->vectors.specular);
}

void DirectionalLight::drawCaster() {
    if (this->caster == nullptr) {
        // cout << "No caster Attached" << endl;
        return;
    }
}
