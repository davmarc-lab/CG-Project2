#include "SpotLight.hpp"

#include "../Menu/IGDebug.hpp"

SpotLight::SpotLight(vec3 position, vec3 direction, float constant, float linear, float quadratic) : SpotLight() {
    this->position = position; this->caster->setPosition(position);
    this->direction = direction;
    this->info.constant = constant;
    this->info.linear = linear;
    this->info.quadratic = quadratic;
}

void SpotLight::sendDataToShader(Shader shader, int index) {
    shader.use();

    shader.setInt("lights[" + to_string(index) + "].lightType", this->type);
    shader.setFloat("lights[" + to_string(index) + "].intensity", this->intensity);
    shader.setInt("max_num_lights", max_lights);

    shader.setVec3("lights[" + to_string(index) + "].color", this->color);

    shader.setVec3("lights[" + to_string(index) + "].position", this->caster->getPosition());
    shader.setVec3("lights[" + to_string(index) + "].direction", this->direction);

    shader.setFloat("lights[" + to_string(index) + "].constant", this->info.constant);
    shader.setFloat("lights[" + to_string(index) + "].linear", this->info.linear);
    shader.setFloat("lights[" + to_string(index) + "].quadratic", this->info.quadratic);

    shader.setVec3("lights[" + to_string(index) + "].ambient", this->vectors.ambient);
    shader.setVec3("lights[" + to_string(index) + "].diffuse", this->vectors.diffuse);
    shader.setVec3("lights[" + to_string(index) + "].specular", this->vectors.specular);

    shader.setFloat("lights[" + to_string(index) + "].cutOff", this->cutOff);
    shader.setFloat("lights[" + to_string(index) + "].outer_cutOff", this->outer_cutOff);

    shader.setMat4("projection", projection);
}

void SpotLight::drawCaster() {
    if (this->caster == nullptr) {
        IGDebug::instance()->addLog(LogType::MISSING_CASTER, "No Caster Attached");
        return;
    }

    this->casterShader.use();
    this->casterShader.setVec3("light_color", this->getColor());
    this->casterShader.setMat4("projection", projection);
    this->casterShader.setMat4("view", camera.getViewMatrix());
    this->casterShader.setMat4("model", this->caster->getModelMatrix());
    this->caster->draw(this->casterShader);
}
